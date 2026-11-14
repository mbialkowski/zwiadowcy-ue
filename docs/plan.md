# Zwiadowcy: Ruiny Gorlanu - gra w Unreal Engine 5

Plan osobnej gry TPP na podstawie moda do Minecrafta „Zwiadowcy: Ruiny Gorlanu”
(<https://github.com/mbialkowski/minecraft-mod-zwiadowcy>). Fabuła obejmuje tylko tom I. Oprawa nawiązuje nastrojem
do Elden Ringa: mroczne fantasy, złote światło i mgła. Niczego z Elden Ringa nie kopiujemy.

> Nieoficjalna gra fanowska na motywach „Zwiadowców” Johna Flanagana: darmowa i niekomercyjna. Świat, postacie i nazwy
> należą do ich właścicieli. Fabuła jest wolną interpretacją „Ruin Gorlanu”.

## Spis treści

1. [Decyzje z ankiety](#1-decyzje-z-ankiety)
2. [Technika](#2-technika)
3. [Świat](#3-świat)
4. [Fabuła i misje](#4-fabuła-i-misje)
5. [Rozgrywka](#5-rozgrywka)
6. [Postacie i przeciwnicy](#6-postacie-i-przeciwnicy)
7. [Oprawa](#7-oprawa)
8. [Assety](#8-assety)
9. [Wydajność (Radeon 890M)](#9-wydajność-radeon-890m)
10. [Repozytorium](#10-repozytorium)
11. [Etapy prac](#11-etapy-prac)
12. [Co musisz zrobić Ty](#12-co-musisz-zrobić-ty)
13. [Ryzyka](#13-ryzyka)

---

## 1. Decyzje z ankiety

| # | Temat | Decyzja |
|---|---|---|
| 1 | Silnik | Najnowszy **Unreal Engine 5.x z Epic Games Launchera** |
| 2 | Język | **C++** dla całej logiki, **cienkie Blueprinty** tylko do podpięcia assetów |
| 3 | Sprzęt docelowy | **Ten komputer** (Ryzen AI Z2 Extreme, Radeon 890M, 16 GB): **30+ kl./s**; mocne PC dostają wyższe ustawienia |
| 4 | Assety | **Fab/Megascans** (darmowe, pobierasz Ty) + **CC0** (Poly Haven, ambientCG, Quaternius) |
| 5 | Repozytorium | W gicie **kod i własne pliki**, CC0 przez Git LFS. Paczki z Fab w folderze ignorowanym, z listą do pobrania |
| 6 | Fabuła | **Główny wątek tomu I: 8 misji + epilog**, 3-5 h gry |
| 7 | Świat | **Jeden połączony świat ok. 2×2 km** (World Partition), zaprojektowany ręcznie |
| 8 | Walka | **Łuk + łagodny soulslike**: przewrót, wytrzymałość, namierzanie, zapowiadane ciosy, **bez parowania** |
| 9 | Postacie | **MetaHuman** + animacje z **Game Animation Sample Project** (motion matching) |
| 10 | Potwory | **Gotowe stwory z Fab**, przerobione na Wargali i Kalkarów |
| 11 | Sterowanie | **Pad pierwszy**, mysz i klawiatura też (Enhanced Input, podpowiedzi zależne od urządzenia) |
| 12 | Łuk | **Celowanie przez ramię z pomocą**: balistyka, lekkie przyciąganie na padzie, rodzaje strzał z moda |
| 13 | Skradanie | **Pełne**: stożki widzenia, wskaźnik wykrycia, trawa i cień, Bezruch płaszcza, ciche zabicie, odciąganie uwagi |
| 14 | Wyrwij | **Jazda i łuk z siodła**, gwizdek, zryw; bez walki wręcz konno |
| 15 | Rozwój | **Umiejętności za misje**, bez punktów doświadczenia |
| 16 | Śmierć | **Ogniska zwiadowców**: leczenie, strzały, powrót po śmierci, szybka podróż; po śmierci nic nie tracisz |
| 17 | Kalkarowie | **Dwie bestie, dwie fazy, ogień jako klucz**, spojrzenie grozy, Halt pomaga z daleka |
| 18 | Dialogi | **Okno dialogu z wyborem** + kilka **przerywników** w Sequencerze; prolog i epilog jako plansze |
| 19 | Głosy | **Bez głosów**, sam tekst i krótkie odgłosy |
| 20 | Język | **Polski + angielski** (tabele tekstów od początku) |
| 21 | Nastrój | **Mroczne, złote światło**: mgła w dolinach, ciepły Redmont, szara Równina Samotności |
| 22 | Pora dnia | **Ustalana przez misje**, swobodna eksploracja w złotym popołudniu |
| 23 | Interfejs | **Minimalny, średniowieczny**: kompas, znacznik celu (do wyłączenia), dziennik i mapa na pergaminie |
| 24 | Dźwięk | **Darmowe biblioteki CC0/CC-BY**, lista autorów w napisach |
| 25 | Zapis | **Autozapis przy ogniskach i po etapach misji + 3 profile** |
| 26 | Trudność | **Jedna, wyważona** |
| 27 | Pierwszy etap | **Pionowy wycinek: las Halta** w docelowej oprawie |
| 28 | Budowa świata | **Całość robię ja skryptami** (nie znasz UE): teren, las, budynki, testy i zrzuty |
| 29 | Uruchamianie | **Stała zgoda** na uruchamianie edytora, gry i testów na tym komputerze |
| 30 | Pobieranie | **Zgoda na listy pakietowe**: jedna lista (źródła, licencje, rozmiar) przed każdą partią |
| 31 | Tytuł | **Zwiadowcy: Ruiny Gorlanu** |
| 32 | Dystrybucja | **GitHub Releases**, zip dla Windows podzielony na części do 2 GB |
| 33 | Towarzysze | **Tylko w wybranych scenach**: Halt w zasadzce i przy Kalkarach, Gilan na Zlocie |
| 34 | Eksploracja | **Obozy Wargali, kurhany z kryptami, strażnice, punkty widokowe, zapiski** |

---

## 2. Technika

### 2.1 Projekt

- **Silnik:** najnowszy UE 5.x z Launchera. Numer wersji zapisujemy w `ZwiadowcyUE.uproject` i w README.
- **Moduły C++:**
  - `Zwiadowcy` (gra): postać, kamera, walka, łuk, skradanie, koń, AI, misje, dialogi, zapis, interfejs,
  - `ZwiadowcyEditor` (tylko edytor): komendy do budowy świata i testów wywoływane ze skryptów.
- **Narzędzia:** Visual Studio 2022 Community (kompilacja C++), Python wbudowany w edytor UE (skrypty budowy świata).
  Systemowy Python nie jest potrzebny.

### 2.2 Praca bez klikania w edytorze

Nie znasz UE, więc wszystko robię z wiersza poleceń:

| Zadanie | Jak |
|---|---|
| Kompilacja | `Build.bat ZwiadowcyEditor Win64 Development` (Unreal Build Tool) |
| Budowa świata | skrypty `Scripts/*.py` uruchamiane przez `UnrealEditor-Cmd.exe -run=pythonscript`: teren z mapy wysokości, PCG (las, trawa, skały), rozstawianie budynków i znaczników misji z plików JSON |
| Testy | Automation Tests i Functional Tests (`-ExecCmds="Automation RunTests Zwiadowcy"`): misje od początku do końca, AI, zapis |
| Wygląd | tryb zrzutów jak `PhotoTour` w modzie: gra sama odwiedza kadry i robi zrzuty, które oglądam i poprawiam |
| Wydajność | `stat unit`/`stat fps` i Unreal Insights w tych samych kadrach, raport kl./s na 890M |
| Paczka | `RunUAT BuildCookRun` → zip dla Windows |

### 2.3 Dane jak w modzie

Misje, dialogi i lokacje to pliki JSON w `Content/Data/` (format zbliżony do moda, ładowany przez C++), a teksty siedzą
w tabelach CSV (`Content/Localization/*.csv` → String Tables). Dzięki temu:

- fabułę, dialogi i zapiski z moda przenoszę prawie bez zmian,
- poprawka tekstu nie wymaga edytora ani ponownej kompilacji,
- angielski to druga kolumna w tych samych tabelach.

### 2.4 Główne systemy (C++)

| System | Klasy (zarys) |
|---|---|
| Postać i kamera | `AZwPlayerCharacter`, `UZwCameraComponent` (TPP, przez ramię przy celowaniu, namierzanie) |
| Ruch | Character Movement + motion matching z Game Animation Sample; przewrót, sprint, kucanie |
| Wytrzymałość i zdrowie | `UZwVitalsComponent` |
| Łuk | `UZwBowComponent`, `AZwArrow` (balistyka, rodzaje strzał, przyciąganie celownika na padzie) |
| Walka wręcz | `UZwMeleeComponent` (saksa, noże do rzucania), trafienia z okien animacji |
| Skradanie | `UZwStealthComponent` (widoczność: cień, trawa, ruch, płaszcz), `UZwPerceptionComponent` wrogów (stożek, słuch, wskaźnik) |
| Koń | `AZwHorse` (Wyrwij: jazda, zryw, gwizdek, łuk z siodła) |
| AI | StateTree: patrol, podejrzenie, alarm, walka, ucieczka; `AZwEnemy` z wariantami |
| Bossowie | `AZwKalkar` (fazy, odporność na zwykłą broń, ogień, spojrzenie grozy) |
| Misje | `UZwQuestSubsystem` (etapy, cele, wyzwalacze - jak w modzie) |
| Dialogi | `UZwDialogueSubsystem` + widget okna dialogu |
| Ogniska | `AZwCampfire` (odpoczynek, powrót, szybka podróż, autozapis) |
| Zapis | `UZwSaveGame` (3 profile, autozapis) |
| Interfejs | HUD (zdrowie, wytrzymałość, strzała, ukrycie, kompas), dziennik, mapa, menu (UMG + Common UI) |

---

## 3. Świat

Jeden połączony świat **ok. 2×2 km** z World Partition. Współrzędne w metrach, (0, 0) w środku; +x na wschód, +y na południe.
Granice tworzą Góry Deszczu i Nocy (południe i wschód), las (zachód) i jezioro z bagnami (północ).

| Miejsce | Położenie (m) | Opis |
|---|---|---|
| **Zamek Redmont** i wioska | (-300, 0) | zamek na wzgórzu, dziedziniec, wielka sala; ciepłe światło, dym z kominów |
| **Las i chata Halta** | (100, -200) | gęsty las, polana z chatą, strzelnica (pionowy wycinek) |
| **Farma Starego Boba** | (150, 300) | łąki, padok, stajnia |
| **Legowisko dzika** | (350, -450) | głęboki las, wykroty |
| **Obóz Zlotu** | (-400, -700) | namioty nad jeziorem, strzelnica konkursowa |
| **Spalona osada** | (550, 50) | zgliszcza na skraju lasu |
| **Równina Samotności** | (500…950, 300…950) | szara, martwa równina, kamienne kręgi, mgła |
| **Ruiny Gorlanu** | (750, 750) | twierdza w ruinie, arena Kalkarów, krypta |

**Miejsca do odkrycia:**

- 4 obozy Wargali,
- 3 kurhany z kryptami,
- 2 strażnice,
- 5 punktów widokowych (odkrywają kawałek mapy),
- ok. 20 zapisków z lore (teksty z moda),
- 6 ognisk zwiadowców.

---

## 4. Fabuła i misje

| # | Misja | Miejsce | Najważniejsze |
|---|---|---|---|
| 1 | **Dzień Wyboru** | Redmont | samouczek ruchu i rozmów; Halt wybiera Willa |
| 2 | **Chata w lesie** | chata Halta | szkolenie: łuk (strzelnica), noże i saksa, Bezruch płaszcza; umiejętności za każdą część |
| 3 | **Koń od Starego Boba** | farma | oswojenie Wyrwija, jazda, gwizdek |
| 4 | **Polowanie na dzika** | legowisko | pierwsza prawdziwa walka (dzik), ratunek Horace'a |
| 5 | **Zlot Zwiadowców** | obóz Zlotu | konkurs łuczniczy, Gilan, wieści o Kalkarach |
| 6 | **Tropem bestii** | spalona osada | tropienie śladów, zasadzka Wargali z Haltem |
| 7 | **Równina Samotności** | równina, ruiny | skradanie wśród Wargali do ruin Gorlanu |
| 8 | **Ogień w ciemności** | arena w ruinach | walka z dwoma Kalkarami (rozdział 6.3), Halt strzela z daleka |
| - | **Epilog: Przed obliczem króla** | Redmont | przerywnik i wybór: Szkoła Rycerska albo Korpus Zwiadowców; plansze epilogu |

**Przerywniki** (Sequencer):

- Dzień Wyboru,
- przybycie na Zlot,
- pierwsze spotkanie z Kalkarem,
- upadek Kalkarów,
- przed królem.

Prolog i epilog to ilustrowane plansze z tekstami z moda.

---

## 5. Rozgrywka

- **Kamera:** TPP za plecami Willa. Przy celowaniu kamera przechodzi nad prawe ramię. Namierzanie celu na pad (R3)
  i mysz (środkowy przycisk).
- **Ruch:** chód, bieg, sprint (wytrzymałość), kucanie, przewrót z krótką nietykalnością (łagodne okno), wspinanie
  na niskie przeszkody.
- **Łuk:**
  - naciąg z balistyką, strzały opadają,
  - na padzie lekkie przyciąganie celownika, a namierzony cel łatwiej trafić,
  - strzały: zwykłe, płonące (podpalają olej i Kalkarów), z szerokim grotem (więcej obrażeń), gwiżdżące
    (odciągają uwagę), z liną (lina do wspinaczki w wybranych miejscach),
  - kołczan wybiera się kołem rodzajów strzał.
- **Wręcz:** saksa (szybkie cięcia), noże do rzucania. Wrogowie zapowiadają ciosy (błysk, zamach), a obroną jest
  unik. **Parowania nie ma.**
- **Skradanie:**
  - nad wrogiem rośnie wskaźnik wykrycia (podejrzenie → alarm),
  - widoczność zależy od cienia, wysokiej trawy, ruchu i płaszcza,
  - Bezruch: w płaszczu i bez ruchu jesteś prawie niewidoczny,
  - ciche zabicie od tyłu,
  - kamień albo gwiżdżąca strzała odciągają uwagę.
- **Wyrwij:** przywołanie gwizdkiem, galop, zryw na wytrzymałości konia, strzelanie z siodła.
- **Umiejętności za misje:** szybszy naciąg, cichszy krok, rzut nożem, lepszy Bezruch, zryw Wyrwija.
- **Ogniska zwiadowców:** leczenie i uzupełnienie strzał, powrót po śmierci, autozapis i szybka podróż między
  odkrytymi ogniskami. Śmierć nic nie zabiera.
- **Trudność:** jedna, wyważona. Zapowiedzi ciosów są wyraźne, a checkpointy częste.

---

## 6. Postacie i przeciwnicy

### 6.1 Postacie

MetaHuman:

- Will, Halt, Horace, baron Arald,
- Gilan, Crowley, Stary Bob,
- wieśniacy (2-3 warianty),
- król Duncan (epilog).

Stroje pochodzą z assetów Fab: płaszcz zwiadowcy z kapturem, zbroje rycerzy, ubrania chłopskie.

### 6.2 Przeciwnicy

| Wróg | Źródło modelu | Zachowanie |
|---|---|---|
| Wargal-wojownik | stwór z Fab (np. wilkołak/gnoll), przerobiony | podstawowy żołnierz Morgaratha, ciosy z zapowiedzią |
| Wargal-zwiadowca | ten sam, mniejszy | szybki, poluje w grupach |
| Wargal-oszczepnik | ten sam + oszczep | rzuca z dystansu |
| Herszt Wargali | ten sam, większy | mini-boss obozów |
| Dzik | zwierzę z Fab | szarża (misja 4) |
| Kalkar ×2 | duży stwór z Fab | boss misji 8 |

### 6.3 Kalkarowie (misja 8)

- **Arena:** ruiny Gorlanu we mgle o zmierzchu, z pasem boss-a na ekranie.
- **Odporność:** zwykłe strzały i saksa zadają bardzo mało obrażeń. Ogień (flasza oleju i płonąca strzała) zadaje dużo.
- **Spojrzenie grozy:** zapowiedziane rykiem. Unieruchamia Willa, jeśli patrzy na bestię. Odwrócenie kamery albo
  zasłona przerywa efekt.
- **Faza 1:** dwie bestie naraz. Halt strzela z murów i odciąga jedną z nich.
- **Faza 2:** gdy pierwsza płonie i ucieka, druga wpada w szał: szybsze szarże, łamie osłony.
- **Śmierć:** walka wraca do początku bieżącej fazy.

---

## 7. Oprawa

- **Kierunek:** mroczne fantasy w złotym świetle.
  - kolory stonowane, a słońce nisko i ciepłe,
  - mgła wolumetryczna w dolinach,
  - ogromne drzewa i ruiny,
  - Redmont jako ciepła oaza,
  - Równina Samotności szara i martwa.
- **Postprocess:** lekkie ziarno, winieta i delikatny bloom, korekcja kolorów osobno dla każdej krainy.
- **Pora dnia:** ustalona dla misji. Dzień Wyboru w pełnym słońcu, Kalkarowie we mgle o zmierzchu, a swobodna
  eksploracja w złotym popołudniu.

---

## 8. Assety

**Fab** (darmowe, pobierasz Ty - lista z dokładnymi nazwami powstanie w etapie 0, każdą pozycję sprawdzę przed
dodaniem):

- Megascans: skały, las, ruiny, roślinność,
- Game Animation Sample Project (motion matching),
- MetaHuman Creator,
- konie i stwory na Wargali i Kalkarów.

**CC0** (pobieram ja, za zgodą na listę):

- Poly Haven: niebo HDRI, tekstury, część modeli,
- ambientCG: tekstury,
- Quaternius: rekwizyty,
- dźwięki i muzyka: Freesound, OpenGameArt (CC0/CC-BY, autorzy w napisach).

**Własne:** chata Halta, Redmont i ruiny złożone z modułów Megascans skryptami, interfejs na pergaminie, ikony.

---

## 9. Wydajność (Radeon 890M)

| Element | Ustawienie domyślne „Przenośny” |
|---|---|
| Rozdzielczość | 1080p wyjściowo, renderowanie ok. 720p, upscaling TSR (albo FSR, jeśli dostępny) |
| Oświetlenie | Lumen programowy, niska jakość; cienie Virtual Shadow Maps albo kaskadowe - zależnie od pomiarów |
| Geometria | Nanite dla skał, budynków i roślinności |
| Postacie | MetaHuman z niższymi LOD, maks. ok. 12 postaci naraz |
| Mgła | wolumetryczna w niskiej rozdzielczości, mocniejsza tylko w wybranych krainach |
| Zasięg | World Partition z ograniczonym zasięgiem wczytywania, HLOD dla dalekich gór |
| Cel | **30+ kl./s** w każdej krainie; mocne PC dostają preset „Wysoki” |

Pomiar kl./s robię po każdym etapie w tych samych kadrach co zrzuty.

---

## 10. Repozytorium

`https://github.com/mbialkowski/zwiadowcy-ue` (publiczne):

| W gicie | Poza gitem |
|---|---|
| `Source/` (C++), `Config/`, `Scripts/` (Python), `Content/Data/` (JSON, CSV), `docs/` | `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/` |
| własne `Content/**` (mapy, Blueprinty, materiały) przez Git LFS | `Content/Fab/` - paczki z Fab (licencja zabrania publikacji), lista w `docs/assety.md` |
| assety CC0 przez Git LFS (pilnujemy limitu 1 GB) | duże pliki źródłowe (HDRI w 8K itp.) |

---

## 11. Etapy prac

| Etap | Zawartość | Wynik |
|---|---|---|
| **E0. Przygotowanie** | instalacja UE i VS, projekt C++, Git LFS, skrypty kompilacji i testów, lista assetów Fab | pusty projekt kompiluje się i uruchamia |
| **E1. Pionowy wycinek: las Halta** | kawałek lasu z chatą w docelowej oprawie, Will z łukiem, skradanie, 3-4 Wargale, ognisko, misja 2 (szkolenie), menu, zapis | grywalne 15-20 min, pomiar kl./s na 890M |
| **E2. Świat** | cały teren 2×2 km, krainy, drogi, Redmont, ogniska, punkty widokowe | można przejść i przejechać cały świat |
| **E3. Misje 1-5** | Dzień Wyboru, chata, Wyrwij, dzik, Zlot; dialogi, przerywniki | początek gry od menu do Zlotu |
| **E4. Misje 6-8 i epilog** | spalona osada, Równina Samotności, Kalkarowie, król; obozy, kurhany, zapiski | cała gra |
| **E5. Szlif i wydanie** | balans, wydajność, angielski, napisy końcowe, paczka 1.0 | wydanie na GitHubie |

Po każdym etapie: testy automatyczne, zrzuty, pomiar kl./s i wpis w `docs/weryfikacja.md`, jak przy tomie II moda.

---

## 12. Co musisz zrobić Ty

Tylko kroki wymagające Twojego konta (do każdego dostaniesz dokładną instrukcję ze zrzutami):

1. **Unreal Engine:** Epic Games Launcher → Unreal Engine → Library → „+” → najnowsza 5.x → Install (ok. 60-100 GB).
2. **Visual Studio 2022 Community:** z obciążeniem „Game development with C++” (ok. 20 GB).
3. **Fab:** dodanie do biblioteki darmowych paczek z mojej listy (kilka kliknięć na pozycję).
4. Ewentualnie **MetaHuman:** zalogowanie się w edytorze przy tworzeniu postaci.

Resztę - projekt, kod, świat, testy, zrzuty, paczki i commity - robię ja.

---

## 13. Ryzyka

| Ryzyko | Skutek | Zabezpieczenie |
|---|---|---|
| Słaby sprzęt (iGPU, 16 GB RAM) do pracy w edytorze | długie kompilacje shaderów, braki pamięci | Derived Data Cache na dysku, praca w trybie bez okna, ograniczenie MetaHumanów |
| Świat 2×2 km budowany wyłącznie skryptami | „automatyczny” wygląd | PCG z ręcznie ustawionymi regułami, przegląd zrzutów, poprawki kadr po kadrze |
| MetaHuman wymaga kroków w interfejsie | trudność automatyzacji | gotowe presety MetaHuman, w razie potrzeby postacie z Fab |
| Brak gotowych modeli Wargali/Kalkarów | gorsze dopasowanie do książki | przeróbka stworów z Fab (materiały, skala, akcesoria) |
| Limit 1 GB LFS i licencja Fab | zablokowany push, naruszenie licencji | paczki Fab poza gitem, monitorowanie rozmiaru LFS |
| Limit 2 GB na plik w wydaniach GitHuba | nie da się wrzucić gry jednym plikiem | zip w częściach + instrukcja łączenia |
| Zmiany w nowych wersjach UE | niekompatybilne API | wersja silnika przypięta w projekcie, aktualizacje tylko świadomie |
