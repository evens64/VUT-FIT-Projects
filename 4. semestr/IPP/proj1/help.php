<?php

/**
 * Výpis nápovědy na výstup
 */
function print_help() {

	echo "\033[1;32mNápověda:\033[0m\n";
	echo "\tSkript pro konverzi JSON formátu do XML\n";
	echo "\n";

	echo "\033[1;32mPoužití:\033[0m\n";
	echo "\tphp5.6 jsn.php [parametry]\n";
	echo "\n";

	echo "\033[1;32mParametry skriptu:\033[0m\n";

	echo "\t\033[1;33m--help\033[0m\n";
	echo "\t\tnápověda skriptu\n";

	echo "\t\033[1;33m--input=filename\033[0m\n";
	echo "\t\tvstupní soubor; není-li zadán, použije se standardní vstup\n";

	echo "\t\033[1;33m--output=filename\033[0m\n";
	echo "\t\tvýstupní soubor; není-li zadán, použije se standardní výstup\n";

	echo "\t\033[1;33m-h=subst\033[0m\n";
	echo "\t\tve jméně elementu odvozeném z dvojice jméno-hodnota nahraďte každý nepovolený znak ve jméně XML značky řetězcem \033[1;32msubst\033[0m\n";

	echo "\t\033[1;33m-n\033[0m\n";
	echo "\t\tnegenerovat XML hlavičku na výstup skriptu\n";

	echo "\t\033[1;33m-r=root-element\033[0m\n";
	echo "\t\tjméno párového kořenového elementu obalujícího výsledek\n";

	echo "\t\033[1;33m--array-name=array-element\033[0m\n";
	echo "\t\tpřejmenování elementu obalující pole z implicitní hodnoty \033[1;32marray\033[0m na \033[1;32marray-element\033[0m\n";

	echo "\t\033[1;33m--item-name=item-element\033[0m\n";
	echo "\t\tpřejmenování elementu pro prvky pole z implicitní hodnoty \033[1;32mitem\033[0m na \033[1;32mitem-element\033[0m\n";

	echo "\t\033[1;33m-s\033[0m\n";
	echo "\t\thodnoty (v dvojici i v poli) typu \033[1;32mstring\033[0m budou transformovány na textové elementy místo atributů\n";

	echo "\t\033[1;33m-i\033[0m\n";
	echo "\t\thodnoty (v dvojici i v poli) typu \033[1;32mnumber\033[0m budou transformovány na textové elementy místo atributů\n";

	echo "\t\033[1;33m-l\033[0m\n";
	echo "\t\thodnoty literálů (\033[1;32mtrue\033[0m, \033[1;32mfalse\033[0m, \033[1;32mnull\033[0m) budou transformovány na elementy \033[1;32m<true/>\033[0m, \033[1;32m<false/>\033[0m a \033[1;32m<null/>\033[0m místo na atributy\n";

	echo "\t\033[1;33m-c\033[0m\n";
	echo "\t\taktivace překladu problematických znaků s UTF-8 kódem menším jak 128\n";

	echo "\t\033[1;33m-a, --array-size\033[0m\n";
	echo "\t\tu pole bude doplněn atribut \033[1;32msize\033[0m s uvedením počtu prvků v tomto poli\n";

	echo "\t\033[1;33m-t, --index-items\033[0m\n";
	echo "\t\tke každému prvku pole bude přidán atribut \033[1;32mindex\033[0m s určením indexu prvku v tomto poli\n";
	echo "\t\tčíslování začíná od \033[1;32m1\033[0m, pokud není parametrem \033[1;32m--start\033[0m určeno jinak\n";

	echo "\t\033[1;33m--start=n\033[0m\n";
	echo "\t\tinicializace inkrementálního čitače pro indexaci prvků pole na zadané kladné celé číslo \033[1;32mn\033[0m včetně nuly\n";
	echo "\t\tnutno kombinovat s parametrem \033[1;32m--index-items\033[0m, jinak chyba s návratovým kódem \033[1;32m1\033[0m\n";
	echo "\t\timplicitně \033[1;32mn=1\033[0m\n";

	echo "\t\033[1;33m--types\033[0m\n";
	echo "\t\tobalující element každé skalární hodnoty bude doplněn o atribut \033[1;32mtype\033[0m obsahující hodnotu:\n";
	echo "\t\t\t\033[1;32minteger\033[0m pro původní celočíselnou hodnotu,\n";
	echo "\t\t\t\033[1;32mreal\033[0m pro původní desetinnou hodnotu,\n";
	echo "\t\t\t\033[1;32mstring\033[0m pro řetězec,\n";
	echo "\t\t\t\033[1;32mliteral\033[0m pro hodnoty literálů\n";

	echo "\n";

}

?>
