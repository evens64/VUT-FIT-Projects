<?php

/**
 * Fce na validaci názvu elementu
 * $value - string určený k validaci
 * $int - pomocná hodnota
 * 		1 - validace stringu zadaného argumentem, používá se při tvorbě elementu
 * 		2 - nevyužito
 * 		3 - validace stringu zadaného argumentem, používá se při zpracování argumentů '-r', '--array-name', '--item-name'
 */
function validate_name($value, $int) {
	global $config, $jsn, $xml;

	// REGEX pro validaci názvu elementu
	$pattern = '/(?i)^(?!xml)(?-i)[a-zA-Z\_\:\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}]+[a-zA-Z0-9\-\_\.\:\x{B7}\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}\x{0300}-\x{036F}\x{203F}-\x{2040}]*$/u';

	switch ($int) {
		case 1:
			if (preg_match($pattern, $value) != 1) {	//pokud je nevalidní, jde se validovat
				$new = $value;	// uložení nevalidního názvu do pomocné proměnné

				if (preg_match('/(?i)^xml(?-i).*/', $new)) {	// název začíná s xml, nahradí se 1. symbol
					$new = $config['h'];
					$new .= (mb_substr($value, 1, NULL, 'UTF-8') != false) ? mb_substr($value, 1, NULL, 'UTF-8') : '';
				}

				// negativní REGEX pouze pro nevalidní znaky
				$pattern_neg = '/^(?![a-zA-Z\_\:\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}]).|(?![a-zA-Z0-9\-\_\.\:\x{B7}\x{C0}-\x{D6}\x{D8}-\x{F6}\x{F8}-\x{2FF}\x{370}-\x{37D}\x{37F}-\x{1FFF}\x{200C}-\x{200D}\x{2070}-\x{218F}\x{2C00}-\x{2FEF}\x{3001}-\x{D7FF}\x{F900}-\x{FDCF}\x{FDF0}-\x{FFFD}\x{0300}-\x{036F}\x{203F}-\x{2040}])./u';
				// změnit všechny nevalidní znaky na znaky (nebo string) určený argumentem '-h' (implicitně '-')
				$new = preg_replace($pattern_neg, $config['h'], $new);

				// poslední kontrola validity, pokud je nový název nevalidní -> chyba
				if (preg_match($pattern, $new) != 1) exit(51);

				// název je validní, stará hodnota se přepíše novou
				$value = $new;
			}
			break;

		case 3:
			if (preg_match($pattern, $value) != 1) exit(50);
			break;

	}

	return $value;		// vrácení validní hodnoty
}

/**
 * Fce kotroluje zda-li zadané pole je indexované nebo sekvenční
 * $array - pole určené ke kontrole
 * vrací true, pokud je pole indexované, jinak false
 */
function simple_array_test(array $array) {
	$result = false;
	$index = 0;

	foreach ($array as $key => $value) {
		if ($key === $index && is_int($key)) {
			$result = true;
		}
		$value++;
	}

	return $result;
}

?>