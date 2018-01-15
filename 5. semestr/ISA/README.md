# ISA projekt: LDAP server (Vladimír Veselý)

### ZADÁNÍ:
Vašim úkolem je:
1) za 6 bodů: Nastudovat si Lightweight Directory Access Protocol a podstatné informace o protokolu, jeho činnosti a vaší implementaci sepsat do dokumentace.
2) za 14 bodů: Vytvořit jednoduchý LDAP server, který bude načítat dotazy od LDAP klientů a vyhledávat odpovědi v lokální textové databázi.
3) za bonus až +5 bodů: Doprogramovat podporu národních znaků. Doprogramovat na serveru podporu SASL autentizace. Bonusové úkoly lze řešit nezávisle na sobě.

### UPŘESNĚNÍ ZADÁNÍ:
1) V dobré dokumentaci se OČEKÁVÁ následující: titulní strana, obsah, logické strukturování textu, výcuc relevantních informací z nastudované literatury, popis zajímavějších pasáží implementace, sekce o testování, bibliografie, popisy k řešení bonusových zadání.

2) Server bude paralelní a neblokující. Server bude implementován v C či C++.

#### Velice zjednodušeně stavový automat serveru:

* Klient zahajuje komunikaci se serverem odesláním BindRequest.
* Server odpovídá zprávou BindResponse.
* Klient žádá o vyhledání záznamu pomocí SearchRequest.
* Server odpovídá na dotazy typu SearchRequest (type 3) pro každý záznam zprávou SearchResEntry. Bude očekávat atributy řetězec typu cn, uid nebo email a odpovědí budou vyhledané řetězce cn a mail. Lze také vyhledávat podle části řetězce.
* Server informuje klienta o ukončení vyhledávání SearchResDone.
* Klient uzavírá komunikaci se serverem díky UnbindRequest.

Server nemusí podporovat češtinu či jiné národní znaky. Server nevyžaduje autentizaci (simple bind), stačí podpora LDAPv2, který řeší pouze vyhledávání. Ostatní zprávy server ignoruje, popřípadě odpoví, že danému příkazu nerozumí (viz specifikace protokolu LDAP).

Pracovní databáze serveru má formát textového souboru CSV (comma separated values se středníkem jako oddělovačem), který obsahuje tři položky - Jméno (cn, CommonName), Login (uid, UserID), Email (mail). Ukázkový příklad takové databáze je ke stažení na http://nes.fit.vutbr.cz/ivesely/isa2017-ldap.csv.

#### Závazný formát výstupní binárky:

	./myldap {-p <port>} -f <soubor>
Význam parametrů a jejich hodnot:

	-p <port>
Umožňuje specifikovat konkrétní port, na kterém začne server naslouchat požadavkům klientů. Výchozí hodnota čísla portu je **389**.

	-f <soubor>
Cesta k textovému soubor ve formátu CSV.

Za referenční prostředí pro běh vašeho LDAP serveru berte MERLIN.fit.vutbr.cz, na kterém proběhne také ověření funkcionality vašeho projektu. Využít můžete POUZE a JEN na MERLINu dostupné standardní systémové knihovny (packet craftovací knihovny třetích stran jsou zakázány). Jako vhodný software pro vlastní testování a klientskou stranu projektu lze doporučit ldapsearch, který je součástí OS serveru EVA.fit.vutbr.cz.

3) Bonusem můžete lepit bodové ztráty z předchozích částí projektu.

### DOPORUČENÍ/OMEZENÍ:
Všechny implementované programy by měly být použitelné a řádně komentované. Pokud už přejímáte zdrojové kódy z různých tutoriálů či příkladů z Internetu (ne mezi sebou pod hrozbou ortelu disciplinární komise), tak je POVINNÉ správně vyznačit tyto sekce a jejich autory dle licenčních podmínek, kterými se distribuce daných zdrojových kódů řídí. V případě nedodržení bude na projekt nahlíženo jako na plagiát!

U syntaxe vstupních voleb jednotlivým programům složené závorky {} znamenají, že volba je NEPOVINNÁ, (pokud není přítomna, tak se použíje implicitní hodnota), přičemž pořadí jednotlivých voleb a jejich parametrů může být libovolné. Pro jejich snadné parsování se DOPORUČUJE použít funkci getopt().

Ukončení aplikace z bodu 2) bude vázáno na SIGINT signál (tedy Ctrl+C).
Projekt bude opravován ručně. Počítejte tedy s nejzazším možným termínem oprav a reklamací určených garantem předmětu.

Výsledky vaší implementace by měly být co možná nejvíce multiplatformní mezi OS založenými na unixu (dbejte na to zejména při vytváření Makefile).

Do souboru Readme uveďte případná omezení funkcionality vašeho projektu - na dokumentovanou chybu se nahlíží v lepším světle než na nedokumentovanou!

Jen pro představu je mé vlastní ošklivé řešení projektu na cca 690 řádků zdrojového kódu a 100 řádků hlavičkové knihovny.

### LITERATURA:
* LDAP specifikace: RFC 2251, 2252, 2254, 4511
* T.A.Howes, M.C.Smith, G.S.Gordon" LDAP Directory Services, Addison-Wesley, 2002
* ASN.1 specifikace: http://www.itu.int/en/ITU-T/asn1/Pages/introduction.aspx a http://www.oss.com/asn1/resources/books-whitepapers-pubs/asn1-books.html a http://luca.ntop.org/Teaching/Appunti/asn1.html
* manuálové stránky příkazu ldapsearch