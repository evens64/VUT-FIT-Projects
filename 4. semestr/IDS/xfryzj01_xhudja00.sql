-- IDS skript
-- Jakub Frýz (xfryzj01), Jakub Hud (xhudja00)

--------------------------------------------------
--              ALTER SESSION
--------------------------------------------------
ALTER SESSION SET NLS_DATE_FORMAT = 'DD.MM.YYYY HH24:MI:SS';
SET serveroutput ON;

--------------------------------------------------
--              DROP SEQUENCE
--------------------------------------------------
DROP SEQUENCE SQ_hraci;
DROP SEQUENCE SQ_postavy;
DROP SEQUENCE SQ_tazeni;
DROP SEQUENCE SQ_dobrodruzstvi;
DROP SEQUENCE SQ_sezeni;
DROP SEQUENCE SQ_umrti;
DROP SEQUENCE SQ_inventare;
DROP SEQUENCE SQ_vybaveni;
DROP SEQUENCE SQ_ucastnici;

--------------------------------------------------
--               DROP TABLES
--------------------------------------------------
DROP TABLE hraci			CASCADE CONSTRAINTS;
DROP TABLE postavy			CASCADE CONSTRAINTS;
DROP TABLE tazeni			CASCADE CONSTRAINTS;
DROP TABLE dobrodruzstvi	CASCADE CONSTRAINTS;
DROP TABLE sezeni			CASCADE CONSTRAINTS;
DROP TABLE umrti			CASCADE CONSTRAINTS;
DROP TABLE inventare		CASCADE CONSTRAINTS;
DROP TABLE vybaveni			CASCADE CONSTRAINTS;
DROP TABLE ucastnici		CASCADE CONSTRAINTS;

--------------------------------------------------
--             CREATE SEQUENCE
--------------------------------------------------
CREATE SEQUENCE SQ_hraci			MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_postavy			MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_tazeni 			MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_dobrodruzstvi	MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_sezeni			MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_umrti 			MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_inventare		MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_vybaveni			MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;
CREATE SEQUENCE SQ_ucastnici		MINVALUE 1 START WITH 1 INCREMENT BY 1 CACHE 10;

--------------------------------------------------
--             CREATE TABLES
--------------------------------------------------
CREATE TABLE hraci (
	ID_hrace			NUMBER			DEFAULT SQ_hraci.nextval	NOT NULL,
	prezdivka			VARCHAR2(25)	NOT NULL,
	jmeno				VARCHAR2(25),
	prijmeni			VARCHAR2(25),
	heslo				VARCHAR2(20)	NOT NULL,
	email				VARCHAR2(35)	NOT NULL,
	datum_narozeni		DATE			NOT NULL,
	admin				CHAR(1)			DEFAULT 'N'		NOT NULL,
	CONSTRAINT PK_hraci					PRIMARY KEY (ID_hrace),
	CONSTRAINT UN_email					UNIQUE (email),
	CONSTRAINT UN_prezdivka				UNIQUE (prezdivka),
	CONSTRAINT CK_admin					CHECK (admin='Y' OR admin='N')
);

CREATE TABLE postavy (
	ID_postavy			NUMBER			DEFAULT SQ_postavy.nextval	NOT NULL,
	jmeno				VARCHAR2(25)	NOT NULL,
	pohlavi				CHAR(1)			NOT NULL,
	uroven				NUMBER			DEFAULT 0		NOT NULL,
	rasa				VARCHAR2(20)	NOT NULL,
	povolani			VARCHAR2(20)	NOT NULL,
	vlastnik			VARCHAR2(25)	NOT NULL,
	CONSTRAINT PK_postavy				PRIMARY KEY (ID_postavy),
	CONSTRAINT UN_jmeno					UNIQUE (jmeno),
	CONSTRAINT CK_pohlavi				CHECK (pohlavi='F' OR pohlavi='M')
);

CREATE TABLE vybaveni (
	ID_vybaveni			NUMBER			NOT NULL,
	nazev				VARCHAR2(50)	NOT NULL,
	druh				CHAR(1)			NOT NULL,		-- W=weapon, A=armor, S=shield, P=potion, K=key, M=magic, S=scroll
	typ					VARCHAR2(10)	NOT NULL,		-- greatsword, health potion, your room's key
	kvalita				NUMBER			DEFAULT 5		NOT NULL,
	stat1				CHAR(3)			NOT NULL,
	stat2				NUMBER			NOT NULL,
	CONSTRAINT PK_vybaveni				PRIMARY KEY (ID_vybaveni),
	CONSTRAINT UN_nazev					UNIQUE (nazev),
	CONSTRAINT CK_druh					CHECK (druh='W' OR druh='A' OR druh='S' OR druh='P' OR druh='K' OR druh='M' OR druh='S'),
	CONSTRAINT CK_kvalita				CHECK (kvalita>=1 AND kvalita<=5),
	CONSTRAINT CK_stat1					CHECK (stat1='DMG' OR stat1='DEF' OR stat1='STR' OR stat1='PER' OR stat1='END' OR stat1='CHA' OR stat1='INT' OR stat1='AGI' OR stat1='LCK' OR stat1='HEA' OR stat1='MAN' OR stat1='STA')
);

CREATE TABLE tazeni (
	ID_tazeni			NUMBER			DEFAULT SQ_tazeni.nextval	NOT NULL,
	cil					VARCHAR2(50)	NOT NULL,
	kontinent			VARCHAR2(20)	NOT NULL,
	autor				VARCHAR2(25)	NOT NULL,
	CONSTRAINT PK_tazeni				PRIMARY KEY (ID_tazeni)
);

CREATE TABLE dobrodruzstvi (
	ID_dobrodruzstvi	NUMBER			DEFAULT SQ_dobrodruzstvi.nextval	NOT NULL,
	cil					VARCHAR2(50)	NOT NULL,
	lokalita			VARCHAR2(20)	NOT NULL,
	obtiznost			NUMBER			NOT NULL,
	autor				VARCHAR2(25)	NOT NULL,
	ID_tazeni			NUMBER,
	CONSTRAINT PK_dobrodruzstvi			PRIMARY KEY (ID_dobrodruzstvi),
	CONSTRAINT CK_obtiznost				CHECK (obtiznost>=1 AND obtiznost<=3)
);

CREATE TABLE sezeni (
	ID_sezeni			NUMBER			DEFAULT SQ_sezeni.nextval	NOT NULL,
	pan_jeskyne			VARCHAR2(25)	NOT NULL,
	kdy					DATE			NOT NULL,
	kde					VARCHAR2(50)	NOT NULL,
	ID_dobrodruzstvi	NUMBER			NOT NULL,
	CONSTRAINT PK_sezeni				PRIMARY KEY (ID_sezeni)
);

--------------------------------------------------
--     CREATE TABLE (TABULKY PRO M:N RELACE)
--------------------------------------------------
CREATE TABLE umrti (
	ID_umrti			NUMBER			DEFAULT SQ_umrti.nextval	NOT NULL,
	postava				VARCHAR2(25)	NOT NULL,
	ID_sezeni			NUMBER			NOT NULL,
	CONSTRAINT PK_umrti					PRIMARY KEY (ID_umrti),
	CONSTRAINT UN_postava				UNIQUE (postava)
);

CREATE TABLE inventare (
	ID_inventare		NUMBER			DEFAULT SQ_inventare.nextval	NOT NULL,
	postava				VARCHAR2(25)	NOT NULL,
	predmet				VARCHAR2(50)	NOT NULL,
	pocet				NUMBER			DEFAULT 1		NOT NULL,
	nalezeno			NUMBER			NOT NULL,
	CONSTRAINT PK_inventare				PRIMARY KEY (ID_inventare)
);

CREATE TABLE ucastnici (
	ID_ucastnika		NUMBER			DEFAULT SQ_ucastnici.nextval	NOT NULL,
	ucastnik			VARCHAR2(25)	NOT NULL,
	ID_sezeni			NUMBER			NOT NULL,
	CONSTRAINT PK_ucastnici				PRIMARY KEY (ID_ucastnika)
);

--------------------------------------------------
--                 ALTER TABLE
--------------------------------------------------
ALTER TABLE postavy				ADD CONSTRAINT FK_vlastnik 				FOREIGN KEY (vlastnik)			REFERENCES hraci(prezdivka);
ALTER TABLE tazeni 				ADD CONSTRAINT FK_autor1 				FOREIGN KEY (autor)				REFERENCES hraci(prezdivka);
ALTER TABLE dobrodruzstvi		ADD CONSTRAINT FK_autor2 				FOREIGN KEY (autor)				REFERENCES hraci(prezdivka);
ALTER TABLE dobrodruzstvi		ADD CONSTRAINT FK_tazeni 				FOREIGN KEY (ID_tazeni)			REFERENCES tazeni(ID_tazeni);
ALTER TABLE sezeni 				ADD CONSTRAINT FK_pan_jeskyne 			FOREIGN KEY (pan_jeskyne)		REFERENCES hraci(prezdivka);
ALTER TABLE sezeni 				ADD CONSTRAINT FK_dobrodruzstvi			FOREIGN KEY (ID_dobrodruzstvi)	REFERENCES dobrodruzstvi(ID_dobrodruzstvi);
ALTER TABLE umrti 				ADD CONSTRAINT FK_postava1				FOREIGN KEY (postava)			REFERENCES postavy(jmeno);
ALTER TABLE umrti 				ADD CONSTRAINT FK_sezeni1				FOREIGN KEY (ID_sezeni)			REFERENCES sezeni(ID_sezeni);
ALTER TABLE inventare			ADD CONSTRAINT FK_postava2				FOREIGN KEY (postava)			REFERENCES postavy(jmeno);
ALTER TABLE inventare			ADD CONSTRAINT FK_predmet				FOREIGN KEY (predmet)			REFERENCES vybaveni(nazev);
ALTER TABLE inventare			ADD CONSTRAINT FK_nalezeno				FOREIGN KEY (nalezeno)			REFERENCES sezeni(ID_sezeni);
ALTER TABLE ucastnici			ADD CONSTRAINT FK_ucastnik				FOREIGN KEY (ucastnik)			REFERENCES postavy(jmeno);
ALTER TABLE ucastnici			ADD CONSTRAINT FK_sezeni2				FOREIGN KEY (ID_sezeni)			REFERENCES sezeni(ID_sezeni);

--------------------------------------------------
--                COMMENT
--------------------------------------------------
COMMENT ON COLUMN VYBAVENI.DRUH IS 'W=weapon, A=armor, S=shield, P=potion, K=key, M=magic, S=scroll';

--------------------------------------------------
--             CREATE TRIGGER
--------------------------------------------------
-- kontrola, zda-li není učástník už přihlášen nebo mrtev
CREATE OR REPLACE TRIGGER trigger_participant
BEFORE INSERT ON ucastnici
FOR EACH ROW
DECLARE
	row_count NUMBER;
BEGIN

	-- kontrola zda-li není postava už mrtvá
	SELECT COUNT(*) INTO row_count FROM umrti WHERE postava = :new.ucastnik;
	IF (row_count > 0)
	THEN
		RAISE_APPLICATION_ERROR(-20000, 'Postava se nemůže zúčastnit sezení, je totiž mrtvá.');
	ELSE
		-- kontrola zda-li se postava už setkání nezúčastňuje
		SELECT COUNT(*) INTO row_count FROM ucastnici WHERE ucastnik = :new.ucastnik AND ID_sezeni = :new.ID_sezeni;
		IF (row_count > 0)
		THEN
			RAISE_APPLICATION_ERROR(-20001, 'Postava se už setkání zúčastňuje.');
		END IF;
	END IF;

END trigger_paticipant;
/
SHOW ERRORS;

-- vygenerování ID pomocí sekvence pro tabulku vybaveni
CREATE OR REPLACE TRIGGER trigger_generate
BEFORE INSERT ON vybaveni
FOR EACH ROW
BEGIN
	IF (:new.ID_vybaveni IS NULL)
	THEN
		:new.ID_vybaveni := SQ_vybaveni.nextval;
	END IF;
END trigger_generate;
/
SHOW ERRORS;

--------------------------------------------------
--             CREATE PROCEDURE
--------------------------------------------------
-- vypíše statistiku o hráči a jeho postavách
CREATE OR REPLACE PROCEDURE player_stats(player IN VARCHAR2)
IS
	CURSOR player_info IS SELECT * FROM postavy WHERE VLASTNIK = player;
	info_helper player_info%ROWTYPE;
	live VARCHAR2(10);
	row_count NUMBER;
	my_ex EXCEPTION;
BEGIN
	SELECT COUNT(*) INTO row_count FROM hraci WHERE prezdivka = player;
	IF (row_count = 0)
	THEN
		RAISE my_ex;
	END IF;

	DBMS_OUTPUT.PUT_LINE('Informace o hráči: ' || player);
	DBMS_OUTPUT.PUT_LINE('');
	
	SELECT COUNT(*) INTO row_count FROM postavy WHERE vlastnik = player;
	DBMS_OUTPUT.PUT_LINE('Počet postav:' || CHR(9) || row_count);
	DBMS_OUTPUT.PUT_LINE('');
	
	DBMS_OUTPUT.PUT_LINE('Autor:');
	SELECT COUNT(*) INTO row_count FROM tazeni WHERE autor = player;
	DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Tažení:' || CHR(9) || CHR(9) || CHR(9) || row_count);
	SELECT COUNT(*) INTO row_count FROM dobrodruzstvi WHERE autor = player;
	DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Dobrodružství:' || CHR(9) || row_count);
	DBMS_OUTPUT.PUT_LINE('');
	
	SELECT COUNT(*) INTO row_count FROM sezeni WHERE pan_jeskyne = player;
	DBMS_OUTPUT.PUT_LINE('Pán Jeskyně: ' || row_count || '-krát');
	DBMS_OUTPUT.PUT_LINE('');
	
	OPEN player_info;
	LOOP
		FETCH player_info INTO info_helper;
		EXIT WHEN player_info%NOTFOUND;

		SELECT COUNT(*) INTO row_count FROM umrti WHERE postava = info_helper.jmeno;
		IF (row_count > 0)
		THEN
			live := 'mrtvá';
		ELSE
			live := 'živá';
		END IF;

		DBMS_OUTPUT.PUT_LINE('Postava: ' || info_helper.jmeno || ' [' || info_helper.uroven || '|' || info_helper.pohlavi || '] - ' || live);
		DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Rasa:' || CHR(9) || CHR(9) || info_helper.rasa);
		DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Povoláni:' || CHR(9) || info_helper.povolani);

		SELECT COUNT(*) INTO row_count FROM ucastnici WHERE ucastnik = info_helper.jmeno;
		DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Zúčastnila se:' || CHR(9) || row_count || ' sezení');

		DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Inventář:');
		SELECT COUNT(DISTINCT predmet) INTO row_count FROM inventare WHERE postava = info_helper.jmeno;
		DBMS_OUTPUT.PUT_LINE(CHR(9) || CHR(9) || 'Počet různých předmětů:' || CHR(9) || row_count);
		SELECT SUM(nalezeno) INTO row_count FROM inventare WHERE postava = info_helper.jmeno;
		DBMS_OUTPUT.PUT_LINE(CHR(9) || CHR(9) || 'Celkový počet předmětů:' || CHR(9) || row_count);

		DBMS_OUTPUT.PUT_LINE('');
	END LOOP;
EXCEPTION
	WHEN my_ex THEN
		raise_application_error(-20003,'Hráč "' || player || '" neexistuje');
	WHEN OTHERS THEN
		raise_application_error(-20999,'Nastala chyba');
END player_stats;
/
SHOW ERRORS;

CREATE OR REPLACE PROCEDURE upcoming_sessions
IS
	CURSOR session_info IS SELECT * FROM sezeni ORDER BY kdy;
	info_helper session_info%ROWTYPE;
	quest dobrodruzstvi.cil%TYPE;
BEGIN
	DBMS_OUTPUT.PUT_LINE('Nadcházející sezení:');
	DBMS_OUTPUT.PUT_LINE('');
	
	OPEN session_info;
	LOOP
		FETCH session_info INTO info_helper;
		EXIT WHEN session_info%NOTFOUND;
		
		IF (info_helper.kdy > SYSDATE)
		THEN
			SELECT DISTINCT cil INTO quest FROM dobrodruzstvi WHERE ID_dobrodruzstvi = info_helper.id_dobrodruzstvi;
			DBMS_OUTPUT.PUT_LINE('Úkol: ' || quest);
			DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Pán jeskyně: ' || info_helper.pan_jeskyne);
			DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Kdy: ' || info_helper.kdy);
			DBMS_OUTPUT.PUT_LINE(CHR(9) || 'Kde: ' || info_helper.kde);
			DBMS_OUTPUT.PUT_LINE('');
		END IF;
		
	END LOOP;
END;
/
SHOW ERRORS;

--------------------------------------------------
--           Oprávnění pro xhudja00
--------------------------------------------------
GRANT ALL ON hraci				TO xhudja00;
GRANT ALL ON postavy			TO xhudja00;
GRANT ALL ON tazeni				TO xhudja00;
GRANT ALL ON dobrodruzstvi		TO xhudja00;
GRANT ALL ON sezeni				TO xhudja00;
GRANT ALL ON umrti				TO xhudja00;
GRANT ALL ON inventare			TO xhudja00;
GRANT ALL ON vybaveni			TO xhudja00;
GRANT ALL ON ucastnici			TO xhudja00;
GRANT EXECUTE ON player_stats			TO xhudja00;
GRANT EXECUTE ON upcoming_sessions		TO xhudja00;

--------------------------------------------------
--            INSERT INTO hraci
--------------------------------------------------
INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni, admin)
VALUES ('xfryzj01', 'Jakub', 'Frýz', '1234', 'xfryzj01@stud.fit.vutbr.cz', to_date('1996-07-22', 'YYYY-MM-DD'), 'Y');

INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni, admin)
VALUES ('xhudja00', 'Jakub', 'Hud', '1234', 'xhudja00@stud.fit.vutbr.cz', to_date('1996-12-13', 'YYYY-MM-DD'), 'Y');

INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni)
VALUES ('xhrabo08', 'Michal', 'Hrabovský', '1234', 'xhrabo08@stud.fit.vutbr.cz', to_date('1996-09-25', 'YYYY-MM-DD'));

INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni)
VALUES ('xjanik20', 'Roman', 'Janík', '1234', 'xjanik20@stud.fit.vutbr.cz', to_date('1997-02-11', 'YYYY-MM-DD'));

INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni)
VALUES ('xdosta46', 'Filip', 'Dostálík', '1234', 'xdosta46@stud.fit.vutbr.cz', to_date('1995-08-21', 'YYYY-MM-DD'));

INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni)
VALUES ('collide', 'Roman', 'Tyčka', '1234', 'email@seznam.cz', to_date('2010-08-21', 'YYYY-MM-DD'));

--------------------------------------------------
--              INSERT INTO postavy
--------------------------------------------------
INSERT INTO postavy (jmeno, pohlavi, uroven, rasa, povolani, vlastnik)
VALUES ('Laura Icecrown', 'F', 22, 'night elf', 'mage', 'xfryzj01');

INSERT INTO postavy (jmeno, pohlavi, rasa, povolani, vlastnik)
VALUES ('Jimmy J', 'M', 'human', 'warrior', 'xfryzj01');

INSERT INTO postavy (jmeno, pohlavi, rasa, povolani, vlastnik)
VALUES ('douche', 'M', 'orc', 'paladin', 'xhrabo08');

INSERT INTO postavy (jmeno, pohlavi, uroven, rasa, povolani, vlastnik)
VALUES ('Dark Knight', 'F', 45, 'high elf', 'death knight', 'collide');

INSERT INTO postavy (jmeno, pohlavi, rasa, povolani, vlastnik)
VALUES ('Samael', 'M', 'human', 'druid', 'xdosta46');

INSERT INTO postavy (jmeno, pohlavi, rasa, povolani, vlastnik)
VALUES ('Sherlock', 'M', 'blood elf', 'warlock', 'xhudja00');

--------------------------------------------------
--           INSERT INTO vybaveni
--------------------------------------------------
INSERT INTO vybaveni
VALUES (SQ_vybaveni.nextval, 'Lektvar poškození zdraví', 'P', 'jed', 2, 'HEA', -25);

INSERT INTO vybaveni
VALUES (SQ_vybaveni.nextval, 'Lektvar uzdravení', 'P', 'lektvar', 2, 'HEA', +25);

INSERT INTO vybaveni
VALUES (SQ_vybaveni.nextval, 'Železná helma', 'A', 'helma', 3, 'DEF', 10);

INSERT INTO vybaveni
VALUES (SQ_vybaveni.nextval, 'Ocelový obouruční meč', 'W', '2H meč', 2, 'DMG', 20);

--------------------------------------------------
--           INSERT INTO tazeni
--------------------------------------------------
INSERT INTO tazeni				-- 01
VALUES (SQ_tazeni.nextval, 'Prolomit brány Mordoru', 'Středozemě', 'xfryzj01');

INSERT INTO tazeni				-- 02
VALUES (SQ_tazeni.nextval, 'Porazit Divoký hon', 'Kontinent', 'xfryzj01');

--------------------------------------------------
--        INSERT INTO dobrodruzstvi
--------------------------------------------------
INSERT INTO dobrodruzstvi		-- 01
VALUES (SQ_dobrodruzstvi.nextval, 'Porazit Sarumana', 'Isengard', 2, 'xfryzj01', 1);

INSERT INTO dobrodruzstvi		-- 02
VALUES (SQ_dobrodruzstvi.nextval, 'Porazit Saurona', 'Mordor', 3, 'xfryzj01', 1);

INSERT INTO dobrodruzstvi		-- 03
VALUES (SQ_dobrodruzstvi.nextval, 'Zachránit Yennefer', 'Vizima', 1, 'xfryzj01', 2);

--------------------------------------------------
--          INSERT INTO sezeni
--------------------------------------------------
INSERT INTO sezeni				-- 01
VALUES (SQ_sezeni.nextval, 'xfryzj01', to_date('2017-03-15 17:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'u Kuby doma', 1);

INSERT INTO sezeni				-- 02
VALUES (SQ_sezeni.nextval, 'xfryzj01', to_date('2017-03-16 17:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'u Kuby doma', 1);

INSERT INTO sezeni				-- 03
VALUES (SQ_sezeni.nextval, 'xfryzj01', to_date('2017-03-17 17:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'v Chajovně', 1);

INSERT INTO sezeni				-- 04
VALUES (SQ_sezeni.nextval, 'xhrabo08', to_date('2017-03-18 17:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'u Michala doma', 1);

INSERT INTO sezeni				-- 05 - pro proceduru upcoming_sessions
VALUES (SQ_sezeni.nextval, 'xfryzj01', SYSDATE + 2, 'u Kuby doma', 2);		

INSERT INTO sezeni				-- 06 - pro proceduru upcoming_sessions
VALUES (SQ_sezeni.nextval, 'xhudja00', SYSDATE + 1, 'v Lužánkách', 3);

--------------------------------------------------
--          INSERT INTO inventare
--------------------------------------------------
INSERT INTO inventare
VALUES (SQ_inventare.nextval, 'Laura Icecrown', 'Lektvar poškození zdraví', 2, 4);

INSERT INTO inventare
VALUES (SQ_inventare.nextval, 'Samael', 'Lektvar poškození zdraví', 3, 1);

INSERT INTO inventare
VALUES (SQ_inventare.nextval, 'Samael', 'Lektvar poškození zdraví', 1, 2);

--------------------------------------------------
--        INSERT INTO ucastnici
--------------------------------------------------
INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Samael', 1);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Sherlock', 1);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'douche', 1);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Samael', 2);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Sherlock', 2);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'douche', 2);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Samael', 3);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Sherlock', 3);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'douche', 3);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Dark Knight', 3);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Samael', 4);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Sherlock', 4);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'douche', 4);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Laura Icecrown', 4);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Samael', 5);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Sherlock', 5);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'douche', 5);

INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Laura Icecrown', 5);

--------------------------------------------------
--             INSERT INTO umrti
--------------------------------------------------
INSERT INTO umrti
VALUES (SQ_umrti.nextval, 'Dark Knight', 3);

--------------------------------------------------
--              SELECT FROM
--------------------------------------------------
--Co bylo cílem dobrodruzství 16.3.2017?
SELECT D.cil
FROM dobrodruzstvi D, sezeni S
WHERE S.kdy = to_date('2017-03-16 17:00:00', 'YYYY-MM-DD HH24:MI:SS')
AND S.ID_dobrodruzstvi = D.ID_dobrodruzstvi;

--Jaké vybavení nalezené v průběhu sezení 15.3.2017 vlastní postava Samael?
SELECT I.predmet, I.pocet
FROM inventare I, sezeni S
WHERE I.postava = 'Samael'
AND I.nalezeno = S.ID_sezeni
AND S.kdy = to_date('2017-03-15 17:00:00', 'YYYY-MM-DD HH24:MI:SS');

--Které postavy se zúčastnily dobrodružství "Porazit Saurona"?
SELECT U.ucastnik
FROM ucastnici U, sezeni S, dobrodruzstvi D
WHERE D.cil = 'Porazit Saurona' AND D.ID_dobrodruzstvi = S.ID_dobrodruzstvi
AND S.ID_sezeni = U.ID_sezeni;

--Kolik postav vlastní hráč Jakub Frýz?
SELECT H.jmeno, H.prijmeni, COUNT(*) "Pocet postav"
FROM hraci H, postavy P
WHERE H.jmeno = 'Jakub' AND H.prijmeni = 'Frýz'
AND H.prezdivka = P.vlastnik
GROUP BY H.jmeno, H.prijmeni;

--Kolik má postava Samael celkem předmětů v inventáři?
SELECT I.postava, SUM(pocet) "Pocet predmetu"
FROM inventare I
WHERE I.postava = 'Samael'
GROUP BY I.postava;

--Kteří hráči se zúčastnili sezení 15.3.2017 i sezení 16.3.2017?
SELECT H.jmeno, H.prijmeni
FROM sezeni S, ucastnici U, postavy P, hraci H
WHERE S.kdy = to_date('2017-03-15 17:00:00', 'YYYY-MM-DD HH24:MI:SS')
AND S.ID_sezeni = U.ID_sezeni AND U.ucastnik = P.jmeno
AND P.vlastnik = H.prezdivka
AND EXISTS (
	SELECT *
	FROM sezeni S, ucastnici U, postavy P
	WHERE S.kdy = to_date('2017-03-16 17:00:00', 'YYYY-MM-DD HH24:MI:SS')
	AND S.ID_sezeni = U.ID_sezeni AND U.ucastnik = P.jmeno
	AND P.vlastnik = H.prezdivka
);

--Kteří hráči byli pány jeskyně v průběhu dobrodružství "Porazit Sarumana" a kdy?
SELECT H.jmeno, H.prijmeni, S.kdy
FROM hraci H, sezeni S
WHERE H.prezdivka IN (
	SELECT S.pan_jeskyne
	FROM dobrodruzstvi D
	WHERE D.cil = 'Porazit Sarumana' AND S.ID_dobrodruzstvi = D.ID_dobrodruzstvi
) ORDER BY S.kdy;

--------------------------------------------------
--             EXPLAIN PLAN & INDEX
--------------------------------------------------
DROP INDEX index_uroven;

EXPLAIN PLAN FOR
SELECT hraci.prezdivka, MAX(postavy.uroven) "Max uroven postavy"
FROM hraci 
INNER JOIN postavy
ON hraci.prezdivka = postavy.vlastnik
GROUP BY hraci.prezdivka
ORDER BY hraci.prezdivka;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX index_uroven ON postavy(uroven);

EXPLAIN PLAN FOR
SELECT /*+ INDEX(postavy index_uroven)*/ hraci.prezdivka, MAX(postavy.uroven) "Max uroven postavy"
FROM hraci 
INNER JOIN postavy
ON hraci.prezdivka = postavy.vlastnik
GROUP BY hraci.prezdivka
ORDER BY hraci.prezdivka;
SELECT * FROM TABLE(DBMS_XPLAN.display);

--------------------------------------------------
--             MATERIALIZED VIEW
--------------------------------------------------

DROP MATERIALIZED VIEW LOG ON hraci;

DROP MATERIALIZED VIEW materialized_hraci;

CREATE MATERIALIZED VIEW LOG ON hraci WITH PRIMARY KEY, ROWID(admin) INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW materialized_hraci
CACHE
BUILD IMMEDIATE
REFRESH FAST ON COMMIT
ENABLE QUERY REWRITE
AS SELECT H.admin, COUNT(H.admin) AS "Admin Count"
FROM hraci H
GROUP BY H.admin;

GRANT ALL ON materialized_hraci TO xhudja00;

SELECT * FROM materialized_hraci;

INSERT INTO hraci (prezdivka, jmeno, prijmeni, heslo, email, datum_narozeni, admin)
VALUES ('NEMESIS', 'Johny', 'Black', '1234', 'hello@iknowwhereyou.live', to_date('1990-01-01', 'YYYY-MM-DD'), 'N');

COMMIT;
SELECT * FROM materialized_hraci;

--------------------------------------------------
--                   ukázky
--------------------------------------------------
-- trigger trigger_participant
	-- mrtvá postava
INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Dark Knight', 5);

	-- už se zúčastňuje
INSERT INTO ucastnici
VALUES (SQ_ucastnici.nextval, 'Laura Icecrown', 5);

-- trigger trigger_generate
INSERT INTO vybaveni (nazev, druh, typ, kvalita, stat1, stat2)
VALUES ('Diamantový štít', 'S', 'stit', 1, 'DEF', 50);

-- procedura player_stats
	-- hráč, co existuje
EXECUTE player_stats('xfryzj01');

	-- hráč, co neexistuje
EXECUTE player_stats('unknown');

-- procedura upcoming_sessions
EXECUTE upcoming_sessions;

