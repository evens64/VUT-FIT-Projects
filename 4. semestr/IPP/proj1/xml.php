<?php

/**
 * Funkce na převod formátu JSON uloženého v poli '$jsn' do formátu XML za podmínek určených argumenty zpracovaných v poli '$config'
 * $int - pomocný integer, zda-li je na vstupu prázdné indexované pole '[]'
 * vrátí zpracované XML jako DOMDocument
 */
function JSON2XML($int) {
	global $config, $jsn, $xml;

	$xml = new DOMDocument('1.0', 'UTF-8');
	$xml->preserveWhiteSpace = false;
	$xml->formatOutput = true;

	if (array_key_exists('r', $config)) {						// argument '-r=root-element'
		$root = $xml->createElement($config['r']);
		(simple_array_test($jsn) === true || $int == 1) ? process_array($root, $jsn) : process_main($root, $jsn);
		$xml->appendchild($root);
	} else {
		(simple_array_test($jsn) === true || $int == 1) ? process_array($xml, $jsn) : process_main($xml, $jsn);
	}

	return $xml;
}


/**
 * zpracuje asociativní pole (objekt {} v JSON) a vloží ho do DOMDocument
 * $node - místo, kam má funkce následně vložit zpracované pole do DOMDocument
 * $array - pole pro zpracování
 */
function process_main($node, array $array) {
	global $xml;

	foreach($array as $key => $value) {

		if (is_array($value)) {				// array
			$element = $xml->createElement(validate_name($key, 1));
			(simple_array_test($value) === true) ? process_array($element, $value) : process_main($element, $value);
			$node->appendchild($element);
		} else {
			process_other($node, validate_name($key, 1), $value);
		}

	}

}

/**
 * zpracuje indexované pole ([]]) a vložího do DOMDocument
 * $node - místo, kam má funkce následně vložit zpracované pole do DOMDocument
 * $array - pole pro zpracování
 */
function process_array($node, array $array) {
	global $config, $jsn, $xml;

	$index = (array_key_exists('start', $config) === true) ? $config['start'] : 1;

	$pole = $xml->createElement($config['array-name']);
	if (array_key_exists('array-size', $config)) {
		$pole->setAttribute('size', count($array));
	}

	foreach ($array as $key => $value) {

		$item = $xml->createElement($config['item-name']);
		if (array_key_exists('index-items', $config)) {
			$item->setAttribute('index', $index);
		}

		if (is_array($value)) {
			(simple_array_test($value) === true) ? process_array($item, $value) : process_main($item, $value);
		} else {

			if (is_null($value) || is_bool($value)) {		// literal

				$literal = (is_bool($value) === true) ? ($value === true) ? 'true' : 'false' : 'null';

				if (array_key_exists('l', $config)) {						// argument '-l'
					$nested_item = $xml->createElement($literal);
					$item->appendchild($nested_item);
				} else {
					$item->setAttribute('value', $literal);
				}

				if (array_key_exists('types', $config) == true) {			// argument '--types'
					$item->setAttribute('type', 'literal');
				}

			} elseif (is_string($value)) {					// string

				if (array_key_exists('s', $config)) {						// argument '-s'
					$nested_item = $xml->createTextNode($value);
					$item->appendchild($nested_item);
				} else {
					$item->setAttribute('value', $value);
				}

				if (array_key_exists('types', $config) == true) {			// argument '--types'
					$item->setAttribute('type', 'string');
				}

			} elseif (is_int($value)) {						// integer

				if (array_key_exists('i', $config)) {						// argument '-i'
					$nested_item = $xml->createTextNode($value);
					$item->appendchild($nested_item);
				} else {
					$item->setAttribute('value', $value);
				}

				if (array_key_exists('types', $config) == true) {			// argument '--types'
					$item->setAttribute('type', 'integer');
				}

			} elseif (is_real($value)) {					// real

				if (array_key_exists('i', $config)) {						// argument '-i'
					$nested_item = $xml->createTextNode(floor($value));
					$item->appendchild($nested_item);
				} else {
					$item->setAttribute('value', floor($value));
				}

				if (array_key_exists('types', $config) == true) {			// argument '--types'
					$item->setAttribute('type', 'real');
				}

			} else {
				// unknown
				exit(100);
			}

		}

		$pole->appendchild($item);
		$index++;

	}

	$node->appendchild($pole);

}

/**
 * pomocná fce pro 'process_main()', zpracuje hodnoty typu 'literal', 'string', 'integer', 'real'
 * $node - místo, kam má funkce následně vložit zpracované pole do DOMDocument
 * $key - klíč pro vložení do DOMDocument
 * $value - zpracovávaná hodnota klíče, následně vložena do DOMDOcument
 */
function process_other($node, $key, $value) {
	global $config, $xml;

	if (is_null($value) || is_bool($value)) {				// literal

		$literal = (is_bool($value) === true) ? ($value === true) ? 'true' : 'false' : 'null';

		$element = $xml->createElement($key);
		if (array_key_exists('l', $config)) {						// argument '-l'
			$nested_element = $xml->createElement($literal);
			$element->appendchild($nested_element);
		} else {
			$element->setAttribute('value', $literal);
		}

		if (array_key_exists('types', $config) == true) {			// argument '--types'
			$element->setAttribute('type', 'literal');
		}

		$node->appendchild($element);

	} elseif (is_string($value)) {							// string

		if (array_key_exists('s', $config)) {						// argument '-s'
			$element = $xml->createElement($key, $value);
		} else {
			$element = $xml->createElement($key);
			$element->setAttribute('value', $value);
		}

		if (array_key_exists('types', $config) == true) {			// argument '--types'
			$element->setAttribute('type', 'string');
		}

		$node->appendchild($element);

	} elseif (is_int($value)) {								// integer

		if (array_key_exists('i', $config)) {						// argument '-i'
			$element = $xml->createElement($key, $value);
		} else {
			$element = $xml->createElement($key);
			$element->setAttribute('value', $value);
		}

		if (array_key_exists('types', $config) == true) {			// argument '--types'
			$element->setAttribute('type', 'integer');
		}

		$node->appendchild($element);

	} elseif (is_real($value)) {							// real

		if (array_key_exists('i', $config)) {						// argument '-i'
			$element = $xml->createElement($key, floor($value));
		} else {
			$element = $xml->createElement($key);
			$element->setAttribute('value', floor($value));
		}

		if (array_key_exists('types', $config) == true) {			// argument '--types'
			$element->setAttribute('type', 'real');
		}

		$node->appendchild($element);

	} else {												// unknown
		exit(100);
	}
}

?>
