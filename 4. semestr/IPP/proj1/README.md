# cesty ke vstupním a výstupním souborům
LOCAL_IN_PATH="./" # (simple relative path)
LOCAL_IN_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_IN_PATH3="`pwd`/" #Alternative 2 (absolute path)
LOCAL_OUT_PATH="./" # (simple relative path)
LOCAL_OUT_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_OUT_PATH3="`pwd`/" #Alternative 2 (absolute path)
# cesta pro ukládání chybového výstupu studentského skriptu
LOG_PATH="./"


# test01: prazdny objekt, vystupem jen hlavicka; Expected output: test01.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test01.jsn --output=${LOCAL_OUT_PATH}test01.xml 2> ${LOG_PATH}test01.err
echo -n $? > test01.!!!

# test02: prazdny objekt, vystupem je prazdny XML (hlavicka vynechana); Expected output: test02.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test02.jsn --output=${LOCAL_OUT_PATH}test02.xml -n -r="koren" 2> ${LOG_PATH}test02.err
echo -n $? > test02.!!!

# test03: jednoduchy objekt, neobaluji (=>nevalidni XML vsak nevadi); Expected output: test03.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION  -n --types --input=${LOCAL_IN_PATH}test03.jsn > "${LOCAL_OUT_PATH3}test03.xml" 2> ${LOG_PATH}test03.err
echo -n $? > test03.!!!

# test04: jednoduchy objekt obalen a vypustena hlavicka; Expected output: test04.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION  --output=${LOCAL_OUT_PATH}test04.xml -n -r="koren" < ${LOCAL_IN_PATH}test04.jsn 2> ${LOG_PATH}test04.err
echo -n $? > test04.!!!

# test05: jednoduchy objekt obalen a vypustena hlavicka, nevyznamne --array-name; Expected output: test05.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -n -r="koren" --array-name="pole" < ${LOCAL_IN_PATH2}test05.jsn > ${LOCAL_OUT_PATH2}test05.xml 2> ${LOG_PATH}test05.err
echo -n $? > test05.!!!

# test06: globalni pole, literaly transformuji na elementy; Expected output: test06.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test06.jsn --output=${LOCAL_OUT_PATH}test06.xml -l 2> ${LOG_PATH}test06.err
echo -n $? > test06.!!!

# test07: globalni pole s parametry -r a --item-name, velikost pole; Expected output: test07.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -r="root" --item-name="pól" --input="${LOCAL_IN_PATH}test07.jsn" --output="${LOCAL_OUT_PATH}test07.xml" -a 2> ${LOG_PATH}test07.err
echo -n $? > test07.!!!

# test08: objekt s polem uvnitr; indexace polozek pole; neobsahuje retezcovy literal, takze se -s neuplatni; Expected output: test08.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test08.jsn --output=${LOCAL_OUT_PATH}test08.xml -n -s --start=0 -t 2> ${LOG_PATH}test08.err
echo -n $? > test08.!!!

# test09: slozitejsi objekt (generuje nevalidni XML; -s => retezce jsou transformovany na textove elementy misto atributu); Expected output: test09.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test09.jsn --output=${LOCAL_OUT_PATH}test09.xml -n -s 2> ${LOG_PATH}test09.err
echo -n $? > test09.!!!

# test10: vstup neni formatovan, obalujici element obsahuje pomlcku (vznika validni element); Expected output: test10.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}test10.jsn" --output=${LOCAL_OUT_PATH}test10.xml -r="tešt-élěm" 2> ${LOG_PATH}test10.err
echo -n $? > test10.!!!

# test11: specialni znaky v hodnote (-c); Expected output: test11.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test11.jsn --output=${LOCAL_OUT_PATH2}test11.xml -c -l -r="rOOt" 2> ${LOG_PATH}test11.err
echo -n $? > test11.!!!

# test12: specialní znaky i diakritika v hodnotě (-c), dále -r a -s; Expected output: test12.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test12.jsn --output=${LOCAL_OUT_PATH2}test12.xml -c -r=root -s 2> ${LOG_PATH}test12.err
echo -n $? > test12.!!!

# test13: komplexni priklad kombinace parametru; Expected output: test13.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -a --input="${LOCAL_IN_PATH3}test13.jsn" -l --output=${LOCAL_OUT_PATH2}test13.xml -r="root" -s --start="2" --index-items 2> ${LOG_PATH}test13.err
echo -n $? > test13.!!!

# test14: chybny element i po nahrazeni pomlckami; Expected output: test14.xml; Expected return code: 51
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}test14.jsn" --output=${LOCAL_OUT_PATH}test14.xml -r="root" 2> ${LOG_PATH}test14.err
echo -n $? > test14.!!!

# test15: chyne jmeno elementu v prikazove radce; Expected output: test15.xml; Expected return code: 50
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}test15.jsn" --output=${LOCAL_OUT_PATH2}test15.xml --array-name="b<a>d" 2> ${LOG_PATH}test15.err
echo -n $? > test15.!!!

# test16: -i a dale -h=xyz, takze skript neskonci chybou; Expected output: test16.xml; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -i --input="${LOCAL_IN_PATH3}test16.jsn" --output="${LOCAL_OUT_PATH3}test16.xml" -h=xyz 2> ${LOG_PATH}test16.err
echo -n $? > test16.!!!
