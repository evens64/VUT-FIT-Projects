ve vyjímečných případech:
	1.	může nastat neukončení procesu vlákna
		(mi osobně se to stale jen jednou a později při spuštění přes valgrind)
	2.	po přijmutí signálu SIGINT bude muset uživatel zmáčknout ještě klávesu ENTER
		(nastalo pouze dvakrát na začátku asi z 35 pokusů, nepodařilo se najít chybu)
	3.	jelikož nejsou v projektu žádné semafory, je zde možnost kolize při pokusu o zápis na stdout,
		pokud v hlavním procesu nastane chyba a vlákno přijme zprávu.
		(částečně ošetřeno pomocí fce shutdown(), která zakáže příjem zpráv)
