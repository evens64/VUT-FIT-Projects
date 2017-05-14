<?php

/**
 * Zpracování pole argumentů
 * $arg1 - pole pro zpracování
 * vrátí zpracované pole nebo ukončí s chybou
 */
function process_arguments($arg1) {

	$pole = array();	// pole pro argumenty
	$first = false;		// kontrola prvního argumentu (název souboru)

	foreach ($arg1 as $argument) {

		if ($first != true) {	// kontrola prvního argumentu
			$first = true;
			continue;
		}

		//echo "\t" . preg_replace('/(=.*)/', '', $argument) . " - " . preg_match('/(=.*)/', $argument) . "\n";

		switch (preg_replace('/(=.*)/', '', $argument)) {

			// Podmínky

			// array_key_exists('input', $pole)
			//		- kontrola, zda-li už argument nebyl jednou zpracován

			// preg_replace('/(.*=)/', '', $argument) == '--input'
			//		- kontrola, pokud byla zadán argument bez hodnoty

			// preg_replace('/(.*=)/', '', $argument) == ''
			//		- kontrola, pokud byl zadán argument jenom s '='

			// preg_match('/(=.*)/', $argument) != 0
			//		- kontrola, zda-li nebyla zadána hodnota za '=' u klíčů, kde se nesmí vyskytnout

			case '--input':
			# --input=filename
				if (array_key_exists('input', $pole) || preg_replace('/(.*=)/', '', $argument) == '--input' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$pole['input'] = preg_replace('/(.*=)/', '', $argument);
				}
				break;

			case '--output':
			# --output=filename
				if (array_key_exists('output', $pole) || preg_replace('/(.*=)/', '', $argument) == '--output' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$pole['output'] = preg_replace('/(.*=)/', '', $argument);
				}
				break;

			case '-h':
			# -h=subst
				if (array_key_exists('h', $pole) || preg_replace('/(.*=)/', '', $argument) == '-h' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$pole['h'] = preg_replace('/(.*=)/', '', $argument);
				}
				break;

			case '-n':
				# -n
				if (array_key_exists('n', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['n'] = true;
				}
				break;

			case '-r':
				# -r=root-element
				if (array_key_exists('r', $pole) || preg_replace('/(.*=)/', '', $argument) == '-r' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$pole['r'] = validate_name(preg_replace('/(.*=)/', '', $argument), 3);
				}
				break;

			case '--array-name':
				# --array-name=array-element
				if (array_key_exists('array-name', $pole) || preg_replace('/(.*=)/', '', $argument) == '--array-name' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$pole['array-name'] = validate_name(preg_replace('/(.*=)/', '', $argument), 3);
				}
				break;

			case '--item-name':
				# --item-name=item-element
				if (array_key_exists('item-name', $pole) || preg_replace('/(.*=)/', '', $argument) == '--item-name' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$pole['item-name'] = validate_name(preg_replace('/(.*=)/', '', $argument), 3);
				}
				break;

			case '-s':
				# -s
				if (array_key_exists('s', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['s'] = true;
				}
				break;

			case '-i':
				# -i
				if (array_key_exists('i', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['i'] = true;
				}
				break;

			case '-l':
				# -l
				if (array_key_exists('l', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['l'] = true;
				}
				break;

			case '-c':
				# -c
				if (array_key_exists('c', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['c'] = true;
				}
				break;

			case '-a':
			case '--array-size':
				# -a, --array-size
				if (array_key_exists('array-size', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['array-size'] = true;
				}
				break;

			case '-t':
			case '--index-items':
				# -t, --index-items
				if (array_key_exists('index-items', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['index-items'] = true;
				}
				break;

			case '--start':
				# --start=n
				if (array_key_exists('start', $pole) || preg_replace('/(.*=)/', '', $argument) == '--start' || preg_replace('/(.*=)/', '', $argument) == '') {
					exit(1);
				} else {
					$string = preg_replace('/(.*=)/', '', $argument);
					if (is_numeric($string)) {		// kotrola zda-li je hodnota argumentu číslo
						((int)$string >= 0) ? $pole['start'] = (int)$string : exit(1);
					} else {
						exit(1);
					}
				}
				break;

			case '--types':
				# --types
				if (array_key_exists('types', $pole) || preg_match('/(=.*)/', $argument) != 0) {
					exit(1);
				} else {
					$pole['types'] = true;
				}
				break;

			default:
			// neznámé argumenty
				exit(1);
				break;

		}
	}

	# podmínky, které musejí některé argumenty splnit

	// kontrola existence input souboru
	if (array_key_exists('input', $pole) == true && file_exists($pole['input']) == false) {
		exit(2);	// soubor neexistuje
	}

	// kontola, pokud byl zadán argument '--start=n', jestli byl zadán i argument '-t' nebo '--index-items'
	if (array_key_exists('start', $pole) == true && array_key_exists('index-items', $pole) == false) {
		exit(1);
	}

	// nastavení implicitní hodnoty, pokud byl pouze zadán argument '-t' nebo '--index-items', ale ne argument '--start=n'
	if (array_key_exists('start', $pole) == false && array_key_exists('index-items', $pole) == true) {
		$pole['start'] = 1;
	}

	// nastavení implicitní hodnoty, pokud nenbyl zadán argument '-h=subst'
	if (array_key_exists('h', $pole) == false) {
		$pole['h'] = '-';
	}

	// nastavení implicitní hodnoty, pokud nebyl zadán argument '--array-name=array-element'
	if (array_key_exists('array-name', $pole) == false) {
		$pole['array-name'] = "array";
	}

	// nastavení implicitní hodnoty, pokud nebyl zadán argument '--item-name=item-element'
	if (array_key_exists('item-name', $pole) == false) {
		$pole['item-name'] = "item";
	}

	return $pole;	// vše OK -> lze array vrátit

}

?>
