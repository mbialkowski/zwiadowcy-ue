# Assety z Fab

Paczki z Fab są darmowe, ale **nie trafiają do repozytorium**: licencja Fab Standard zabrania publikowania
źródłowych plików assetów. W gicie są tylko własne katalogi `Content/Zwiadowcy/` i `Content/Data/` (reszta `Content/`
jest w `.gitignore`). Po sklonowaniu repozytorium trzeba więc dodać paczki z tej listy do własnej biblioteki Fab.

Każdą pozycję sprawdziłem na fab.com 14.11.2026: cena, wydawca, zgodność z UE 5.8 i sposób dystrybucji.

> Postacie z Paragona: Epic pozwala ich używać w projektach UE, ale nazwy PARAGON nie wolno używać w nazwie
> ani reklamie gry. Używamy tylko modeli i animacji.

## Partia 1 - pionowy wycinek „las Halta” (E1)

| # | Paczka | Wydawca | Rodzaj | UE | Do czego |
|---|---|---|---|---|---|
| 1 | [Game Animation Sample](https://www.fab.com/listings/880e319a-a59e-4ed2-b268-b32dac7fa016) | Epic Games | kompletny projekt | 5.4-5.8 | ruch Willa: motion matching, 500+ animacji, przeszkody |
| 2 | [Medieval Village Megascans Sample](https://www.fab.com/listings/2e11a225-a6ea-4781-a3e1-fe975b461894) | Quixel Megascans | kompletny projekt | 5.3-5.8 | chata Halta, Redmont: domy, rekwizyty, materiały gruntu |
| 3 | [Paragon: Sparrow](https://www.fab.com/listings/7d76ddf0-d9ce-4d00-939e-d72793534d01) | Epic Games | paczka assetów | 5.0-5.8 | łuk, strzały i animacje łuku (przeniesione na Willa) |
| 4 | [Paragon: Khaimera](https://www.fab.com/listings/e7c665c1-8c13-42f0-9152-0753008853d7) | Epic Games | paczka assetów | 5.0-5.8 | baza Wargala: model, animacje ataków |
| 5 | [Megaplants: English Oak](https://www.fab.com/listings/83642c38-7661-4df1-8629-0422e1898d26) | Quixel | paczka assetów | 5.7-5.8 | dęby w lesie Halta |
| 6 | [Megaplants: European Beech](https://www.fab.com/listings/cefe5722-9c31-4aa2-9ee2-e5426610d5e6) | Quixel | paczka assetów | 5.7-5.8 | buki |
| 7 | [Megaplants: Norway Spruce](https://www.fab.com/listings/f87364c9-10a2-4834-8566-a0291c717758) | Quixel | paczka assetów | 5.8 | świerki (ciemniejsze partie lasu) |
| 8 | [Megaplants: Bracken Fern](https://www.fab.com/listings/4d60e635-7a49-4f19-818c-d018046be39f) | Quixel | paczka assetów | 5.8 | paprocie - runo i kryjówki przy skradaniu |

Wszystkie pozycje są **darmowe** (licencja standardowa Fab). Dokładny rozmiar Launcher pokazuje przed pobraniem.
Szacunkowo cała partia to ok. 15-20 GB na dysku (na dysku C: jest ok. 290 GB wolnego).

**Uwagi techniczne:**

- Megaplants to drzewa z Nanite Foliage i Procedural Vegetation Editor. Obie funkcje są w UE 5.8 eksperymentalne.
  Po pobraniu włączę wtyczkę `ProceduralVegetationEditor` i `r.Nanite.Foliage`. Jeśli wydajność na 890M albo
  stabilność będą słabe, zamienię je na klasyczne drzewa Megascans, np.
  [European Beech](https://www.fab.com/listings/d11cc01d-9422-41b7-950f-416c9ce79caf) (UE 5.1-5.6).
- Kompletne projekty (1, 2) tworzy się w osobnym katalogu. Potrzebne części kopiuję skryptem do `Content/` naszego
  projektu (ścieżki `/Game/...` zostają te same, więc odwołania działają).
- Paczki assetów (3-8) Launcher dodaje bezpośrednio do projektu `ZwiadowcyUE`.

## Jak dodać paczki

1. Zaloguj się na [fab.com](https://www.fab.com) kontem Epic.
2. Na stronie każdej paczki z tabeli kliknij **„Dodaj do biblioteki”** („Add to My Library”).
3. Epic Games Launcher → **Unreal Engine → Library → Fab Library**:
   - paczki assetów (3-8): **Add to Project** → `ZwiadowcyUE` (pokaż wszystkie projekty, wersja 5.8),
   - kompletne projekty (1, 2): **Create Project** → katalog `C:\Users\<Ty>\Documents\Unreal Projects`, wersja 5.8.
4. Potem `Scripts\zw.cmd setup` skopiuje potrzebne części kompletnych projektów do `Content/`.

## Kolejne partie (do potwierdzenia przed każdym etapem)

| Etap | Czego szukam |
|---|---|
| E2 świat | skały i klify Megascans, teren (materiały ziemi, trawy), ruiny, mgła; Redmont z partii 1 |
| E3 misje 1-5 | koń (Wyrwij), dzik, MetaHuman (wtyczka w UE 5.8), stroje średniowieczne, namioty Zlotu |
| E4 misje 6-8 | Kalkarowie (duży stwór), spalone budynki, ruiny twierdzy |
| E5 | dźwięki i muzyka CC0/CC-BY (Freesound, OpenGameArt) - autorzy w napisach |
