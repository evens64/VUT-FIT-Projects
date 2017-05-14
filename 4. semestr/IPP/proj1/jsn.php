<?php
# Úloha JSN: JSON2XML v PHP 5 do IPP 2016/2017
# Jméno a příjmení: Jakub Frýz
# Login: xfryzj01

mb_internal_encoding('UTF-8');

// pokud je jedním z argumentů '--help'
if (in_array("--help", $argv)) {
	if ($argc > 2) {				// argumentů je více než má
		exit(1);					// chyba
	} else {						// zobrazení nápovědy
		include('help.php');
		print_help();
		exit(0);
	}
}

// include
include('arg.php');
include('fce.php');
include('xml.php');

# globalní proměnné
$config;		// argumenty
$jsn;			// dekódovaný JSON soubor
$xml;			// vytvořený XML soubor

// zpracování argumentů
$config = process_arguments($argv);

// načtení souboru
$import_location = (array_key_exists('input', $config) === true) ? $config['input'] : 'php://stdin';
$import_file = file_get_contents($import_location);

// zpracování JSON
$jsn = json_decode($import_file, true);	// dekódovat zadaný JSON soubor a vrátit jako pole, jinak NULL
if ($jsn == NULL) {
	if (preg_replace('/\s+/', '', $import_file) != '{}' && preg_replace('/\s+/', '', $import_file) != '[]') {		// soubor je v pořádku, ale obsahuje pouze '{}'
		exit(4);
	} else {
		if (preg_replace('/\s+/', '', $import_file) == '{}') {
			$xml = JSON2XML(0);
		}
		if (preg_replace('/\s+/', '', $import_file) == '[]') {
			$xml = JSON2XML(1);
		}
	}
} else {	// $jsn != NULL
	$xml = JSON2XML(0);
}

// výpis souboru
$export_location = (array_key_exists('output', $config) === true) ? $config['output'] : 'php://stdout';
$export_file = (array_key_exists('n', $config) === true) ? preg_replace('/^.+\n/', '', $xml->saveXML()) : $xml->saveXML();

if (array_key_exists('c', $config) === false) {		// "korekce" XML, pokud nebyl zadán argument '-c'
	$export_file = html_entity_decode($export_file, ENT_QUOTES | ENT_XML1, 'UTF-8');
}

if (file_put_contents($export_location, $export_file) === false) exit(3);

?>
