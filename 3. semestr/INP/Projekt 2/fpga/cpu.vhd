-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Jakub Frýz (xfryzj01)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru

   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

type fsm_state is (
	st_fetch,
	st_decode,
	st_ptr_inc,
	st_ptr_dec,
	st_var_inc_phase01, st_var_inc_phase02,
	st_var_dec_phase01, st_var_dec_phase02,
	st_while_start_phase01, st_while_start_phase02, st_while_start_phase03, st_while_start_phase04,
	st_while_end_phase01, st_while_end_phase02, st_while_end_phase03, st_while_end_phase04, st_while_end_phase05,
	st_get,
	st_put,
	st_push,
	st_pop,
	st_halt,
	st_nop
);
signal present_state	: fsm_state;
signal next_state		: fsm_state;

signal reg_pc_addr		: std_logic_vector(11 downto 0);
signal reg_pc_inc		: std_logic;
signal reg_pc_dec		: std_logic;

signal reg_ptr_addr		: std_logic_vector(9 downto 0);
signal reg_ptr_inc		: std_logic;
signal reg_ptr_dec		: std_logic;

signal reg_cnt_addr		: std_logic_vector(7 downto 0);
signal reg_cnt_inc		: std_logic;
signal reg_cnt_dec		: std_logic;

signal reg_tmp_addr		: std_logic_vector(7 downto 0);

signal cnt_data_aux		: std_logic;
signal data_rdata_aux	: std_logic;

begin

cnt_data_aux	<= '1' when (reg_cnt_addr = "00000000") else '0';
data_rdata_aux	<= '1' when (DATA_RDATA = "00000000") else '0';

--	----------------------------------------------------------------------------
--								Registr PC
--	----------------------------------------------------------------------------
pc_register: process(RESET, CLK)
begin
	if (RESET = '1') then
		reg_pc_addr <= (others => '0');
	elsif (CLK'event) and (CLK='1') then
		if (reg_pc_inc = '1') then
			reg_pc_addr <= reg_pc_addr + 1;
		elsif (reg_pc_dec = '1') then
			reg_pc_addr <= reg_pc_addr - 1;
		end if;
	end if;
end process;

CODE_ADDR <= reg_pc_addr;

--	----------------------------------------------------------------------------
--								Registr PTR
--	----------------------------------------------------------------------------
ptr_register: process(RESET, CLK)
begin
	if (RESET = '1') then
		reg_ptr_addr <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (reg_ptr_inc = '1') then
			reg_ptr_addr <= reg_ptr_addr + 1;
		elsif (reg_ptr_dec = '1') then
			reg_ptr_addr <= reg_ptr_addr - 1;
		end if;
	end if;
end process;

DATA_ADDR <= reg_ptr_addr;

--	----------------------------------------------------------------------------
--								Registr CNT
--	----------------------------------------------------------------------------
cnt_register: process(RESET, CLK)
begin
	if (RESET = '1') then
		reg_cnt_addr <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (reg_cnt_inc = '1') then
			reg_cnt_addr <= reg_cnt_addr + 1;
		elsif (reg_cnt_dec = '1') then
			reg_cnt_addr <= reg_cnt_addr - 1;
		end if;
	end if;
end process;

--	----------------------------------------------------------------------------
--								Konečný automat
--	----------------------------------------------------------------------------
fsm_present: process(RESET, CLK)
begin
	if (RESET = '1') then
		present_state <= st_fetch;
	elsif (CLK'event) and (CLK = '1') then
		if (EN = '1') then
			present_state <= next_state;
		end if;
	end if;
end process;

fsm_next: process(present_state, cnt_data_aux, data_rdata_aux, CLK, RESET, EN, CODE_DATA, DATA_RDATA, IN_DATA, IN_VLD, OUT_BUSY)
begin

	next_state <= st_fetch;

	CODE_EN <= '0';
	DATA_EN <= '0';
	IN_REQ <= '0';
	OUT_WE <= '0';

	reg_pc_inc <= '0';
	reg_pc_dec <= '0';

	reg_ptr_inc <= '0';
	reg_ptr_dec <= '0';

	reg_cnt_inc <= '0';
	reg_cnt_dec <= '0';

	case present_state is

		when st_fetch =>
			CODE_EN <= '1';							-- získání instrukce
			next_state <= st_decode;				-- dekódování instrukce

		when st_decode =>
			case CODE_DATA is						-- dekódování instrukce
				when X"3E"		=> next_state <=	st_ptr_inc;				--	>
				when X"3C"		=> next_state <=	st_ptr_dec;				--	<
				when X"2B"		=> next_state <=	st_var_inc_phase01;		--	+
				when X"2D"		=> next_state <=	st_var_dec_phase01;		--	-
				when X"5B"		=> next_state <=	st_while_start_phase01;	--	[
				when X"5D"		=> next_state <=	st_while_end_phase01;	--	]
				when X"2E"		=> next_state <=	st_put;					--	.
				when X"2C"		=> next_state <=	st_get;					--	,
				when X"24"		=> next_state <=	st_push;				--	$
				when X"21"		=> next_state <=	st_pop;					--	!
				when X"00"		=> next_state <=	st_halt;				--	null
				when others		=> next_state <=	st_nop;					--	other
			end case;

		-- inkrementace ukazatele
		when st_ptr_inc =>
			reg_ptr_inc <= '1';						-- inkrementace ukazatele
			reg_pc_inc <= '1';						-- posun v programu
			next_state <= st_fetch;					-- nová instrukce

		-- dekrementace ukazatele
		when st_ptr_dec =>
			reg_ptr_dec <= '1';						-- dekrementace ukazatele
			reg_pc_inc <= '1';						-- posun v programu
			next_state <= st_fetch;					-- nová instrukce

		-- inkrementace buňky
		when st_var_inc_phase01 =>
			DATA_EN <= '1';							-- povolení RAM
			DATA_RDWR <= '1';						-- čtení dat
			next_state <= st_var_inc_phase02;

		when st_var_inc_phase02 =>
			DATA_EN <= '1';							-- povolení RAM
			DATA_RDWR <= '0';						-- zápis dat
			reg_pc_inc <= '1';						-- posun v programu
			DATA_WDATA <= DATA_RDATA + 1;
			next_state <= st_fetch;					-- nová instrukce

		-- dekrementace buňky
		when st_var_dec_phase01 =>
			DATA_EN <= '1';							-- povolení RAM
			DATA_RDWR <= '1';						-- čtení dat
			next_state <= st_var_dec_phase02;

		when st_var_dec_phase02 =>
			DATA_EN   <= '1';						-- povolení RAM
			DATA_RDWR <= '0';						-- zápis dat
			reg_pc_inc <= '1';						-- posun v programu
			DATA_WDATA <= DATA_RDATA - 1;
			next_state <= st_fetch;					-- nová instrukce

		-- while start
		when st_while_start_phase01 =>
			DATA_EN <= '1';							-- povolení RAM
			DATA_RDWR <= '1';						-- čtení dat
			reg_pc_inc <= '1';						-- posun v programu
			next_state <= st_while_start_phase02;

		when st_while_start_phase02 =>
			if (data_rdata_aux = '1') then
				next_state <= st_while_start_phase03;
			else
				next_state <= st_fetch;
			end if;

		when st_while_start_phase03 =>
			if (cnt_data_aux = '1') then
				reg_cnt_inc <= '1';
				next_state <= st_fetch;
			else
				CODE_EN <= '1';
				next_state <= st_while_start_phase04;
			end if;

		when st_while_start_phase04 =>
			if (CODE_DATA = X"5B") then				-- [
				reg_cnt_inc <= '1';
			elsif (CODE_DATA = X"5D") then			-- ]
				reg_cnt_dec <= '1';
			end if;
			reg_pc_inc <= '1';
			next_state <= st_while_start_phase03;

		-- while end
		when st_while_end_phase01 =>
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			next_state <= st_while_end_phase02;

		when st_while_end_phase02 =>
			if (data_rdata_aux = '1') then
				reg_pc_inc <= '1';
				next_state <= st_fetch;
			else
				reg_cnt_inc <= '1';
				reg_pc_dec <= '1';
				next_state <= st_while_end_phase03;
			end if;

		when st_while_end_phase03 =>
			if (cnt_data_aux = '1') then
				next_state <= st_fetch;
			else
				CODE_EN <= '1';
				next_state <= st_while_end_phase04;
			end if;

		when st_while_end_phase04 =>
			 if (CODE_DATA = X"5B") then			-- [
				reg_cnt_dec <= '1';
			elsif (CODE_DATA = X"5D") then			-- ]
				reg_cnt_inc <= '1';
			end if;
			next_state <= st_while_end_phase05;

		when st_while_end_phase05 =>
			if (cnt_data_aux = '1') then
				reg_pc_inc <= '1';
			else
				reg_pc_dec <= '1';
			end if;
			next_state <= st_while_end_phase03;

		-- get
		when st_get =>
			IN_REQ <= '1';							-- požadování vstupu
			if (IN_VLD = '0') then					-- pokud data nejsou platná
				next_state <= st_get;				-- čekej
			else									-- pokud data jsou platná
				DATA_RDWR <= '0';					-- zápis dat
				DATA_EN <= '1';						-- povolení RAM
				reg_pc_inc <= '1';					-- posun v programu
				DATA_WDATA <= IN_DATA;
				next_state <= st_fetch;				-- nová instrukce
			end if;

		-- put
		when st_put =>
			if (OUT_BUSY = '1') then				-- pokud je LCD zaneprázděno
				next_state <= st_put;				-- čekej
			else									-- pokud je LCD nezaneprázděno
				DATA_RDWR <= '1';					-- čtení dat
				OUT_DATA <= DATA_RDATA;				-- výstup dat
				OUT_WE <= '1';						-- povolení výstupu
				reg_pc_inc <= '1';					-- posun v programu
				next_state <= st_fetch;				-- nová instrukce
			end if;

		-- push to TMP
		when st_push =>
			DATA_EN <= '1';							-- povolení RAM
			DATA_RDWR <= '1';						-- čtení dat
			reg_tmp_addr <= DATA_RDATA;				-- zápis dat do pomocné proměnné
			reg_pc_inc <= '1';						-- posun v programu
			next_state <= st_fetch;					-- nová instrukce

		-- pop from TMP
		when st_pop =>
			DATA_EN <= '1';							-- povolení RAM
			DATA_RDWR <= '0';						-- zápis dat
			DATA_WDATA <= reg_tmp_addr;				-- zápis dat z pomocné proměnné
			reg_pc_inc <= '1';						-- posun v programu
			next_state <= st_fetch;					-- nová instrukce

		-- halt
		when st_halt =>
			next_state <= st_halt;					-- konec

		-- nop
		when st_nop =>
			reg_pc_inc <= '1';						-- posun v programu
			next_state <= st_fetch;					-- nová instrukce

		when others => null;

	end case;

end process;

end behavioral;

