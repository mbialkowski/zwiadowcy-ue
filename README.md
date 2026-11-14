# Zwiadowcy: Ruiny Gorlanu (Unreal Engine 5)

Gra TPP na podstawie moda do Minecrafta
[„Zwiadowcy: Ruiny Gorlanu”](https://github.com/mbialkowski/minecraft-mod-zwiadowcy): tom I, łuk zwiadowcy,
skradanie, koń Wyrwij, Wargale i Kalkarowie w jednym połączonym świecie. Oprawa nawiązuje nastrojem do mrocznego
fantasy w złotym świetle.

> **Nieoficjalna gra fanowska** na motywach „Zwiadowców” Johna Flanagana. Darmowa i niekomercyjna. Nie jest związana
> z autorem ani z wydawcami serii. Świat, postacie i nazwy należą do ich właścicieli, a fabuła jest wolną
> interpretacją „Ruin Gorlanu”.

**Stan:** etap 0 gotowy - projekt C++ kompiluje się, uruchamia i przechodzi testy. Następny etap: pionowy wycinek
„las Halta”. Plan: [docs/plan.md](docs/plan.md), weryfikacja: [docs/weryfikacja.md](docs/weryfikacja.md).

![Poligon testowy E0](docs/zrzuty/e0_poligon.jpg)

## Wymagania

- Unreal Engine **5.8** (Epic Games Launcher)
- Visual Studio 2026 albo 2022 z obciążeniem „Game development with C++”
- Git z Git LFS
- Paczki z Fab z listy w [docs/assety.md](docs/assety.md) (nie są w repozytorium - zabrania tego ich licencja)

## Uruchomienie

```bat
Scripts\zw.cmd setup      :: kopiuje manekina i prototypy z szablonu UE, włącza Git LFS
Scripts\zw.cmd build      :: kompiluje edytor
Scripts\zw.cmd py poligon :: buduje mapę testową
Scripts\zw.cmd test       :: testy automatyczne (edytor + gra)
Scripts\zw.cmd game       :: uruchamia grę
```

Inne polecenia: `editor` (otwiera edytor), `zrzut` (zrzut ekranu i pomiar kl./s), `paczka` (gra dla Windows).
Silnik jest szukany w `C:\Program Files\Epic Games\UE_5.8`; inną ścieżkę podaje się w zmiennej `ZW_UE`.

## Sterowanie (na razie)

| Akcja | Pad | Klawiatura i mysz |
|---|---|---|
| Ruch | lewa gałka | WSAD |
| Kamera | prawa gałka | mysz |
| Skok | A / ✕ | spacja |
| Sprint | wciśnięcie lewej gałki | lewy Shift |
| Kucanie | B / ○ | lewy Ctrl albo C |

## Autorzy

ABialas & 🤖CC
