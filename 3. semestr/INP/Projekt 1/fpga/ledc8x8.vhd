library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
	port ( -- Sem doplnte popis rozhrani obvodu.
		ROWS	: out std_logic_vector (0 to 7);
		LEDS	: out std_logic_vector (0 to 7);
		RESET	: in std_logic;
		SMCLK	: in std_logic
	);
end ledc8x8;

architecture main of ledc8x8 is
	-- Sem doplnte definice vnitrnich signalu.
	signal radky	: std_logic_vector(7 downto 0);
	signal ledky	: std_logic_vector(7 downto 0);
	signal citac	: std_logic_vector(22 downto 0);
	signal enable	: std_logic;
	signal switch	: std_logic;
begin
	-- Sem doplnte popis funkce obvodu (zakladni konstrukce VHDL jako napr.
	-- prirazeni signalu, multiplexory, dekodery, procesy...).
	-- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL UVEDENE NA WEBU:
	-- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html

	-- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
	-- v souboru ledc8x8.ucf.

	counter: process(SMCLK, RESET)
	begin
		if (RESET = '1') then
			citac <= (others => '0');
		elsif (SMCLK'event) and (SMCLK = '1') then
			citac <= citac + 1;
		end if;
	end process counter;

	enable <= '1' when citac(7 downto 0) = "11111111" else '0';
	--switch <= '1' when citac = "" else '0';
	switch <= citac(22);

	registry: process(RESET, SMCLK, enable)
	begin
		if (RESET = '1') then
			radky <= (others => '0');
		elsif (SMCLK'event) and (SMCLK = '1') and (enable = '1') then
			case radky is
				when "10000000" =>	radky	<= "01000000";
				when "01000000" =>	radky	<= "00100000";
				when "00100000" =>	radky	<= "00010000";
				when "00010000" =>	radky	<= "00001000";
				when "00001000" =>	radky	<= "00000100";
				when "00000100" =>	radky	<= "00000010";
				when "00000010" =>	radky	<= "00000001";
				when "00000001" =>	radky	<= "10000000";
				-- failsafe
				when others =>		radky	<= "10000000";
			end case;
		end if;
	end process registry;

	-- # # # # # # # # # # - # # # # # # # # # #
	-- # 1 1 1 1 1 1 1 1 # - # 1 1 1 1 1 1 1 1 #
	-- # 1 0 0 0 0 0 0 1 # - # 1 0 0 0 0 0 0 1 #
	-- # 1 1 1 1 1 1 0 1 # - # 1 0 1 1 1 1 1 1 #
	-- # 1 1 1 1 1 1 0 1 # - # 1 0 1 1 1 1 1 1 #
	-- # 1 0 1 1 1 1 0 1 # - # 1 0 0 0 1 1 1 1 #
	-- # 1 0 0 1 1 0 0 1 # - # 1 0 1 1 1 1 1 1 #
	-- # 1 1 1 0 0 1 1 1 # - # 1 0 1 1 1 1 1 1 #
	-- # 1 1 1 1 1 1 1 1 # - # 1 1 1 1 1 1 1 1 #
	-- # # # # # # # # # # - # # # # # # # # # #

	decoder: process(radky)
	begin
		if (switch = '0') then
			case radky is
				when "10000000" =>	ledky	<= "11111111";
				when "01000000" =>	ledky	<= "10000001";
				when "00100000" =>	ledky	<= "11111101";
				when "00010000" =>	ledky	<= "11111101";
				when "00001000" =>	ledky	<= "10111101";
				when "00000100" =>	ledky	<= "10011001";
				when "00000010" =>	ledky	<= "11100111";
				when "00000001" =>	ledky	<= "11111111";
				-- failsafe
				when others =>		ledky <= "11111111";
			end case;
		else
			case radky is
				when "10000000" =>	ledky	<= "11111111";
				when "01000000" =>	ledky	<= "10000001";
				when "00100000" =>	ledky	<= "10111111";
				when "00010000" =>	ledky	<= "10111111";
				when "00001000" =>	ledky	<= "10001111";
				when "00000100" =>	ledky	<= "10111111";
				when "00000010" =>	ledky	<= "10111111";
				when "00000001" =>	ledky	<= "11111111";
				-- failsafe
				when others =>		ledky <= "11111111";
			end case;
		end if;
	end process decoder;

	ROWS <= radky;
	LEDS <= ledky;

end main;
