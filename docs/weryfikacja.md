# Weryfikacja etapów

Po każdym etapie: testy automatyczne, zrzuty, pomiar kl./s i sprawdzenie założeń z [planu](plan.md).

## E0. Przygotowanie (14.11.2026)

**Środowisko:** Unreal Engine 5.8.4, Visual Studio Community 2026 (MSVC 14.52 - nowszy niż wersje zalecane przez
UE 5.8, kompiluje bez błędów, jedno ostrzeżenie C4864 w nagłówku silnika), Windows SDK 10.0.26100, Git LFS 3.7.

| Założenie z planu | Stan | Jak sprawdzone |
|---|---|---|
| Najnowszy UE 5.x, wersja zapisana w projekcie (dec. 1) | ✅ | `ZwiadowcyUE.uproject`: `EngineAssociation` 5.8 |
| Logika w C++, moduły `Zwiadowcy` + `ZwiadowcyEditor` (dec. 2) | ✅ | kompilacja `Scripts\zw.cmd build` |
| Pad pierwszy, mysz i klawiatura też (dec. 11) | ✅ | akcje Enhanced Input budowane w kodzie; test `Zwiadowcy.Postac.Sterowanie` wymaga klawisza na padzie i na klawiaturze/myszy dla każdej akcji |
| Dane jako JSON w `Content/Data` (2.3) | ✅ | `lokacje.json` (8 miejsc z planu); test `Zwiadowcy.Dane.Lokacje`; katalog kopiowany do paczki jako zwykłe pliki |
| Praca bez klikania w edytorze (2.2, dec. 28) | ✅ | `Scripts\zw.cmd`: setup, build, py, test, zrzut, game, editor, paczka; mapa poligonu budowana skryptem `Scripts/py/poligon.py` |
| Testy automatyczne (2.2) | ✅ | 3 testy w edytorze + 4 w grze, w tym `Zwiadowcy.Swiat.Poligon`: Will staje na ziemi i przechodzi > 1,5 m |
| Tryb zrzutów z pomiarem kl./s (2.2, rozdz. 9) | ✅ | `Scripts\zw.cmd zrzut`: [zrzut](zrzuty/e0_poligon.jpg), raport w `Saved/Zrzuty/raport.txt` |
| Lumen programowy, TSR, Nanite (rozdz. 9) | ✅ | `Config/DefaultEngine.ini` |
| Paczki Fab poza gitem (dec. 5) | ✅ | `.gitignore`: w `Content/` tylko `Zwiadowcy/` i `Data/` |
| Lista assetów Fab (E0) | ✅ | [assety.md](assety.md): 8 pozycji partii 1, każda sprawdzona na fab.com (darmowa, zgodna z UE 5.8) |
| Złote, nisko stojące słońce (dec. 21) | ✅ | poligon: słońce 14° nad horyzontem, 4300 K, mgła wolumetryczna, winieta i ziarno |

**Pomiar (Radeon 890M, 1920×1080, ustawienia domyślne):** poligon - średnio **52,8 kl./s**, 1% najgorszych klatek
43,0 kl./s. To pusta scena, więc wynik jest tylko punktem odniesienia przed lasem w E1.

![Poligon E0](zrzuty/e0_poligon.jpg)

**Do zrobienia w E1:** paczki z partii 1, las Halta, Will z łukiem i animacjami GASP, skradanie, Wargale, ognisko,
misja 2, menu i zapis.
