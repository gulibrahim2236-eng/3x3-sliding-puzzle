#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    int tahta[3][3];
    int bos_satir;
    int bos_sutun;
    int hamle_sayisi;
    bool cozuldu_mu;
} OyunDurumu;

void tahtayi_hazirla(OyunDurumu *durum);
void cozulebilir_yap(OyunDurumu *durum);
void tahtayi_yazdir(OyunDurumu *durum);
void hamleyi_al(OyunDurumu *durum);
void galibiyet_kontrolu(OyunDurumu *durum);

int main() {
    // Rastgelelik için seed ayarımız
    srand(time(NULL));

    OyunDurumu durum;
    durum.hamle_sayisi = 0;
    durum.cozuldu_mu = false;

    printf("--- 3x3 SLIDING PUZZLE OYUNUNA HOS GELDINIZ ---\n");
    printf("Hedef: Sayilari 1'den 8'e kadar siralamak ve boslugu en sona getirmek.\n");
    printf("Kontroller: W (Yukari), S (Asagi), A (Sol), D (Sag)\n\n");

    tahtayi_hazirla(&durum);
    cozulebilir_yap(&durum);

    while (!durum.cozuldu_mu) {
        tahtayi_yazdir(&durum);
        hamleyi_al(&durum);
        galibiyet_kontrolu(&durum);
    }

    tahtayi_yazdir(&durum);
    printf("\nTEBRIKLER! Oyunu %d hamlede basariyla tamamladiniz.\n", durum.hamle_sayisi);

    return 0;
}

void tahtayi_hazirla(OyunDurumu *durum) {
    int tek_boyut[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    
    // Karıştırma algoritmamiz
    for (int i = 8; i > 0; i--) {
        int j = rand() % (i + 1);
        int gecici = tek_boyut[i];
        tek_boyut[i] = tek_boyut[j];
        tek_boyut[j] = gecici;
    }
    
    // Tek boyutlu diziyi 2D matrise aktar
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            durum->tahta[i][j] = tek_boyut[k];
            if (tek_boyut[k] == 0) {
                durum->bos_satir = i;
                durum->bos_sutun = j;
            }
            k++;
        }
    }
}

void cozulebilir_yap(OyunDurumu *durum) {
    int gecici_dizi[9];
    int k = 0;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            gecici_dizi[k++] = durum->tahta[i][j];
        }
    }

    int terslik_sayisi = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (gecici_dizi[i] != 0 && gecici_dizi[j] != 0 && gecici_dizi[i] > gecici_dizi[j]) {
                terslik_sayisi++;
            }
        }
    }
    
    // Inversion tek ise son iki dolu taşı yer değiştir
    if (terslik_sayisi % 2 != 0) {
        int indis1 = 8, indis2 = 7;
        
        if (gecici_dizi[indis1] == 0) {
            indis1 = 7; indis2 = 6;
        } else if (gecici_dizi[indis2] == 0) {
            indis2 = 6;
        }

        int yedek = gecici_dizi[indis1];
        gecici_dizi[indis1] = gecici_dizi[indis2];
        gecici_dizi[indis2] = yedek;

        k = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                durum->tahta[i][j] = gecici_dizi[k];
                if (gecici_dizi[k] == 0) {
                    durum->bos_satir = i;
                    durum->bos_sutun = j;
                }
                k++;
            }
        }
    }
}

void tahtayi_yazdir(OyunDurumu *durum) {
	
    system("cls || clear");
    printf("--- 3x3 SLIDING PUZZLE OYUNUNA HOS GELDINIZ ---\n");
    printf("Hedef: Sayilari 1'den 8'e kadar siralamak ve boslugu en sona getirmek.\n");
    printf("Kontroller: W (Yukari), S (Asagi), A (Sol), D (Sag)\n");
    printf("-------------\n");
    printf("\nTahta Durumu (Hamle: %d)\n", durum->hamle_sayisi);
    printf("-------------\n");
    for (int i = 0; i < 3; i++) {
        printf("| ");
        for (int j = 0; j < 3; j++) {
            if (durum->tahta[i][j] == 0) {
                printf("  | "); 
            } else {
                printf("%d | ", durum->tahta[i][j]);
            }
        }
        printf("\n-------------\n");
    }
}

// Girdiyi alır, buffer'ı temizler ve sınır kontrolü yapar.
void hamleyi_al(OyunDurumu *durum) {
    char tus;
    printf("Hamle (W/A/S/D): ");
    
    tus = getchar();
    char c;
    while ((c = getchar()) != '\n' && c != EOF);

    tus = toupper(tus);

    int yeni_satir = durum->bos_satir;
    int yeni_sutun = durum->bos_sutun;

    if (tus == 'W') yeni_satir++;
    else if (tus == 'S') yeni_satir--;
    else if (tus == 'A') yeni_sutun++;
    else if (tus == 'D') yeni_sutun--;
    else {
        printf("[!] Sadece W, A, S veya D tuslarini kullanin.\n");
        return;
    }

    // Sınır kontrolümüz
    if (yeni_satir < 0 || yeni_satir > 2 || yeni_sutun < 0 || yeni_sutun > 2) {
        printf("[!] Gecersiz hamle! Kenardan disari cikamazsiniz.\n");
        return;
    }

    // Değerlerin yerini değiştiriyor (Swap)
    durum->tahta[durum->bos_satir][durum->bos_sutun] = durum->tahta[yeni_satir][yeni_sutun];
    durum->tahta[yeni_satir][yeni_sutun] = 0;
    
    durum->bos_satir = yeni_satir;
    durum->bos_sutun = yeni_sutun;
    
    durum->hamle_sayisi++;
}

void galibiyet_kontrolu(OyunDurumu *durum) {
    int hedef_matris[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (durum->tahta[i][j] != hedef_matris[i][j]) {
                return; 
            }
        }
    }
    
    durum->cozuldu_mu = true;
}

