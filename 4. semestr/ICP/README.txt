ICP Projekt
Tým:		Michal Hrabovský (xhrabo08) - vedoucí
			Jakub Frýz (xfryzj01)

HowTO:

Input:
1.
	* ID hry (1-4)
	* 'H' - nápověda
2. pokud ID hry, tak dále:
	* 'D' - vytažení karty
	* 'R' - přehození balíčku
	* 'H [count]' - počet nápověd tahů
	* 'L [filename]' - načtení hry
	* 'S [filename]' - uložení hry
	* 'N' - nová hra
	* '[letterfrom][idfrom][letterto][idto] [count]'
		- letterftom	== [P|W|F] odkud vzít kartu
		- idfrom		== ID skupiny odkud vzít kartu
		- letterto		== [P|W|F] kam vložit kartu
		- idto			== ID skupiny kam vložit kartu
		- count			== počet karet k přesunutí
EXAMPLES: 2P1P2 2, 1D, 1H 3
