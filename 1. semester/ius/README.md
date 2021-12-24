# Project
### Zadání
Vytvořte informační systém pro hlášení a správů chyb a zranitelností systému.
Systém umožňuje uživatelům hlásit bugy, jejich závažnosti a moduly, ve kterých se vyskytly, ve formě tiketů.
Tikety mohou obsahovat hlášení o více než jednom bugu a stejný bug může být zahlášen více uživateli.
Bug může (ale nemusí) být zranitelností a v tomto případě zaevidujeme i potenciální míru nebezpečí zneužití této zranitelnosti.
V případě zahlášení bugů, odešle systém upozornění programátorovi, který zodpovídá za daný modul, přičemž může odpovídat za více modulů.
Programátor pak daný tiket zabere, přepne jeho stav na "V řešení" a začne pracovat na opravě ve formě Patche.
Patch je charakterizován datem vydání a musí být schválen programátorem zodpovědným za modul, které mohou být v různých programovacích jazycích.
Jeden Patch může řešit více bugů a současně řešit více tiketů a vztahuje se na několik modulů.
Samotní uživatelé mohou rovněž tvořit patche. Takové patche však musí projít silnější kontrolou než jsou zavedeny do systému.
Kromě data vytvoření patche rovněž evidujte datum zavedení patche do ostrého provozu.
Každý uživatel a programátor je charakterizován základními informacemi (jméno, věk, apod.), ale současně i jazyky, kterými disponuje, apod.
V případě opravení bugů, mohou být uživatele upozorněni na danou opravu a případně být odměněni peněžní hodnotou (podle závažnosti bugu či zranitelnosti).

---
### Model případů užití
První částí projektu je vypracovat model případů užití.

Diagram případů užití by měl být na jednu stranu formátu A4, měl by obsahovat více než jednoho aktéra a sadu případů užití.
Případy užití volte dostatečně rozdílné, aby bylo poznat, že modelu a jeho smyslu rozumíte.
V případě použití include/extend relací si dejte pozor na jejich správnou aplikaci.

Dobrou volbou případů užití dáváte najevo, že problematice rozumíte, zatímco volbou příliš podobných případů užití
(např. vytvořit, změnit a zrušit objednávku) ukazujete, že si příliš jisti nejste a navíc si chcete ušetřit práci.

---
### ER diagram (Entity Relationship Diagram)
Druhou částí projektu je vypracování ER diagramu, ten také slouží pro prezentaci požadavků definovaných zákazníkem.
Pomocí ER diagramu se modelují perzistentní data a vztahy mezi nimi.
Jedná se o vyšší úroveň dat (abstrakci), která jsou v systému uložena a mohou být dále zpracovávána.

Perzistentní data v informačním systému zůstávají, i po vypnutí systému.
ER diagram je základem pro návrh schématu databáze (hlaviček jednotlivých tabulek).
Tvoření databázového systému si prakticky vyzkoušíte v předmětu IDS.

Při vypracovávání ER diagramu si dejte pozor, abyste opravdu modelovali data v klidu
(při nepochopení ER diagramu, tj. modelování dynamické práce s daty, se může rapidně zhoršit hodnocení).
Modelování dynamiky se typicky projeví při pojmenovávání vztahů mezi entitními množinami jako "edituje", "prohlíží si", "zadává" atd.
Opakujeme: jde o data, která zůstávají v systému i po jeho vypnutí. Atributy entitních množin by měly být atomické (již nedělitelné).
Pakliže máte atribut, jenž by měl složitý datový typ, rozmyslete se, zda by nebylo lepší modelovat ho jako entitní množinu.

Vztahy jsou v ER diagramu vyjádřeny čarami mezi entitními množinami, cízí klíče (primární klíče jiných entitních množin)
se tudíž mezi atributy dáné entitní množiny nenachází. Tyto se vyskytují až v databázových tabulkách při implementaci v rámci databázového systému.
Při návrhu ER diagramu je nutné správně porozumět konceptu kardinality vztahů.
Při použití vztahu 1:1..N (oproti vztahu 1:0..N) je třeba mít dobrý důvod pro to,
aby musela existovat minimálně jedna entita z entitní množiny na pravé straně,
která je ve vztahu s entitou na levé straně. Vztahy 1:1 jsou podezřelé, protože takové entity množiny je možné sloučit do jedné.
Vztah n:n je sice při implementaci do tabulek problematický, ale v ER diagramech být může.
Není ani chybou, pokud se již v ER diagramu převede přes vazební entitní množinu na dva vztahy 1:n
(pozor ale na správnou orientaci kardinalit - kardinalita n je v obou případech na straně vazební entitní množiny).

Oba modely musí splňovat funkcionalitu a být konzistentní, to znamená mimo jiné:

* Názvy entitních a vztahových množin ER diagramu musí korespondovat se jmény v názvech a popisech případů užití.
* Ke každému případu užití musí existovat vhodné entitní množiny, se kterými se manipuluje:
  Například pokud mám případ užití Zadat novou žádost, musím mít v ER diagramu entitní množinu, která obsahuje údaje o zadaných žádostech.
* Naopak ke každé entitní množině by měl existovat případ užití, který přidá/mění/maže položky příslušné entitní množiny:
  Pokud mám například entitní množinu Uživatel, je nutnou součástí modelu případu užití možnost správy uživatelů. 
