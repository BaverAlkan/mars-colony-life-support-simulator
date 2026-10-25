# Mars Colony Life Support Simulator

A text-based survival game written in C. You are an engineer stranded alone at a small research station on Mars. Manage your health, energy, oxygen, water, spare parts and module damage with single-character commands and survive **15 days** until the rescue team arrives.

The project uses only core C constructs and no external libraries: `switch-case`, `if / else if / else`, `for`, `do-while`, arithmetic and logical operators, and `rand()`.

> **Türkçe sürüm için:** bu belgenin tam Türkçe çevirisi [aşağıdadır](#türkçe-sürüm).

> **Note:** The in-game interface is in Turkish (written without special characters for terminal compatibility). Command letters are Turkish mnemonics (e.g. `K` = *Kolay* / Easy).

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
- [Türkçe Sürüm (Turkish Version)](#türkçe-sürüm)

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

Some mechanics can be implemented in more than one way. These are the choices made in this implementation:

- All numeric values (difficulty table, amounts found, repair amounts, rest bonuses, storm damage) are chosen for a playable balance.
- Commands cancelled for lack of resources still use a turn, because only `D` and undefined commands are exempt. `Q` ends the game and is not counted as a turn.
- A whole `K` command counts as one game turn; the storm rounds inside it are internal to the loop.
- "Completing day 15" means the day counter passes 15 (45 turns). The day-start effects of a 16th day are not applied.
- If a turn would end with both a win and a death condition, death takes priority.
- The airlock seal is permanent once achieved.
- `T`, `U` and `K` do not consume energy by design: repairs use spare parts, resting restores energy, and the storm is an environmental event.

## Türkçe Sürüm

**Mars Kolonisi Yaşam Destek Simülatörü**

C ile yazılmış, metin tabanlı bir hayatta kalma oyunu. Mars'taki küçük bir araştırma istasyonunda tek başına kalmış bir mühendissin. Tek karakterli komutlarla **sağlık**, **enerji**, **oksijen**, **su**, **yedek parça** ve **modül hasarını** yönet ve kurtarma ekibi gelene kadar **15 gün** hayatta kal.

Proje yalnızca C'nin temel yapılarını kullanır ve harici kütüphane gerektirmez: `switch-case`, `if / else if / else`, `for`, `do-while`, aritmetik ve mantıksal operatörler ve `rand()`.

> **Not:** Oyun arayüzü Türkçedir (terminal uyumu için Türkçe özel karakterler kullanılmamıştır). Komut harfleri Türkçe kısaltmalardır (örn. `K` = *Kolay*).

### İçindekiler

- [Özellikler](#özellikler)
- [Derleme ve Çalıştırma](#derleme-ve-çalıştırma)
- [Oynanış](#oynanış)
- [Komutlar](#komutlar)
- [Zorluk Seviyeleri](#zorluk-seviyeleri)
- [Zaman, Kazanma ve Kaybetme](#zaman-kazanma-ve-kaybetme)
- [Örnek Oturum](#örnek-oturum)
- [C Yapıları Nasıl Kullanıldı](#c-yapıları-nasıl-kullanıldı)
- [Tasarım Kararları](#tasarım-kararları)

### Özellikler

- Büyük/küçük harf duyarsız 8 tek karakterli komut (`g` ve `G` aynı işlemi yapar).
- Başlangıç kaynaklarını ve olay eşiklerini değiştiren üç zorluk seviyesi.
- Rastgele olaylar (`rand() % 100`); her zar ve karşılaştırıldığı eşik ekrana yazılır, örn. `Zar: 37 / Esik: 70 -> SU BULUNDU!`.
- `for` döngüsüyle simüle edilen, `break` ve `continue` kullanan bir kum fırtınası.
- `do-while` döngüsüyle ve sınırlı denemeyle çalışan hava kilidi şifre mekanizması.
- Hayatta kalınan gün, toplam tur, son değerler ve puanı gösteren bir sonuç raporu.

### Derleme ve Çalıştırma

`gcc` veya `clang` gibi bir C derleyicisine ihtiyacın var.

```bash
gcc -o mars mars_simulator.c
./mars
```

Windows'ta (MinGW) `./mars` yerine `mars.exe` çalıştırılır.

### Oynanış

Sekiz değeri takip edersin: **sağlık**, **enerji**, **oksijen** (hepsi 0–100), **su**, **yedek parça**, **modül hasarı** (0–100, yüksek olması kötüdür), **hava kilidi mühürlü mü (0/1)** ve **gün / tur sayaçları**.

Oyun; `Q` ile çıkarsan, sağlığın veya oksijenin 0'a düşerse ya da 15. günü tamamlarsan sona erer.

### Komutlar

| Tuş | Komut | Ne yapar |
|-----|-------|----------|
| `G` | Keşif (*Kesif*) | 15 enerji ve 10 oksijen harcar. Yaralanabilirsin; su (+12), yedek parça (+1) bulabilir veya eli boş dönebilirsin. |
| `O` | Oksijen üret (*Oksijen uret*) | Suyu oksijene çevirir: her 2 su 3 oksijen olur. Tek sayıda su girersen 1 birim depoda kalır. 10 enerji harcar. |
| `T` | Tamir (*Tamir*) | Yedek parça harcayarak modül hasarını azaltır. Kademe, hasar seviyesine bağlıdır (aşağıya bak). |
| `D` | Durum raporu (*Durum raporu*) | Tüm değerleri ve kritik uyarıları gösterir. Tur **harcamaz**. |
| `U` | Uyu / dinlen (*Uyu / dinlen*) | +30 enerji, −5 oksijen. Sağlık +10 artar; ancak oksijen 20'nin altındaysa sağlık bunun yerine 5 azalır. |
| `K` | Kum fırtınası (*Kum firtinasi*) | Birkaç turluk bir fırtına çalıştırır; her tur sana zarar verebilir veya zararsız geçebilir. |
| `H` | Hava kilidi şifresi (*Hava kilidi sifresi*) | Tek harflik şifreyi 3 denemede tahmin ederek hava kilidini mühürle. |
| `Q` | Çıkış (*Cikis*) | Oyunu bitirir ve sonuç raporunu yazar. |

Tanımsız bir komut uyarı verir ve tur **harcamaz**.

**Tamir kademeleri (`T`):**

| Modül hasarı | Kademe | Harcanan yedek parça | Onarılan hasar |
|--------------|--------|----------------------|----------------|
| 0–30 | Hafif | 1 | 20 |
| 31–70 | Orta | 2 | 35 |
| 71–100 | Ağır | 3 | 50 |

**Keşif (`G`) ayrıntıları:** `zar < 10 || modulHasari > 70` koşulu sağlanırsa yaralanırsın. Keşiflerde su bulma eşiği, `enerji > 40 && oksijen > 30` ise daha yüksek, değilse daha düşüktür. Eşiğin altında kalan ikinci bir zar su bulur, eşik + 20'nin altında kalan yedek parça bulur, aksi halde eli boş dönersin.

**Kum fırtınası (`K`) ayrıntıları:** Her turda bir sayı üretilir. Sağlığın 50'nin üzerindeyse ve modül hasarı 50'nin altındaysa korunma eşiğin 40'tır, değilse 20'dir. Sonuç; zararsız geçme, sağlık kaybı, oksijen sızıntısı veya modül hasarı olabilir. Çift numaralı turlarda rüzgar şiddetlenir ve hasar iki katına çıkar. Hava kilidi mühürlüyse oksijen sızıntıları `continue` ile atlanır. Sağlık veya oksijen 0'a düşerse fırtına `break` ile hemen sonlandırılır. Sonunda bir özet yazdırılır.

**Hava kilidi şifresi (`H`) ayrıntıları:** Şifre sabit değildir, o anki günden türetilir: `'A' + (gün % 26)`. İpucu, şifrenin `A` harfinden bugünün gün sayısı kadar ilerideki harf olduğunu söyler. Küçük harf girişi de kabul edilir. Her yanlış tahmin 5 enerji harcar ve "ileri git / geri git" ipucu verir. Doğru tahmin hava kilidini mühürler; 3 denemenin hepsi başarısız olursa modül hasarı 20 artar.

### Zorluk Seviyeleri

| | Kolay (`K`) | Normal (`N`) | Zor (`Z`) |
|---|---|---|---|
| Sağlık | 100 | 100 | 90 |
| Enerji | 100 | 90 | 80 |
| Oksijen | 100 | 90 | 80 |
| Su | 60 | 50 | 40 |
| Yedek parça | 5 | 3 | 2 |
| Modül hasarı | 0 | 10 | 20 |
| Fırtına turu | 3 | 5 | 7 |
| Fırtına temel hasarı | 6 | 8 | 10 |
| Su eşiği (iyi / kötü durum) | 70 / 40 | 60 / 30 | 50 / 25 |
| Yaralanmada sağlık kaybı | 8 | 10 | 12 |

Geçersiz bir zorluk girişinde seçim tekrar istenir.

### Zaman, Kazanma ve Kaybetme

- `D` ve tanımsız komutlar hariç her geçerli komut bir tur harcar. Kaynak yetersizliği yüzünden iptal edilen bir komut da tur sayılır.
- Her 3 turda bir (`tur % 3 == 0`) yeni bir gün başlar: oksijen 8 azalır, modül hasarı 70'in üzerindeyse ayrıca 5 sağlık kaybedersin.
- **Kazanma:** 15. günü tamamlarsın (45 tur) ve kurtarma ekibi gelir.
- **Kaybetme:** Sağlık veya oksijen 0'a düşer. Bu kontrol her turun sonunda yapılır.
- **Puan:** `(gün × 100) + sağlık + oksijen − modül hasarı`.

### Örnek Oturum

Örnek bir oyun çıktısı için yukarıdaki [Sample Session](#sample-session) bölümüne bakabilirsin. Oyunun ekran yazıları iki dilde de aynıdır (Türkçe), bu yüzden çıktı tekrar edilmemiştir.

### C Yapıları Nasıl Kullanıldı

| Yapı | Nerede kullanıldı | Neden |
|---|---|---|
| Fall-through'lu `switch-case` | Zorluk seçimi ve komut sistemi (`case 'g': case 'G':`) | Büyük/küçük harfi tek blokla karşılar ve komutları okunaklı tutar. `default` bilinmeyen girişi reddeder. |
| `if / else if / else` | Tamir kademeleri, keşif sonuçları, fırtına olayları, dinlenirken iç içe kontroller, sınırlandırma, oyun sonu kontrolleri | Mevcut değerlere bağlı çok yollu kararlar. |
| `for` döngüsü | Kum fırtınası turları | Tur sayısı önceden bellidir (zorluğa göre). `break` döngüyü erken bitirir, `continue` mühürlü hava kilidinde sızıntıyı atlar. |
| `do-while` döngüsü | Ana komut döngüsü, zorluk seçimi, hava kilidi şifresi, su miktarı girişi | Bu bloklar koşul kontrol edilmeden önce en az bir kez çalışmalıdır. |
| `%` (mod) | Oksijen üretiminde çift/tek su, çift fırtına turları, gün değişimi (`tur % 3`), şifre formülü | Çift/tek ve döngüsel hesaplamalar. |
| `&&`, `\|\|` | Keşif eşikleri, yaralanma koşulu, şifre döngü koşulu, durum kontrolü | Birden fazla koşulu tek ifadede birleştirir. |
| `rand() % 100` | Keşif ve fırtına zarları | Rastgele olaylar; her zar eşiğiyle birlikte yazdırılır. |

### Tasarım Kararları

Bazı mekanikler birden fazla şekilde uygulanabilir. Bu uygulamada alınan kararlar şunlardır:

- Tüm sayısal değerler (zorluk tablosu, bulunan miktarlar, onarım miktarları, dinlenme kazanımları, fırtına hasarı) oynanabilir bir denge için seçilmiştir.
- Kaynak yetersizliği yüzünden iptal edilen komutlar da tur harcar, çünkü yalnızca `D` ve tanımsız komutlar istisnadır. `Q` oyunu bitirir ve tur olarak sayılmaz.
- Bir `K` komutunun tamamı oyun açısından 1 tur sayılır; içindeki fırtına turları yalnızca döngünün iç turlarıdır.
- "15. günü tamamlamak", gün sayacının 15'i geçmesi demektir (45 tur). 16. günün gün başı etkileri uygulanmaz.
- Bir turun sonunda hem kazanma hem ölüm koşulu oluşursa ölüm önceliklidir.
- Hava kilidi mührü bir kez sağlandıktan sonra kalıcıdır.
- `T`, `U` ve `K` komutları tasarım gereği enerji harcamaz: tamir yedek parça kullanır, dinlenme enerjiyi geri kazandırır, fırtına ise bir çevre olayıdır.
