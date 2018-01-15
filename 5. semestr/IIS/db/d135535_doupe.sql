-- phpMyAdmin SQL Dump
-- version 3.5.8.2
-- http://www.phpmyadmin.net
--
-- Počítač: wm123.wedos.net:3306
-- Vygenerováno: Pon 04. pro 2017, 21:09
-- Verze serveru: 10.0.21-MariaDB
-- Verze PHP: 5.4.23

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Databáze: `d135535_doupe`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `hraci`
--

CREATE TABLE IF NOT EXISTS `hraci` (
  `ID_hrace` int(11) NOT NULL AUTO_INCREMENT,
  `uziv_jmeno` varchar(25) COLLATE utf8_czech_ci NOT NULL COMMENT 'uživatelské jméno (použije se pro přihlášení, nesmí obsahovat mezery)',
  `heslo` varchar(25) COLLATE utf8_czech_ci NOT NULL COMMENT 'heslo (nesmí obsahovat mezery, atd.)',
  `email` varchar(50) COLLATE utf8_czech_ci NOT NULL COMMENT 'email',
  `prezdivka` varchar(50) COLLATE utf8_czech_ci NOT NULL COMMENT 'přezdívka (použije se na webu)',
  `krestni_jmeno` varchar(25) COLLATE utf8_czech_ci DEFAULT NULL COMMENT 'křestní jméno',
  `prijmeni` varchar(25) COLLATE utf8_czech_ci DEFAULT NULL COMMENT 'příjmení',
  `pohlavi` varchar(1) COLLATE utf8_czech_ci NOT NULL COMMENT 'M/F',
  `datum_narozeni` date NOT NULL COMMENT 'datum narození (pouze hráči 15+)',
  `pan_jeskyne` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0=není PJ, cokoliv jiného je PJ',
  `admin` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0=není Admin, cokoliv jiného je Admin',
  PRIMARY KEY (`ID_hrace`),
  UNIQUE KEY `prezdivka` (`prezdivka`),
  UNIQUE KEY `email` (`email`),
  UNIQUE KEY `uziv_jmeno` (`uziv_jmeno`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=367 ;

--
-- Vypisuji data pro tabulku `hraci`
--

INSERT INTO `hraci` (`ID_hrace`, `uziv_jmeno`, `heslo`, `email`, `prezdivka`, `krestni_jmeno`, `prijmeni`, `pohlavi`, `datum_narozeni`, `pan_jeskyne`, `admin`) VALUES
(1, 'admin', 'admin', 'admin@dracidoupe.cz', 'admin', '', '', 'M', '1990-01-01', 1, 1),
(364, 'gamer', 'gamer', 'gamer@dracidoupe.cz', 'PC MASTER RACE', 'Damian', 'Wayne', 'M', '2000-12-24', 0, 0),
(365, 'dmaster', 'dmaster', 'dmaster@dracidoupe.cz', 'Dungeon Master', 'Dungeon', 'Master', 'M', '1990-01-01', 1, 0),
(366, 'fila01', '123456', 'alindor01@seznam.cz', 'Fila', 'Filip', 'D', 'M', '1996-01-17', 0, 0);

--
-- Spouště `hraci`
--
DROP TRIGGER IF EXISTS `before_hraci_insert`;
DELIMITER //
CREATE TRIGGER `before_hraci_insert` BEFORE INSERT ON `hraci`
 FOR EACH ROW BEGIN
    -- kontrola jmena
	IF length(NEW.uziv_jmeno) - length(replace(NEW.uziv_jmeno, ' ', '')) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Uživatelské jméno nesmí obsahovat mezery.', MYSQL_ERRNO = 0101;
    END IF;
	
    IF length(NEW.uziv_jmeno) < 5 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Uživatelské jméno musí mít alespoň 5 znaků.', MYSQL_ERRNO = 0102;
    END IF;
	
	-- kontrola hesla
    IF length(NEW.heslo) - length(replace(NEW.heslo, ' ', '')) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Heslo nesmí obsahovat mezery.', MYSQL_ERRNO = 0103;
    END IF;
	
    IF length(NEW.heslo) < 5 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Heslo musí mít alespoň 5 znaků.', MYSQL_ERRNO = 0104;
    END IF;
	
-- email format
    	IF NOT (NEW.email REGEXP '^[a-zA-Z0-9][a-zA-Z0-9._-]*@[a-zA-Z0-9][a-zA-Z0-9._-]*.[a-zA-Z]{2,4}$') THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Neplatná emailová adresa.', MYSQL_ERRNO = 1005;
        END IF;
	
	-- age > 15
	IF TIMESTAMPDIFF(YEAR, NEW.datum_narozeni, CURDATE()) < 15 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Věk musí být větší než 15.', MYSQL_ERRNO = 0106;
    END IF;
	
	IF NOT (NEW.admin = 0 OR NEW.admin = 1) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "admin" může obsahovat jen hodnoty 0/1.', MYSQL_ERRNO = 0107;
    END IF;
	
    IF NOT (NEW.pan_jeskyne = 0 OR NEW.pan_jeskyne = 1) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "kvalita" může obsahovat jen hodnoty 1-5.', MYSQL_ERRNO = 0108;
    END IF;
	
	-- pokud neni zadana prezdivka pouzije se uziv_jmeno
	IF NOT length(NEW.prezdivka) THEN
        SET NEW.prezdivka = NEW.uziv_jmeno;
    END IF;
    
END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_hraci_update`;
DELIMITER //
CREATE TRIGGER `before_hraci_update` BEFORE UPDATE ON `hraci`
 FOR EACH ROW BEGIN
    -- kontrola jmena
	IF length(NEW.uziv_jmeno) - length(replace(NEW.uziv_jmeno, ' ', '')) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Uživatelské jméno nesmí obsahovat mezery.', MYSQL_ERRNO = 0101;
    END IF;
	
    IF length(NEW.uziv_jmeno) < 5 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Uživatelské jméno musí mít alespoň 5 znaků.', MYSQL_ERRNO = 0102;
    END IF;
	
	-- kontrola hesla
    IF length(NEW.heslo) - length(replace(NEW.heslo, ' ', '')) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Heslo nesmí obsahovat mezery.', MYSQL_ERRNO = 0103;
    END IF;
	
    IF length(NEW.heslo) < 5 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Heslo musí mít alespoň 5 znaků.', MYSQL_ERRNO = 0104;
    END IF;
	
-- email format
    	IF NOT (NEW.email REGEXP '^[a-zA-Z0-9][a-zA-Z0-9._-]*@[a-zA-Z0-9][a-zA-Z0-9._-]*\.[a-zA-Z]{2,4}$') THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Neplatná emailová adresa.', MYSQL_ERRNO = 1005;
        END IF;
	
	-- age > 15
	IF TIMESTAMPDIFF(YEAR, NEW.datum_narozeni, CURDATE()) < 15 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Věk musí být větší než 15.', MYSQL_ERRNO = 0106;
    END IF;
	
	IF NOT (NEW.admin = 0 OR NEW.admin = 1) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "admin" může obsahovat jen hodnoty 0/1.', MYSQL_ERRNO = 0107;
    END IF;
	
    IF NOT (NEW.pan_jeskyne = 0 OR NEW.pan_jeskyne = 1) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "kvalita" může obsahovat jen hodnoty 1-5.', MYSQL_ERRNO = 0108;
    END IF;
	
	-- pokud neni zadana prezdivka pouzije se uziv_jmeno
	IF NOT length(NEW.prezdivka) THEN
        SET NEW.prezdivka = NEW.uziv_jmeno;
    END IF;
    
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `hra_dobrodruzstvi`
--

CREATE TABLE IF NOT EXISTS `hra_dobrodruzstvi` (
  `ID_dobrodruzstvi` int(11) NOT NULL AUTO_INCREMENT,
  `ID_hrace` int(11) NOT NULL COMMENT 'autor',
  `ID_tazeni` int(11) DEFAULT NULL COMMENT 'nadřazené tažení',
  `obtiznost` int(11) NOT NULL COMMENT '1-5',
  `cil` varchar(50) COLLATE utf8_czech_ci NOT NULL COMMENT 'jeden quest',
  `lokalita` varchar(25) COLLATE utf8_czech_ci NOT NULL COMMENT 'část mapy (např. Ronan)',
  `datum_vytvoreni` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `priznak` int(1) NOT NULL DEFAULT '0' COMMENT '0 = neukončené; 1 = ukončené',
  PRIMARY KEY (`ID_dobrodruzstvi`),
  KEY `ID_tazeni` (`ID_tazeni`),
  KEY `ID_hrace` (`ID_hrace`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=64 ;

--
-- Vypisuji data pro tabulku `hra_dobrodruzstvi`
--

INSERT INTO `hra_dobrodruzstvi` (`ID_dobrodruzstvi`, `ID_hrace`, `ID_tazeni`, `obtiznost`, `cil`, `lokalita`, `datum_vytvoreni`, `priznak`) VALUES
(62, 1, 8, 1, 'Zajmout agenta Hydry', 'Boston', '2017-12-04 17:01:18', 0),
(63, 1, NULL, 4, 'Zabít vládce temného lesa', 'Kalimdor', '2017-12-04 17:03:52', 0);

--
-- Spouště `hra_dobrodruzstvi`
--
DROP TRIGGER IF EXISTS `before_hra_dobrodruzstvi_insert`;
DELIMITER //
CREATE TRIGGER `before_hra_dobrodruzstvi_insert` BEFORE INSERT ON `hra_dobrodruzstvi`
 FOR EACH ROW BEGIN

    IF NOT (NEW.obtiznost >= 1 AND NEW.obtiznost <= 5) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "obtiznost" může mít hodnoty 1-5.', MYSQL_ERRNO = 0101;
	END IF;

END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_hra_dobrodruzstvi_update`;
DELIMITER //
CREATE TRIGGER `before_hra_dobrodruzstvi_update` BEFORE UPDATE ON `hra_dobrodruzstvi`
 FOR EACH ROW BEGIN

    IF NOT (NEW.obtiznost >= 1 AND NEW.obtiznost <= 5) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '"obtiznost" value can only be 1-5.', MYSQL_ERRNO = 0101;
	END IF;

END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `hra_sezeni`
--

CREATE TABLE IF NOT EXISTS `hra_sezeni` (
  `ID_sezeni` int(11) NOT NULL AUTO_INCREMENT,
  `ID_hrace` int(11) NOT NULL COMMENT 'pán jeskyně',
  `kdy` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'kdy se bude sezení konat',
  `kde` varchar(50) COLLATE utf8_czech_ci NOT NULL COMMENT 'kde se bude sezení konat',
  `ID_dobrodruzstvi` int(11) NOT NULL COMMENT 'nadřazené dobrodružství',
  `datum_vytvoreni` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `priznak` int(1) NOT NULL DEFAULT '0' COMMENT '0 = neukončené; 1 = ukončené; 2 = spuštěné',
  PRIMARY KEY (`ID_sezeni`),
  KEY `ID_dobrodruzstvi` (`ID_dobrodruzstvi`),
  KEY `ID_hrace` (`ID_hrace`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=16 ;

--
-- Vypisuji data pro tabulku `hra_sezeni`
--

INSERT INTO `hra_sezeni` (`ID_sezeni`, `ID_hrace`, `kdy`, `kde`, `ID_dobrodruzstvi`, `datum_vytvoreni`, `priznak`) VALUES
(13, 1, '2017-12-09 17:30:00', 'Chajovna, Brno', 62, '2017-12-04 17:03:50', 2),
(14, 1, '2018-12-20 11:00:00', 'u mne doma', 63, '2017-12-04 19:46:07', 0),
(15, 1, '2017-12-05 15:00:00', 'Na zahradě', 62, '2017-12-04 19:49:24', 0);

--
-- Spouště `hra_sezeni`
--
DROP TRIGGER IF EXISTS `before_hra_sezeni_insert`;
DELIMITER //
CREATE TRIGGER `before_hra_sezeni_insert` BEFORE INSERT ON `hra_sezeni`
 FOR EACH ROW BEGIN

    IF NEW.kdy < NOW() THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "kdy" smí obsahovat datum jen v budoucnu.', MYSQL_ERRNO = 0301;
	END IF;

END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_hra_sezeni_update`;
DELIMITER //
CREATE TRIGGER `before_hra_sezeni_update` BEFORE UPDATE ON `hra_sezeni`
 FOR EACH ROW BEGIN

    IF NEW.kdy < NOW() THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '"kdy" can only be in the future.', MYSQL_ERRNO = 0301;
	END IF;

END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `hra_tazeni`
--

CREATE TABLE IF NOT EXISTS `hra_tazeni` (
  `ID_tazeni` int(11) NOT NULL AUTO_INCREMENT,
  `ID_hrace` int(11) NOT NULL COMMENT 'autor',
  `cil` varchar(50) COLLATE utf8_czech_ci NOT NULL COMMENT 'questline',
  `mapa` varchar(100) COLLATE utf8_czech_ci NOT NULL COMMENT 'prostředí, kde se tažení koná (Středozem, ...)',
  `datum_vytvoreni` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `priznak` int(1) NOT NULL DEFAULT '0' COMMENT '0 = neukončené; 1 = ukončené',
  PRIMARY KEY (`ID_tazeni`),
  KEY `ID_hrace` (`ID_hrace`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=9 ;

--
-- Vypisuji data pro tabulku `hra_tazeni`
--

INSERT INTO `hra_tazeni` (`ID_tazeni`, `ID_hrace`, `cil`, `mapa`, `datum_vytvoreni`, `priznak`) VALUES
(8, 1, 'Porazit Hydru', 'Alternativní svět, kde vládne Hydra', '2017-12-04 17:00:33', 0);

-- --------------------------------------------------------

--
-- Struktura tabulky `mn_inventare`
--

CREATE TABLE IF NOT EXISTS `mn_inventare` (
  `ID_postavy` int(11) NOT NULL COMMENT 'kdo?',
  `ID_predmetu` int(11) NOT NULL COMMENT 'co?',
  `ID_sezeni` int(11) NOT NULL COMMENT 'kde?, kdy?',
  `pocet` int(11) NOT NULL DEFAULT '1' COMMENT 'kolik?',
  PRIMARY KEY (`ID_postavy`,`ID_predmetu`,`ID_sezeni`),
  KEY `ID_predmetu` (`ID_predmetu`),
  KEY `ID_sezeni` (`ID_sezeni`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

--
-- Vypisuji data pro tabulku `mn_inventare`
--

INSERT INTO `mn_inventare` (`ID_postavy`, `ID_predmetu`, `ID_sezeni`, `pocet`) VALUES
(63, 356, 13, 5),
(63, 360, 13, 2),
(63, 366, 13, 1);

--
-- Spouště `mn_inventare`
--
DROP TRIGGER IF EXISTS `before_mn_inventare_insert`;
DELIMITER //
CREATE TRIGGER `before_mn_inventare_insert` BEFORE INSERT ON `mn_inventare`
 FOR EACH ROW BEGIN

    IF NEW.pocet <= 0 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Pole "pocet" musí být větší než 0.', MYSQL_ERRNO = 0501;
	END IF;

END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_mn_inventare_update`;
DELIMITER //
CREATE TRIGGER `before_mn_inventare_update` BEFORE UPDATE ON `mn_inventare`
 FOR EACH ROW BEGIN

    IF NEW.pocet <= 0 THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '"pocet" must be greater than 0.', MYSQL_ERRNO = 0501;
	END IF;

END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `mn_ucastnici`
--

CREATE TABLE IF NOT EXISTS `mn_ucastnici` (
  `ID_postavy` int(11) NOT NULL COMMENT 'kdo se zúčastnil',
  `ID_sezeni` int(11) NOT NULL COMMENT 'čeho se zúčastnil',
  PRIMARY KEY (`ID_postavy`,`ID_sezeni`),
  KEY `ID_sezeni` (`ID_sezeni`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

--
-- Vypisuji data pro tabulku `mn_ucastnici`
--

INSERT INTO `mn_ucastnici` (`ID_postavy`, `ID_sezeni`) VALUES
(63, 13),
(65, 13);

--
-- Spouště `mn_ucastnici`
--
DROP TRIGGER IF EXISTS `before_mn_ucastnici_insert`;
DELIMITER //
CREATE TRIGGER `before_mn_ucastnici_insert` BEFORE INSERT ON `mn_ucastnici`
 FOR EACH ROW BEGIN

    IF (SELECT COUNT(*)
	FROM mn_umrti U
	WHERE U.ID_postavy = NEW.ID_postavy AND U.ID_sezeni = NEW.ID_sezeni)
	THEN
	    CALL raise_application_error(0601, 'Nelze vložit mrtvou postavu.');
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Nelze vložit mrtvou postavu.', MYSQL_ERRNO = 0601;
	END IF;
	
	IF (SELECT COUNT(*)
	FROM mn_ucastnici C
	WHERE C.ID_postavy = NEW.ID_postavy AND C.ID_sezeni = NEW.ID_sezeni)
	THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Postava už je zaregistrována.', MYSQL_ERRNO = 0602;
	END IF;

END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_mn_ucastnici_update`;
DELIMITER //
CREATE TRIGGER `before_mn_ucastnici_update` BEFORE UPDATE ON `mn_ucastnici`
 FOR EACH ROW BEGIN

    IF (SELECT COUNT(*)
	FROM mn_umrti U
	WHERE U.ID_postavy = NEW.ID_postavy AND U.ID_sezeni = NEW.ID_sezeni)
	THEN
	    CALL raise_application_error(0601, 'Cant insert dead character.');
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Cant insert dead character.', MYSQL_ERRNO = 0601;
	END IF;
	
	IF (SELECT COUNT(*)
	FROM mn_ucastnici C
	WHERE C.ID_postavy = NEW.ID_postavy AND C.ID_sezeni = NEW.ID_sezeni)
	THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Character is already registered.', MYSQL_ERRNO = 0602;
	END IF;

END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `mn_umrti`
--

CREATE TABLE IF NOT EXISTS `mn_umrti` (
  `ID_postavy` int(11) NOT NULL COMMENT 'kdo umřel',
  `ID_sezeni` int(11) NOT NULL COMMENT 'kdy/kde umřel',
  PRIMARY KEY (`ID_postavy`,`ID_sezeni`),
  UNIQUE KEY `postava` (`ID_postavy`),
  KEY `ID_sezeni` (`ID_sezeni`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

--
-- Vypisuji data pro tabulku `mn_umrti`
--

INSERT INTO `mn_umrti` (`ID_postavy`, `ID_sezeni`) VALUES
(66, 14);

-- --------------------------------------------------------

--
-- Struktura tabulky `postavy`
--

CREATE TABLE IF NOT EXISTS `postavy` (
  `ID_postavy` int(11) NOT NULL AUTO_INCREMENT,
  `ID_hrace` int(11) NOT NULL COMMENT 'vlastnik',
  `jmeno` varchar(50) COLLATE utf8_czech_ci NOT NULL COMMENT 'jméno postavy',
  `pohlavi` char(1) COLLATE utf8_czech_ci NOT NULL COMMENT 'pohlavi (M/F)',
  `rasa` varchar(25) COLLATE utf8_czech_ci NOT NULL COMMENT 'rasa (specifikuje se)',
  `povolani` varchar(25) COLLATE utf8_czech_ci NOT NULL COMMENT 'povolaní, classa (specifikuje se)',
  `stat_uroven` int(11) NOT NULL DEFAULT '1' COMMENT 'úroveň postavy',
  `stat_XP` int(11) NOT NULL DEFAULT '0' COMMENT 'zkušenosti (max: uroven*1000)',
  `stat_HP` int(11) NOT NULL DEFAULT '100' COMMENT 'životy (max: uroven*100)',
  `stat_MN` int(11) NOT NULL DEFAULT '100' COMMENT 'mana (max: uroven*100)',
  `stat_SP` int(11) NOT NULL DEFAULT '100' COMMENT 'výdrž (max: uroven*100)',
  PRIMARY KEY (`ID_postavy`),
  UNIQUE KEY `jmeno` (`jmeno`),
  KEY `ID_hrace` (`ID_hrace`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=67 ;

--
-- Vypisuji data pro tabulku `postavy`
--

INSERT INTO `postavy` (`ID_postavy`, `ID_hrace`, `jmeno`, `pohlavi`, `rasa`, `povolani`, `stat_uroven`, `stat_XP`, `stat_HP`, `stat_MN`, `stat_SP`) VALUES
(63, 1, 'Ariel', 'F', 'Elf', 'Kouzelník', 3, 2400, 150, 300, 300),
(64, 1, 'Persil', 'M', 'Temný elf', 'Zloděj', 1, 0, 100, 100, 100),
(65, 365, 'Aranei', 'Ž', 'Elf', 'Lučištník', 1, 0, 100, 100, 100),
(66, 1, '@@@22', 'Ž', 'Ork', 'Lučištník', 1, 0, 0, 76, 100);

--
-- Spouště `postavy`
--
DROP TRIGGER IF EXISTS `before_postavy_update`;
DELIMITER //
CREATE TRIGGER `before_postavy_update` BEFORE UPDATE ON `postavy`
 FOR EACH ROW BEGIN
	
    DECLARE uroven int(11);
	SET uroven = OLD.stat_uroven;
	
    IF NEW.stat_XP <> OLD.stat_XP THEN
	    IF NEW.stat_uroven = OLD.stat_uroven THEN
			WHILE NEW.stat_XP >= (uroven * 1000) DO
				SET NEW.stat_XP = NEW.stat_XP - (uroven * 1000);
				SET uroven = uroven + 1;
			END WHILE;
			SET NEW.stat_uroven = uroven;
			IF NEW.stat_HP = OLD.stat_HP THEN
		        SET NEW.stat_HP = NEW.stat_uroven * 100;
	        END IF;
			IF NEW.stat_MN = OLD.stat_MN THEN
				SET NEW.stat_MN = NEW.stat_uroven * 100;
			END IF;
			IF NEW.stat_SP = OLD.stat_SP THEN
				SET NEW.stat_SP = NEW.stat_uroven * 100;
			END IF;
		END IF;
	END IF;
		
	IF NEW.stat_HP > (NEW.stat_uroven * 100) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HP nesmí být větší než level*100.', MYSQL_ERRNO = 0801;
	END IF;
	IF NEW.stat_MN > (NEW.stat_uroven * 100) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'MN nesmí být větší než level*100.', MYSQL_ERRNO = 0802;
	END IF;
	IF NEW.stat_SP > (NEW.stat_uroven * 100) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'SP nesmí být větší než level*100.', MYSQL_ERRNO = 0803;
	END IF;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `predmety`
--

CREATE TABLE IF NOT EXISTS `predmety` (
  `ID_predmetu` int(11) NOT NULL AUTO_INCREMENT,
  `nazev` varchar(50) COLLATE utf8_czech_ci NOT NULL,
  `popis` varchar(200) COLLATE utf8_czech_ci DEFAULT NULL COMMENT 'možný popis (vylepšení, enchanty, atd.)',
  `kvalita` int(11) NOT NULL DEFAULT '0' COMMENT '(pouze pro zbroje a zbrane, jinak 0) znamka jak ve skole (1-5)',
  `priznak` char(1) COLLATE utf8_czech_ci DEFAULT NULL COMMENT 'C=consumable; A=armor; W=weapon',
  PRIMARY KEY (`ID_predmetu`),
  UNIQUE KEY `nazev` (`nazev`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=389 ;

--
-- Vypisuji data pro tabulku `predmety`
--

INSERT INTO `predmety` (`ID_predmetu`, `nazev`, `popis`, `kvalita`, `priznak`) VALUES
(356, 'Elixir života (+50HP)', NULL, 0, 'C'),
(357, 'Elixir many (+50MN)', NULL, 0, 'C'),
(358, 'Elixir výdrže (+50SP)', NULL, 0, 'C'),
(359, 'Elixir života (+25HP)', NULL, 0, 'C'),
(360, 'Elixir many (+25MN)', NULL, 0, 'C'),
(361, 'Elixir výdrže (+25SP)', NULL, 0, 'C'),
(362, 'Elixir života (+100HP)', NULL, 0, 'C'),
(363, 'Elixir many (+100MN)', NULL, 0, 'C'),
(364, 'Elixir výdrže (+100SP)', NULL, 0, 'C'),
(365, 'Železná helma (+1DEF)', NULL, 5, 'A'),
(366, 'Železná zbroj (+1DEF)', NULL, 5, 'A'),
(367, 'Železné rukavice (+1DEF)', NULL, 5, 'A'),
(368, 'Železné boty (+1DEF)', NULL, 5, 'A'),
(369, 'Ocelová helma (+2DEF)', NULL, 4, 'A'),
(370, 'Ocelová zbroj (+2DEF)', NULL, 4, 'A'),
(371, 'Ocelové rukavice (+2DEF)', NULL, 4, 'A'),
(372, 'Ocelové boty (+2DEF)', NULL, 4, 'A'),
(373, 'Železná sekyra (+1DMG)', NULL, 5, 'W'),
(374, 'Železný meč (+1DMG)', NULL, 5, 'W'),
(375, 'Železné kladivo (+1DMG)', NULL, 5, 'W'),
(376, 'Ocelová sekyra (+2DMG)', NULL, 4, 'W'),
(377, 'Ocelová meč (+2DMG)', NULL, 4, 'W'),
(378, 'Ocelové kladivo (+2DMG)', NULL, 4, 'W'),
(379, 'Klíč od domu', 'Klič k domu postavy', 0, NULL),
(380, 'Vlaštovičník', 'Zázračná bylina použiváná k výrobě elixírů života', 0, NULL),
(381, 'Kámen', 'Náhodně sebraný kámen', 0, '0'),
(382, 'Talíř', 'Talíř s ornamenty', 0, NULL);

--
-- Spouště `predmety`
--
DROP TRIGGER IF EXISTS `before_predmety_insert`;
DELIMITER //
CREATE TRIGGER `before_predmety_insert` BEFORE INSERT ON `predmety`
 FOR EACH ROW BEGIN

	IF (NEW.priznak = 'A' OR NEW.priznak = 'W') THEN
        	IF NOT (NEW.kvalita >= 1 AND NEW.kvalita <= 5) THEN
                	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'predmety s priznakem "W" a "A" musi mit nastavenou kvalitu na 1-5.', MYSQL_ERRNO = 0901;
                END IF;
        ELSE
        	IF NOT (NEW.kvalita = 0) THEN
                	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'predmety s jinym priznakem nez "W" a "A" musi mit nastavenou kvalitu na 0.', MYSQL_ERRNO = 0903;
                END IF;
        END IF;

	IF NOT (NEW.priznak = 'C' OR NEW.priznak = 'W' OR NEW.priznak = 'A' OR NEW.priznak = '') THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'hodnota "priznak" muze byt poze "C", "W", "A" nebo NULL.', MYSQL_ERRNO = 0902;
	END IF;
	
END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_predmety_update`;
DELIMITER //
CREATE TRIGGER `before_predmety_update` BEFORE UPDATE ON `predmety`
 FOR EACH ROW BEGIN

    IF NOT (NEW.kvalita >= 1 AND NEW.kvalita <= 5) THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '"kvalita" value can only be 1-5.', MYSQL_ERRNO = 0901;
	END IF;

	IF NOT (NEW.priznak = 'C' OR NEW.priznak = 'W' OR NEW.priznak = 'A' OR NEW.priznak = '') THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '"priznak" value can only be "C", "W", "A" or NULL.', MYSQL_ERRNO = 0902;
	END IF;
	
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Struktura tabulky `zadosti`
--

CREATE TABLE IF NOT EXISTS `zadosti` (
  `ID_zadosti` int(11) NOT NULL AUTO_INCREMENT,
  `ID_hrace` int(11) NOT NULL,
  `datum_vytvoreni` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `priznak` int(11) NOT NULL DEFAULT '0' COMMENT '0 = vytvořena; 1 = povoleno; 2 = zakázáno',
  PRIMARY KEY (`ID_zadosti`),
  KEY `ID_hrace` (`ID_hrace`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci COMMENT='žádosti povýšení na Pána jeskyně' AUTO_INCREMENT=11 ;

--
-- Vypisuji data pro tabulku `zadosti`
--

INSERT INTO `zadosti` (`ID_zadosti`, `ID_hrace`, `datum_vytvoreni`, `priznak`) VALUES
(8, 1, '2017-12-04 12:02:18', 1),
(9, 366, '2017-12-04 17:31:50', 0),
(10, 365, '2017-12-04 18:30:47', 1);

--
-- Spouště `zadosti`
--
DROP TRIGGER IF EXISTS `before_zadosti_insert`;
DELIMITER //
CREATE TRIGGER `before_zadosti_insert` BEFORE INSERT ON `zadosti`
 FOR EACH ROW BEGIN
	
	IF (SELECT COUNT(*)
	FROM hraci H
	WHERE H.ID_hrace = NEW.ID_hrace AND H.pan_jeskyne)
	THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Hráč už je pán jeskyně.', MYSQL_ERRNO = 1001;
	END IF;
	
	IF (SELECT COUNT(*)
	FROM zadosti Z
	WHERE Z.ID_hrace = NEW.ID_hrace AND TIMESTAMPDIFF(MONTH, Z.datum_vytvoreni, CURDATE()) < 2)
	THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Hráč už žádal o post pána jeskyně před méně než dvěma měsíci.', MYSQL_ERRNO = 1002;
	END IF;

END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `before_zadosti_update`;
DELIMITER //
CREATE TRIGGER `before_zadosti_update` BEFORE UPDATE ON `zadosti`
 FOR EACH ROW BEGIN
	
	IF (SELECT COUNT(*)
	FROM hraci H
	WHERE H.ID_hrace = NEW.ID_hrace AND H.pan_jeskyne)
	THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Player is already dungeon master.', MYSQL_ERRNO = 1001;
	END IF;
	
	IF (SELECT COUNT(*)
	FROM zadosti Z
	WHERE Z.ID_hrace = NEW.ID_hrace AND TIMESTAMPDIFF(MONTH, Z.datum_vytvoreni, CURDATE()) < 2)
	THEN
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Player has an active request less than 2 months old.', MYSQL_ERRNO = 1002;
	END IF;

END
//
DELIMITER ;

--
-- Omezení pro exportované tabulky
--

--
-- Omezení pro tabulku `hra_dobrodruzstvi`
--
ALTER TABLE `hra_dobrodruzstvi`
  ADD CONSTRAINT `hra_dobrodruzstvi_ibfk_1` FOREIGN KEY (`ID_hrace`) REFERENCES `hraci` (`ID_hrace`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `hra_dobrodruzstvi_ibfk_2` FOREIGN KEY (`ID_tazeni`) REFERENCES `hra_tazeni` (`ID_tazeni`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `hra_sezeni`
--
ALTER TABLE `hra_sezeni`
  ADD CONSTRAINT `hra_sezeni_ibfk_1` FOREIGN KEY (`ID_hrace`) REFERENCES `hraci` (`ID_hrace`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `hra_sezeni_ibfk_2` FOREIGN KEY (`ID_dobrodruzstvi`) REFERENCES `hra_dobrodruzstvi` (`ID_dobrodruzstvi`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `hra_tazeni`
--
ALTER TABLE `hra_tazeni`
  ADD CONSTRAINT `hra_tazeni_ibfk_1` FOREIGN KEY (`ID_hrace`) REFERENCES `hraci` (`ID_hrace`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `mn_inventare`
--
ALTER TABLE `mn_inventare`
  ADD CONSTRAINT `mn_inventare_ibfk_1` FOREIGN KEY (`ID_postavy`) REFERENCES `postavy` (`ID_postavy`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `mn_inventare_ibfk_2` FOREIGN KEY (`ID_predmetu`) REFERENCES `predmety` (`ID_predmetu`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `mn_inventare_ibfk_3` FOREIGN KEY (`ID_sezeni`) REFERENCES `hra_sezeni` (`ID_sezeni`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `mn_ucastnici`
--
ALTER TABLE `mn_ucastnici`
  ADD CONSTRAINT `mn_ucastnici_ibfk_1` FOREIGN KEY (`ID_postavy`) REFERENCES `postavy` (`ID_postavy`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `mn_ucastnici_ibfk_2` FOREIGN KEY (`ID_sezeni`) REFERENCES `hra_sezeni` (`ID_sezeni`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `mn_umrti`
--
ALTER TABLE `mn_umrti`
  ADD CONSTRAINT `mn_umrti_ibfk_1` FOREIGN KEY (`ID_postavy`) REFERENCES `postavy` (`ID_postavy`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `mn_umrti_ibfk_2` FOREIGN KEY (`ID_sezeni`) REFERENCES `hra_sezeni` (`ID_sezeni`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `postavy`
--
ALTER TABLE `postavy`
  ADD CONSTRAINT `postavy_ibfk_1` FOREIGN KEY (`ID_hrace`) REFERENCES `hraci` (`ID_hrace`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `zadosti`
--
ALTER TABLE `zadosti`
  ADD CONSTRAINT `zadosti_ibfk_1` FOREIGN KEY (`ID_hrace`) REFERENCES `hraci` (`ID_hrace`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
