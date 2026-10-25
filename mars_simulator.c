/*
 * MARS KOLONISI YASAM DESTEK SIMULATORU
 * Metin tabanli, tek karakterli komutlarla kontrol edilen yasam destek oyunu.
 *
 * Komutlar:
 *   G: Kesif    O: Oksijen uret   T: Tamir        D: Durum raporu
 *   U: Dinlen   K: Kum firtinasi  H: Hava kilidi  Q: Cikis
 *
 * Kullanilan yapilar:
 *   - switch-case (zorluk secimi ve komut sistemi, fall-through ile buyuk/kucuk harf)
 *   - if / else if / else (kademeli kararlar, sinirlandirma, oyun sonu)
 *   - for dongusu (kum firtinasi, break ve continue ile)
 *   - do-while dongusu (ana komut dongusu, hava kilidi sifresi)
 *   - aritmetik (+ - * / %) ve mantiksal (&& || !) operatorler
 *   - rand() % 100 ile rastgelelik (srand(time(NULL)))
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEDEF_GUN 15              /* Bu gun tamamlaninca oyun kazanilir   */
#define GUN_TUR_SAYISI 3          /* Her 3 turda bir yeni gun baslar      */
#define GUN_BASI_OKSIJEN_KAYBI 8  /* Gun basinda oksijen kaybi            */
#define GUN_BASI_SAGLIK_KAYBI 5   /* Hasar > 70 ise gun basinda saglik kaybi */

int main(void)
{
    /* ---- Durum degiskenleri ---- */
    int saglik, enerji, oksijen;
    int su, yedekParca, modulHasari;
    int kilitMuhurlu = 0;
    int gun = 1, tur = 0;

    /* ---- Zorluga bagli olay esikleri ---- */
    int firtinaTurSayisi, firtinaHasari;
    int suEsigiIyi, suEsigiKotu, yaralanmaKaybi;

    /* ---- Kontrol degiskenleri ---- */
    char zorluk, komut;
    int zorlukGecerli;
    int turHarcandi;
    int oyunDurumu = 0;   /* 0: devam, 1: kazanildi, 2: kaybedildi, 3: Q ile cikis */
    int temizle;
    int puan;
    int harcananParca, onarilanHasar, eskiHasar;   /* T komutu icin */
    int suMiktari, kullanilanSu, artikSu, uretilenOksijen;   /* O komutu icin */
    int okunan, suGecerli;
    int zarYaralanma, zarSonuc, suEsigi;   /* G komutu icin */
    char sifre, giris;                     /* H komutu icin */
    int deneme;
    int i, firtinaZar, korunmaEsigi, hasar, kayip;   /* K komutu icin */
    int hasarliTur, toplamSaglikKaybi, toplamOksijenKaybi, toplamModulHasari;

    srand(time(NULL));

    printf("=== MARS KOLONISI YASAM DESTEK SIMULATORU ===\n");
    printf("Hedef: Kurtarma ekibi gelene kadar %d gun hayatta kal.\n\n", HEDEF_GUN);

    /* =====================================================
     * ZORLUK SECIMI (ayri switch-case, gecersizse tekrar sorulur)
     * ===================================================== */
    do
    {
        printf("Zorluk seviyesi sec (K: Kolay, N: Normal, Z: Zor): ");
        scanf(" %c", &zorluk);
        while ((temizle = getchar()) != '\n' && temizle != EOF);

        zorlukGecerli = 1;
        switch (zorluk)
        {
            case 'k':
            case 'K':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 60; yedekParca = 5; modulHasari = 0;
                firtinaTurSayisi = 3; firtinaHasari = 6;
                suEsigiIyi = 70; suEsigiKotu = 40; yaralanmaKaybi = 8;
                printf("Kolay seviye secildi.\n");
                break;
            case 'n':
            case 'N':
                saglik = 100; enerji = 90; oksijen = 90;
                su = 50; yedekParca = 3; modulHasari = 10;
                firtinaTurSayisi = 5; firtinaHasari = 8;
                suEsigiIyi = 60; suEsigiKotu = 30; yaralanmaKaybi = 10;
                printf("Normal seviye secildi.\n");
                break;
            case 'z':
            case 'Z':
                saglik = 90; enerji = 80; oksijen = 80;
                su = 40; yedekParca = 2; modulHasari = 20;
                firtinaTurSayisi = 7; firtinaHasari = 10;
                suEsigiIyi = 50; suEsigiKotu = 25; yaralanmaKaybi = 12;
                printf("Zor seviye secildi.\n");
                break;
            default:
                printf("Gecersiz zorluk seviyesi! Lutfen K, N veya Z gir.\n\n");
                zorlukGecerli = 0;
                break;
        }
    } while (zorlukGecerli == 0);

    printf("\nKomutlar: G:Kesif  O:Oksijen uret  T:Tamir  D:Durum raporu\n");
    printf("          U:Uyu/dinlen  K:Kum firtinasi  H:Hava kilidi sifresi  Q:Cikis\n");

    /* =====================================================
     * ANA KOMUT DONGUSU (do-while)
     * Oyuncu Q der, karakter olur veya 15. gun tamamlanana kadar surer.
     * ===================================================== */
    do
    {
        printf("\n[Gun: %d | Tur: %d] Komut: ", gun, tur);
        scanf(" %c", &komut);
        while ((temizle = getchar()) != '\n' && temizle != EOF);

        turHarcandi = 0;   /* Komut tur harcarsa case icinde 1 yapilir */

        /* ---- KOMUT SISTEMI (switch-case, fall-through ile buyuk/kucuk harf) ---- */
        switch (komut)
        {
            case 'g':
            case 'G':
                printf("[G] Kesif gorevine cikiliyor...\n");

                if (enerji < 15)
                {
                    /* Yetersiz enerji: komut calistirilamaz */
                    printf("Yetersiz enerji (gereken: 15, mevcut: %d)! Komut iptal edildi.\n", enerji);
                }
                else
                {
                    enerji = enerji - 15;
                    oksijen = oksijen - 10;
                    printf("Enerji -15, Oksijen -10\n");

                    /* Yaralanma kontrolu: (zar < 10 || modulHasari > 70) */
                    zarYaralanma = rand() % 100;
                    if (zarYaralanma < 10 || modulHasari > 70)
                    {
                        saglik = saglik - yaralanmaKaybi;
                        printf("Zar: %d / Esik: 10 (modul hasari: %d) -> YARALANDIN! Saglik -%d\n",
                               zarYaralanma, modulHasari, yaralanmaKaybi);
                    }
                    else
                    {
                        printf("Zar: %d / Esik: 10 (modul hasari: %d) -> Yaralanma yok\n",
                               zarYaralanma, modulHasari);
                    }

                    /* Su bulma esigi, enerji ve oksijen durumuna gore secilir */
                    if (enerji > 40 && oksijen > 30)
                    {
                        suEsigi = suEsigiIyi;      /* Durum iyi: esik yuksek */
                    }
                    else
                    {
                        suEsigi = suEsigiKotu;     /* Durum kotu: esik dusuk */
                    }

                    /* Uc farkli sonuc: su bulma, yedek parca bulma, eli bos donme */
                    zarSonuc = rand() % 100;
                    if (zarSonuc < suEsigi)
                    {
                        su = su + 12;
                        printf("Zar: %d / Esik: %d -> SU BULUNDU! Su +12\n", zarSonuc, suEsigi);
                    }
                    else if (zarSonuc < suEsigi + 20)
                    {
                        yedekParca = yedekParca + 1;
                        printf("Zar: %d / Esik: %d -> YEDEK PARCA BULUNDU! Yedek parca +1\n",
                               zarSonuc, suEsigi + 20);
                    }
                    else
                    {
                        printf("Zar: %d / Esik: %d -> ELI BOS DONDUN\n", zarSonuc, suEsigi + 20);
                    }
                }
                turHarcandi = 1;
                break;

            case 'o':
            case 'O':
                printf("[O] Oksijen uretimi baslatiliyor...\n");

                if (enerji < 10)
                {
                    /* Yetersiz enerji: komut calistirilamaz */
                    printf("Yetersiz enerji (gereken: 10, mevcut: %d)! Komut iptal edildi.\n", enerji);
                }
                else if (su < 2)
                {
                    /* 2 birim su = 3 birim oksijen, bu yuzden en az 2 su gerekir */
                    printf("En az 2 birim su gerekli (mevcut: %d)! Komut iptal edildi.\n", su);
                }
                else
                {
                    /* Su miktarini al: 1 ile mevcut su arasinda olmali */
                    do
                    {
                        printf("Kac birim su kullanilsin? (1-%d): ", su);
                        okunan = scanf("%d", &suMiktari);
                        while ((temizle = getchar()) != '\n' && temizle != EOF);

                        if (okunan != 1 || suMiktari < 1 || suMiktari > su)
                        {
                            printf("Gecersiz miktar! Mevcut sudan fazla veya 1'den az girilemez.\n");
                            suGecerli = 0;
                        }
                        else
                        {
                            suGecerli = 1;
                        }
                    } while (suGecerli == 0);

                    /* Mod operatoru ile cift/tek kontrolu */
                    if (suMiktari % 2 == 0)
                    {
                        kullanilanSu = suMiktari;       /* Cift: tamami kullanilir */
                        artikSu = 0;
                    }
                    else
                    {
                        kullanilanSu = suMiktari - 1;   /* Tek: 1 birim artik kalir */
                        artikSu = 1;
                    }

                    /* Her 2 birim su 3 birim oksijene donusur */
                    uretilenOksijen = (kullanilanSu / 2) * 3;

                    enerji = enerji - 10;
                    su = su - kullanilanSu;

                    /* Oksijen 100'u asmamali */
                    if (oksijen + uretilenOksijen > 100)
                    {
                        uretilenOksijen = 100 - oksijen;
                        printf("Oksijen deposu doldu, fazlasi bosa gitti.\n");
                    }
                    oksijen = oksijen + uretilenOksijen;

                    printf("%d su kullanildi, %d su artik olarak depoda kaldi.\n", kullanilanSu, artikSu);
                    printf("Uretilen oksijen: +%d | Enerji: -10\n", uretilenOksijen);
                }
                turHarcandi = 1;
                break;

            case 't':
            case 'T':
                printf("[T] Tamir baslatiliyor...\n");

                if (yedekParca == 0)
                {
                    /* Yedek parca yoksa tamir yapilamaz */
                    printf("Yedek parca yok, tamir yapilamaz! (komut iptal edildi)\n");
                }
                else
                {
                    /* Hasar seviyesine gore kademe: IF - ELSE IF - ELSE zinciri */
                    if (modulHasari <= 30)
                    {
                        printf("Hasar seviyesi: HAFIF\n");
                        harcananParca = 1;
                        onarilanHasar = 20;
                    }
                    else if (modulHasari <= 70)
                    {
                        printf("Hasar seviyesi: ORTA\n");
                        harcananParca = 2;
                        onarilanHasar = 35;
                    }
                    else
                    {
                        printf("Hasar seviyesi: AGIR\n");
                        harcananParca = 3;
                        onarilanHasar = 50;
                    }

                    if (yedekParca < harcananParca)
                    {
                        printf("Yeterli yedek parca yok (gereken: %d, mevcut: %d). Tamir iptal edildi.\n",
                               harcananParca, yedekParca);
                    }
                    else
                    {
                        eskiHasar = modulHasari;
                        yedekParca = yedekParca - harcananParca;
                        modulHasari = modulHasari - onarilanHasar;
                        if (modulHasari < 0)
                        {
                            modulHasari = 0;
                        }
                        printf("%d yedek parca harcandi. Modul hasari: %d -> %d\n",
                               harcananParca, eskiHasar, modulHasari);
                    }
                }
                turHarcandi = 1;
                break;

            case 'd':
            case 'D':
                /* D tur harcamaz: turHarcandi 0 kalir */
                printf("--- DURUM RAPORU ---\n");
                printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
                printf("Su: %d | Yedek parca: %d | Modul hasari: %d\n", su, yedekParca, modulHasari);
                printf("Hava kilidi: %s | Gun: %d | Tur: %d\n",
                       (kilitMuhurlu == 1) ? "MUHURLU" : "ACIK", gun, tur);

                /* Kritik seviyedeki her deger icin ayri uyari */
                if (oksijen < 25)
                {
                    printf("UYARI: Oksijen kritik seviyede (%d)!\n", oksijen);
                }
                if (saglik < 30)
                {
                    printf("UYARI: Saglik kritik seviyede (%d)!\n", saglik);
                }
                if (enerji < 20)
                {
                    printf("UYARI: Enerji kritik seviyede (%d)!\n", enerji);
                }
                if (su < 10)
                {
                    printf("UYARI: Su kritik seviyede (%d)!\n", su);
                }
                if (modulHasari > 70)
                {
                    printf("UYARI: Modul hasari kritik seviyede (%d)!\n", modulHasari);
                }
                if (yedekParca == 0)
                {
                    printf("UYARI: Yedek parca kalmadi!\n");
                }

                /* Tum degerler guvenli aralikta mi: tek && ile birlesik kosul */
                if (oksijen >= 25 && saglik >= 30 && enerji >= 20 &&
                    su >= 10 && modulHasari <= 70 && yedekParca > 0)
                {
                    printf("TUM SISTEMLER NORMAL\n");
                }
                break;

            case 'u':
            case 'U':
                printf("[U] Dinleniyorsun...\n");

                /* Enerji artar, oksijen tuketimi devam eder */
                enerji = enerji + 30;
                oksijen = oksijen - 5;
                printf("Enerji +30, Oksijen -5\n");

                /* Saglik, oksijen durumuna gore degisir (ic ice if) */
                if (oksijen < 20)
                {
                    /* Oksijen yetersiz: saglik artmaz, azalir */
                    saglik = saglik - 5;
                    printf("Oksijen 20'nin altinda! Dinlenme ise yaramadi: Saglik -5\n");
                    if (saglik < 30)
                    {
                        printf("TEHLIKE: Saglik cok dusuk, once oksijen uret!\n");
                    }
                }
                else
                {
                    saglik = saglik + 10;
                    printf("Saglik +10\n");
                }
                turHarcandi = 1;
                break;

            case 'k':
            case 'K':
                printf("[K] KUM FIRTINASI basladi! Sure: %d tur\n", firtinaTurSayisi);

                hasarliTur = 0;
                toplamSaglikKaybi = 0;
                toplamOksijenKaybi = 0;
                toplamModulHasari = 0;

                /* FOR dongusu: her tur bir firtina turudur */
                for (i = 1; i <= firtinaTurSayisi; i++)
                {
                    printf("\n-- Firtina turu %d/%d --\n", i, firtinaTurSayisi);

                    hasar = firtinaHasari;
                    if (i % 2 == 0)   /* Cift turlarda ruzgar siddetlenir, hasar 2 kat */
                    {
                        hasar = hasar * 2;
                        printf("Ruzgar siddetlendi! Hasar 2 kat: %d\n", hasar);
                    }

                    /* Oyuncunun durumuna gore korunma esigi */
                    if (saglik > 50 && modulHasari < 50)
                    {
                        korunmaEsigi = 40;
                    }
                    else
                    {
                        korunmaEsigi = 20;
                    }

                    firtinaZar = rand() % 100;

                    if (firtinaZar < korunmaEsigi)
                    {
                        printf("Zar: %d / Esik: %d -> KORUNDUN, zarar yok\n", firtinaZar, korunmaEsigi);
                    }
                    else if (firtinaZar < korunmaEsigi + 20)
                    {
                        /* Can kaybi */
                        kayip = hasar;
                        if (kayip > saglik)
                        {
                            kayip = saglik;
                        }
                        saglik = saglik - kayip;
                        toplamSaglikKaybi = toplamSaglikKaybi + kayip;
                        hasarliTur++;
                        printf("Zar: %d / Esik: %d -> CAN KAYBI! Saglik -%d\n",
                               firtinaZar, korunmaEsigi + 20, kayip);
                    }
                    else if (firtinaZar < korunmaEsigi + 40)
                    {
                        /* Oksijen sizintisi */
                        if (kilitMuhurlu == 1)
                        {
                            printf("Zar: %d / Esik: %d -> Oksijen sizintisi, ama hava kilidi muhurlu: ENGELLENDI\n",
                                   firtinaZar, korunmaEsigi + 40);
                            continue;   /* Bu turun kalan kismi atlanir */
                        }
                        kayip = hasar;
                        if (kayip > oksijen)
                        {
                            kayip = oksijen;
                        }
                        oksijen = oksijen - kayip;
                        toplamOksijenKaybi = toplamOksijenKaybi + kayip;
                        hasarliTur++;
                        printf("Zar: %d / Esik: %d -> OKSIJEN SIZINTISI! Oksijen -%d\n",
                               firtinaZar, korunmaEsigi + 40, kayip);
                    }
                    else
                    {
                        /* Modul hasari */
                        kayip = hasar;
                        if (kayip > 100 - modulHasari)
                        {
                            kayip = 100 - modulHasari;
                        }
                        modulHasari = modulHasari + kayip;
                        toplamModulHasari = toplamModulHasari + kayip;
                        hasarliTur++;
                        printf("Zar: %d / Esik: 100 -> MODUL HASARI! Hasar +%d\n", firtinaZar, kayip);
                    }

                    /* Saglik veya oksijen 0'a duserse firtina aninda biter */
                    if (saglik <= 0 || oksijen <= 0)
                    {
                        printf("KRITIK: Saglik veya oksijen 0'a dustu, firtina sonlandirildi!\n");
                        break;
                    }
                }

                printf("\n--- FIRTINA OZETI ---\n");
                printf("Hasar alinan tur: %d\n", hasarliTur);
                printf("Kaybedilen saglik: %d | Kaybedilen oksijen: %d | Modul hasari artisi: %d\n",
                       toplamSaglikKaybi, toplamOksijenKaybi, toplamModulHasari);
                turHarcandi = 1;
                break;

            case 'h':
            case 'H':
                printf("[H] Hava kilidi sifresi...\n");

                if (kilitMuhurlu == 1)
                {
                    printf("Hava kilidi zaten muhurlu.\n");
                }
                else if (enerji < 15)
                {
                    /* En kotu durumda 3 yanlis deneme x 5 enerji = 15 */
                    printf("Yetersiz enerji (gereken: 15, mevcut: %d)! Komut iptal edildi.\n", enerji);
                }
                else
                {
                    /* Sifre sabit degil: gun sayisindan turetilir */
                    sifre = 'A' + (gun % 26);
                    deneme = 0;

                    printf("IPUCU: Sifre, alfabede A harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);
                    printf("Tek harf gir (A-Z), 3 deneme hakkin var.\n");

                    do
                    {
                        printf("Deneme %d/3 - Sifre: ", deneme + 1);
                        scanf(" %c", &giris);
                        while ((temizle = getchar()) != '\n' && temizle != EOF);

                        /* Kucuk harf girisi de kabul edilir */
                        if (giris >= 'a' && giris <= 'z')
                        {
                            giris = giris - 'a' + 'A';
                        }
                        deneme++;

                        if (giris != sifre)
                        {
                            enerji = enerji - 5;
                            printf("YANLIS! Enerji -5. ");
                            if (giris < sifre)
                            {
                                printf("Ipucu: ileri git.\n");
                            }
                            else
                            {
                                printf("Ipucu: geri git.\n");
                            }
                        }
                    } while (giris != sifre && deneme < 3);   /* Tek mantiksal ifade */

                    if (giris == sifre)
                    {
                        kilitMuhurlu = 1;
                        printf("DOGRU SIFRE! Hava kilidi muhurlendi.\n");
                    }
                    else
                    {
                        modulHasari = modulHasari + 20;
                        printf("3 deneme de basarisiz! Modul hasari +20.\n");
                    }
                }
                turHarcandi = 1;
                break;

            case 'q':
            case 'Q':
                printf("Cikis yapiliyor...\n");
                oyunDurumu = 3;
                break;

            default:
                /* Tanimsiz komut: uyari verir, tur harcamaz */
                printf("Tanimsiz komut! Gecerli komutlar: G O T D U K H Q (tur harcanmadi)\n");
                break;
        }

        /* ---- ZAMAN AKISI ---- */
        if (turHarcandi == 1)
        {
            tur++;
            if (tur % GUN_TUR_SAYISI == 0)   /* Her 3 turda yeni gun */
            {
                gun++;
                if (gun <= HEDEF_GUN)        /* 16. gun baslamaz, oyun kazanilir */
                {
                    printf("\n*** GUN %d basladi: oksijen -%d ***\n", gun, GUN_BASI_OKSIJEN_KAYBI);
                    oksijen = oksijen - GUN_BASI_OKSIJEN_KAYBI;
                    if (modulHasari > 70)
                    {
                        printf("*** Modul hasari cok yuksek: saglik -%d ***\n", GUN_BASI_SAGLIK_KAYBI);
                        saglik = saglik - GUN_BASI_SAGLIK_KAYBI;
                    }
                }
            }
        }

        /* ---- SINIRLANDIRMA (0-100) ---- */
        if (saglik < 0)       saglik = 0;
        if (saglik > 100)     saglik = 100;
        if (enerji < 0)       enerji = 0;
        if (enerji > 100)     enerji = 100;
        if (oksijen < 0)      oksijen = 0;
        if (oksijen > 100)    oksijen = 100;
        if (modulHasari < 0)  modulHasari = 0;
        if (modulHasari > 100) modulHasari = 100;

        /* ---- OYUN SONU KONTROLU (her turun sonunda) ---- */
        if (oyunDurumu == 0)
        {
            if (saglik == 0 || oksijen == 0)
            {
                oyunDurumu = 2;
            }
            else if (gun > HEDEF_GUN)
            {
                oyunDurumu = 1;
            }
        }

    } while (oyunDurumu == 0);

    /* =====================================================
     * SONUC RAPORU (kazanma, kaybetme veya Q ile cikis)
     * ===================================================== */
    printf("\n==============================\n");
    if (oyunDurumu == 1)
    {
        printf("KURTARMA EKIBI GELDI! Oyunu kazandin.\n");
        gun = HEDEF_GUN;   /* 15. gun tamamlandi */
    }
    else if (oyunDurumu == 2)
    {
        printf("OYUN BITTI: Hayatta kalamadin.\n");
    }
    else
    {
        printf("Simulasyondan cikildi.\n");
    }

    puan = (gun * 100) + saglik + oksijen - modulHasari;

    printf("--- SONUC RAPORU ---\n");
    printf("Hayatta kalinan gun: %d\n", gun);
    printf("Toplam tur: %d\n", tur);
    printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
    printf("Su: %d | Yedek parca: %d | Modul hasari: %d\n", su, yedekParca, modulHasari);
    printf("PUAN: %d\n", puan);
    printf("==============================\n");

    return 0;
}