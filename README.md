# Math Blaster: The Equation Chronicles

> *„Když matematika ožije, symbolem se stává tvá zbraň.“*  

---

## Základní idea

**Math Blaster** je logicko-strategická hra propojující **matematiku, historii a karetní mechaniky**.  
Hráč používá **matematické symboly jako karty**, kterými řeší rovnice, algebraické výrazy a logické výroky.  
Každý level představuje **epochu dějin matematiky** – a v ní slavného matematika, který hráči předá nové symboly a poznání.  

---

## Herní princip

- Tahová karetní hra: každé kolo se hráčovi rozdá sada karet se symboly (např. `+`, `−`, `×`, `÷`, `^`, `√`, `log`, `sin` …).  
- Cílem je **doplňovat rovnice** (např. `3 [] 6 = 9`) správnými symboly.  
- Každá karta má **cenu (mana)**, **efekt (sílu)** a **typ (aritmetická, funkční, logická)**.  
- Hráč má omezený počet tahů a musí **přemýšlet strategicky**, jak své karty použít.  
- Po vyřešení všech rovnic přechází do další epochy – s novými symboly a složitější logikou.

---

## Herní flow

1. **Začátek levelu:**  
   - Zobrazí se mentor (např. Al-Chvárizmí, Descartes…).  
   - Hráč dostane 5 karet a určitý počet many.  
   - Na obrazovce jsou 3–5 rovnic.

2. **Tah hráče:**  
   - Vybere kartu a použije ji do `[]` v rovnici.  
   - Hra výraz vyhodnotí a porovná výsledek:  
     - *Správně:* rovnice se vyřeší.  
     - *Přesně / silně:* způsobí “overkill” (bonus).  
     - *Špatně:* rovnice se „brání“ nebo přidá penalizaci.

3. **Konec kola:**  
   - Pokud jsou všechny rovnice vyřešeny → hráč získává nové karty.  
   - Pokud dojde mana nebo tahy → konec kola / prohra.

4. **Boss fight:**  
   - Boss (např. „Algebrion“) má 100 HP.  
   - Rovnice působí damage podle výsledku (`3 × 6 = 18` → boss −18 HP).  
   - Silnější operace (`^`, `√`) mají vyšší cenu many, ale i větší účinek.  
   - Cílem je porazit bosse v co nejmenším počtu tahů.

---

## Historické epochy

| Éra | Mentor | Nové symboly / koncepty | Styl a téma |
|------|---------|---------------------------|--------------|
| **Antika** | Pythagoras / Diogénes | `+`, `−` | Řecké vzorce, harmonie čísel |
| **Arabská éra** | Al-Chvárizmí | `×`, `÷`, `=` | Pergameny, algoritmy |
| **Renesance** | Descartes | `x`, `y`, grafické zobrazení | Kartézská rovina |
| **Novověk** | Euler / Gauss | `√`, `sin`, `f(x)` | Formální funkce |
| **19.–20. století** | Boole / Gödel / Turing | `∧`, `∨`, `¬`, `→` | Logické systémy |
| **Moderní éra** | Hilbert / Cantor / Noetherová | `∞`, `∪`, `⊂` | Abstraktní struktury a množiny |

Každý mentor hráči vysvětlí nové symboly a nabídne výzvu, která odpovídá jeho oboru.

---

## Karetní systém

Každá karta má atributy:

| Parametr | Popis |
|-----------|--------|
| **Symbol** | Znak operace (`+`, `×`, `^`, `∨`...) |
| **Typ** | Aritmetická / Algebraická / Logická |
| **Cena (Mana)** | Kolik stojí zahrání (např. 1–5) |
| **Síla / Efekt** | Kolik HP ubere rovnice / bossovi |
| **Rarita** | Běžná / Vzácná / Legendární (např. `^`, `√`) |

---

## Boss systém

Boss má vlastní:
- **HP** (např. 100)  
- **Slabiny** (např. „odolný vůči +, slabý proti ×“)  
- **Speciální efekty** (může měnit rovnice, přidávat překážky)  

Vyhodnocení příkladu:
```
3 [] 6 = 9
  ↑
  karta: ×  (cena 2, síla 18)
→ damage: 18
→ boss HP: 82
```

---

## Technická architektura

**Jazyk:** C++  
**Knihovna:** SFML (Simple and Fast Multimedia Library)  
**Cíl:** Desktop (Windows, Linux)

## Struktura souborů:
```
src/
 ├── main.cpp
 ├── Game.cpp / Game.h
 ├── Player.cpp / Player.h
 ├── Card.cpp / Card.h
 ├── Equation.cpp / Equation.h
 ├── Boss.cpp / Boss.h
 ├── Level.cpp / Level.h
 └── UI.cpp / UI.h
assets/
 ├── fonts/
 ├── music/
 ├── textures/
 └── levels/
      ├── level1.json
      ├── level2.json
      └── boss1.json
```

## Základní třídy:
- `Game` – hlavní smyčka, správa stavu hry  
- `Player` – informace o ruce, maně a kartách  
- `Card` – jednotlivé symboly a efekty  
- `Equation` – objekt s rovnicí a logikou vyhodnocení  
- `Boss` – objekt s HP, slabinami a reakcemi  
- `UI` – texty, mana, skóre, vizuální rozhraní  

---

## Vývojový plán

| Fáze | Popis | Cíl |
|------|--------|-----|
| I. Koncept | Dokončení návrhu (flow, mechaniky, objekty) | Kompletní dokumentace |
| II. Engine Setup | Základní SFML engine, okno, vstupy | Spustitelný prázdný loop |
| III. Aritmetika | Rovnice `3 [] 6 = 9`, základní evaluace | První hratelný prototyp |
| IV. Karetní systém | Přidat karty, manu a ruce | Hratelná logická tahová verze |
| V. Boss fight | HP systém, damage, animace | Finální prototyp |
| VI. Mentors & Lore | Dialogy, odemykání symbolů, zvuk | Prezentovatelná verze |

---

## Atmosféra & styl

- **Vizuál:** staré rukopisy, pergameny, křídové tabule.  
- **Hudba:** ambientní – antické struny, arabské melodie, cembalo, elektronický logický ambient.  
- **UI styl:** jednoduchý, kontrastní, připomínající učebnici.  

---

## Cíl projektu
> Vytvořit vzdělávací, esteticky působivou a herně zajímavou hru,  
> která propojí matematické principy, historické myšlení a logickou strategii.  
>  
> Hráč se neučí vzorce – učí se *přemýšlet jako matematik.*

---

## Autoři projektu
- **Design a vývoj:** Ondřej Škubala a Jan Souhrada  
- **Konzultace:** kolegové a studenti  
- **Technologie:** C++, SFML  
- **Rok:** 2025 

---

### Dev log
*Initial Git learning test.*
