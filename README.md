# Mars Colony Life Support Simulator

A text-based survival game written in C. You are an engineer stranded alone at a small research station on Mars. Manage your health, energy, oxygen, water, spare parts and module damage with single-character commands and survive **15 days** until the rescue team arrives.

This project was developed for the *Algorithms and Programming* course. It uses only core C constructs: `switch-case`, `if / else if / else`, `for`, `do-while`, arithmetic and logical operators, and `rand()`.

> **Note:** The in-game interface is in Turkish (without special characters, for terminal compatibility), as required by the course assignment. Command letters are Turkish mnemonics (e.g. `K` = *Kolay* / Easy).

## Table of Contents

- [Features](#features)
- [Build and Run](#build-and-run)
- [Gameplay](#gameplay)
- [Commands](#commands)
- [Difficulty Levels](#difficulty-levels)
- [Time, Winning and Losing](#time-winning-and-losing)
- [Sample Session](#sample-session)
- [How the C Constructs Are Used](#how-the-c-constructs-are-used)
- [Design Decisions](#design-decisions)
- [Türkçe Özet](#türkçe-özet)

## Features

- 8 single-character commands, case-insensitive (`g` and `G` behave the same).
- Three difficulty levels that change starting resources and event thresholds.
- Randomized events (`rand() % 100`); every roll and its threshold is printed, e.g. `Zar: 37 / Esik: 70 -> SU BULUNDU!`.
- A sand storm simulated with a `for` loop, using `break` and `continue`.
- An airlock password mini-game using a `do-while` loop with limited attempts.
- A final report with survived days, total turns, final values and a score.

## Build and Run

You need a C compiler such as `gcc` or `clang`.

```bash
gcc -o mars mars_simulator.c
./mars
```

On Windows (MinGW), run `mars.exe` instead of `./mars`.

## Gameplay

You track eight values: **health**, **energy**, **oxygen** (all 0–100), **water**, **spare parts**, **module damage** (0–100, higher is worse), **airlock sealed (0/1)** and the **day / turn counters**.

The game ends when you quit with `Q`, when your health or oxygen reaches 0, or when you complete day 15.

## Commands

| Key | Command | What it does |
|-----|---------|--------------|
| `G` | Exploration (*Kesif*) | Costs 15 energy and 10 oxygen. You may get injured, and you may find water (+12), a spare part (+1) or nothing. |
| `O` | Produce oxygen (*Oksijen uret*) | Converts water into oxygen: every 2 water become 3 oxygen. If you enter an odd amount, 1 unit stays in storage. Costs 10 energy. |
| `T` | Repair (*Tamir*) | Spends spare parts to reduce module damage. The tier depends on the damage level (see below). |
| `D` | Status report (*Durum raporu*) | Shows all values and critical warnings. Does **not** use a turn. |
| `U` | Rest (*Uyu / dinlen*) | +30 energy, −5 oxygen. Health +10, but if oxygen is below 20 health decreases by 5 instead. |
| `K` | Sand storm (*Kum firtinasi*) | Runs a storm of several rounds; each round may hurt you or be harmless. |
| `H` | Airlock password (*Hava kilidi sifresi*) | Guess the one-letter password in 3 attempts to seal the airlock. |
| `Q` | Quit (*Cikis*) | Ends the game and prints the final report. |

An undefined command prints a warning and does **not** use a turn.

**Repair tiers (`T`):**

| Module damage | Tier | Spare parts used | Damage repaired |
|---------------|------|------------------|-----------------|
| 0–30 | Light | 1 | 20 |
| 31–70 | Medium | 2 | 35 |
| 71–100 | Heavy | 3 | 50 |

**Exploration (`G`) details:** an injury happens if `zar < 10 || modulHasari > 70`. For discoveries, the water threshold is higher when `enerji > 40 && oksijen > 30` and lower otherwise. A second roll below the threshold finds water, below the threshold + 20 finds a spare part, otherwise you return empty-handed.

**Sand storm (`K`) details:** each round rolls a number. If your health is above 50 and module damage is below 50 your protection threshold is 40, otherwise 20. The result is harmless, health loss, oxygen leak or module damage. On even-numbered rounds the wind intensifies and the damage doubles. If the airlock is sealed, oxygen leaks are skipped with `continue`. If health or oxygen reaches 0, the storm stops immediately with `break`. A summary is printed at the end.

**Airlock password (`H`) details:** the password is not fixed. It is derived from the current day: `'A' + (day % 26)`. The hint says it is the letter that many steps after `A`. Lowercase input is accepted. Each wrong guess costs 5 energy and gives a "go forward / go back" hint. A correct guess seals the airlock; failing all 3 attempts adds 20 module damage.

## Difficulty Levels

| | Easy (`K`) | Normal (`N`) | Hard (`Z`) |
|---|---|---|---|
| Health | 100 | 100 | 90 |
| Energy | 100 | 90 | 80 |
| Oxygen | 100 | 90 | 80 |
| Water | 60 | 50 | 40 |
| Spare parts | 5 | 3 | 2 |
| Module damage | 0 | 10 | 20 |
| Storm rounds | 3 | 5 | 7 |
| Storm base damage | 6 | 8 | 10 |
| Water threshold (good / bad state) | 70 / 40 | 60 / 30 | 50 / 25 |
| Injury health loss | 8 | 10 | 12 |

An invalid difficulty input asks again.

## Time, Winning and Losing

- Every valid command uses one turn, except `D` and undefined commands. A command cancelled because of insufficient resources still counts as a turn.
- Every 3 turns (`tur % 3 == 0`) a new day starts: oxygen decreases by 8, and if module damage is above 70 you also lose 5 health.
- **Win:** you complete day 15 (45 turns) and the rescue team arrives.
- **Lose:** health or oxygen reaches 0. The check runs at the end of every turn.
- **Score:** `(day × 100) + health + oxygen − module damage`.

## Sample Session

```text
=== MARS KOLONISI YASAM DESTEK SIMULATORU ===
Hedef: Kurtarma ekibi gelene kadar 15 gun hayatta kal.

Zorluk seviyesi sec (K: Kolay, N: Normal, Z: Zor): n
Normal seviye secildi.

Komutlar: G:Kesif  O:Oksijen uret  T:Tamir  D:Durum raporu
          U:Uyu/dinlen  K:Kum firtinasi  H:Hava kilidi sifresi  Q:Cikis

[Gun: 1 | Tur: 0] Komut: h
[H] Hava kilidi sifresi...
IPUCU: Sifre, alfabede A harfinden bugunun gun sayisi (1) kadar ilerideki harftir.
Tek harf gir (A-Z), 3 deneme hakkin var.
Deneme 1/3 - Sifre: b
DOGRU SIFRE! Hava kilidi muhurlendi.

[Gun: 1 | Tur: 1] Komut: d
--- DURUM RAPORU ---
Saglik: 100 | Enerji: 90 | Oksijen: 90
Su: 50 | Yedek parca: 3 | Modul hasari: 10
Hava kilidi: MUHURLU | Gun: 1 | Tur: 1
TUM SISTEMLER NORMAL

[Gun: 1 | Tur: 1] Komut: q
Cikis yapiliyor...

==============================
Simulasyondan cikildi.
--- SONUC RAPORU ---
Hayatta kalinan gun: 1
Toplam tur: 1
Saglik: 100 | Enerji: 90 | Oksijen: 90
Su: 50 | Yedek parca: 3 | Modul hasari: 10
PUAN: 280
==============================
```

## How the C Constructs Are Used

| Construct | Where it is used | Why |
|---|---|---|
| `switch-case` with fall-through | Difficulty selection and the command system (`case 'g': case 'G':`) | Handles upper and lower case with one block and keeps commands readable. `default` rejects unknown input. |
| `if / else if / else` | Repair tiers, exploration outcomes, storm events, nested checks while resting, clamping, game-over checks | Multi-way decisions that depend on current values. |
| `for` loop | Sand storm rounds | The number of rounds is known in advance (set by difficulty). `break` ends it early, `continue` skips a sealed-airlock leak. |
| `do-while` loop | Main command loop, difficulty selection, airlock password, water amount input | These blocks must run at least once before checking the condition. |
| `%` (modulo) | Even/odd water in oxygen production, even storm rounds, day change (`tur % 3`), password formula | Parity and cyclic calculations. |
| `&&`, `\|\|` | Exploration thresholds, injury condition, password loop condition, status check | Combine several conditions in one expression. |
| `rand() % 100` | Exploration and storm rolls | Random events; every roll is printed with its threshold. |

## Design Decisions

The assignment leaves some details open. These are the choices made in this implementation:

- All numeric values (difficulty table, amounts found, repair amounts, rest bonuses, storm damage) are chosen for a playable balance.
- Commands cancelled for lack of resources still use a turn, because only `D` and undefined commands are exempt. `Q` ends the game and is not counted as a turn.
- A whole `K` command counts as one game turn; the storm rounds inside it are internal to the loop.
- "Completing day 15" means the day counter passes 15 (45 turns). The day-start effects of a 16th day are not applied.
- If a turn would end with both a win and a death condition, death takes priority.
- The airlock seal is permanent once achieved.
- `T`, `U` and `K` do not use energy because the assignment does not specify an energy cost for them.

## Türkçe Özet

### Proje Hakkında

Bu proje, C diliyle yazılmış metin tabanlı bir **Mars Kolonisi Yaşam Destek Simülatörü**dür. Oyuncu, Mars'taki küçük bir araştırma istasyonunda tek başına kalmış bir mühendisi yönetir ve kurtarma ekibi gelene kadar **15 gün** hayatta kalmaya çalışır. Program sağlık, enerji, oksijen, su, yedek parça, modül hasarı ve gün sayısını takip eder.

Proje *Algoritma ve Programlama* dersi ödevi kapsamında geliştirilmiştir. Yalnızca C'nin temel yapıları kullanılmıştır: `switch-case`, `if / else if / else`, `for`, `do-while`, aritmetik ve mantıksal operatörler, `rand()`.

### Derleme ve Çalıştırma

```bash
gcc -o mars mars_simulator.c
./mars
```

### Komutlar

| Tuş | Komut | Açıklama |
|-----|-------|----------|
| `G` | Keşif | Enerji 15, oksijen 10 azalır. Su, yedek parça bulunabilir ya da yaralanılabilir. |
| `O` | Oksijen üret | 2 su = 3 oksijen. Tek sayıda su girilirse 1 birim artık kalır (mod operatörü). |
| `T` | Tamir | Yedek parça harcanarak modül hasarı azaltılır (hafif / orta / ağır kademe). |
| `D` | Durum raporu | Tüm değerleri ve kritik uyarıları gösterir. Tur harcamaz. |
| `U` | Uyu / dinlen | Enerji ve sağlık artar; oksijen 20'nin altındaysa sağlık azalır. |
| `K` | Kum fırtınası | `for` döngüsüyle simüle edilir; `break` ve `continue` kullanılır. |
| `H` | Hava kilidi şifresi | `do-while` döngüsüyle, 3 denemelik şifre mekanizması. |
| `Q` | Çıkış | Oyunu bitirir ve sonuç raporunu yazar. |

Komutlar büyük/küçük harf duyarsızdır. Tanımsız komut uyarı verir ve tur harcamaz.

### Oyunun Akışı

- `D` ve tanımsız komutlar dışındaki her geçerli komut 1 tur harcar. Her 3 turda bir yeni gün başlar; oksijen 8 azalır, modül hasarı 70'in üzerindeyse sağlık 5 azalır.
- Sağlık veya oksijen 0'a düşerse oyun kaybedilir; 15. gün tamamlanırsa kurtarma ekibi gelir ve oyun kazanılır.
- Oyun bitince hayatta kalınan gün, toplam tur, son değerler ve puan yazdırılır.
- Puan: `(gün × 100) + sağlık + oksijen − modül hasarı`.


