OMEZENÍ

'and' s 'or' nejsou podporovány

při SearchResultEntry nejsou ošetřeny některé chyby (aplikace v tom případě odešle klientu SearchResultDone s návratovou hodnotou 0 a žádný SearchResultEntry)

ignoruje otázky na jména objektů (název 'person' je natvrdo daný)

pokud je TKV filtr SearchRequestu typu present, tak server vypíše prvních SIZE_LIMIT záznamů (u mne to funguje, na evě klient vrátí chybu)
