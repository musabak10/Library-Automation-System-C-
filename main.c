#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_AD     50
#define MAX_ISBN   14
#define MAX_ETIKET 25
#define MAX_TARIH  11

typedef union {
    char rafta[7];
    char ogrenciNo[9];
} RafDurumu;

typedef enum {
    RAFTA_ENUM = 0,
    ODUNC_ENUM = 1
} RafTipi;

typedef struct KitapOrnegi {
    char etiketNo[MAX_ETIKET];
    RafTipi durum;
    RafDurumu rafDurumu;
} KitapOrnegi;

typedef struct KitapNode {
    char ad[MAX_AD];
    char isbn[MAX_ISBN];
    int adet;
    KitapOrnegi* ornekler;
    struct KitapNode* next;
} KitapNode;

typedef struct YazarNode {
    int yazarId;
    char ad[MAX_AD];
    char soyad[MAX_AD];
    struct YazarNode* next;
} YazarNode;

typedef struct {
    char ad[MAX_AD];
    char soyad[MAX_AD];
    char ogrenciNo[9];
    int puan;
} Ogrenci;

typedef struct {
    char isbn[MAX_ISBN];
    int yazarId;
} KitapYazar;

typedef struct {
    char etiketNo[MAX_ETIKET];
    char ogrenciNo[9];
    int islemTuru;
    char tarih[MAX_TARIH];
} OduncKayit;

typedef struct {
    Ogrenci* dizi;
    int boyut;
    int kapasite;
} OgrenciDizi;

typedef struct {
    KitapYazar* dizi;
    int boyut;
    int kapasite;
} KitapYazarDizi;
void bugunTarih(char* buf);
int tarihFarkGun(char* t1, char* t2);
void ogrencileriDosyayaYaz(OgrenciDizi* od);
void ogrencileriDosyadanOku(OgrenciDizi* od);
void yazarlariDosyayaYaz(YazarNode* bas);
YazarNode* yazarlariDosyadanOku(void);
void kitaplariDosyayaYaz(KitapNode* bas);
KitapNode* kitaplariDosyadanOku(void);
void kitapYazarDosyayaYaz(KitapYazarDizi* kyd);
void kitapYazarDosyadanOku(KitapYazarDizi* kyd);
void oduncKayitEkleDosya(OduncKayit* kayit);
int ogrenciBul(OgrenciDizi* od, char* no);
int ogrenciBulAdSoyad(OgrenciDizi* od, char* ad, char* soyad);
void ogrenciEkle(OgrenciDizi* od);
void ogrenciSil(OgrenciDizi* od);
void ogrenciGuncelle(OgrenciDizi* od);
void ogrenciListele(OgrenciDizi* od);
void ogrenciBilgiGoster(OgrenciDizi* od);
void kitapTeslimEtmemisListele(OgrenciDizi* od, KitapNode* kitapBas);
void cezaliOgrenciListele(OgrenciDizi* od);
int yazarBul(YazarNode* bas, int id);
int yazarMaxId(YazarNode* bas);
YazarNode* yazarEkle(YazarNode* bas);
YazarNode* yazarSil(YazarNode* bas, KitapYazarDizi* kyd);
YazarNode* yazarGuncelle(YazarNode* bas);
void yazarListele(YazarNode* bas);
void yazarBilgiGoster(YazarNode* bas, KitapYazarDizi* kyd, KitapNode* kitapBas);
KitapNode* kitapBul(KitapNode* bas, char* isbn);
KitapNode* kitapBulAd(KitapNode* bas, char* ad);
KitapNode* kitapEkle(KitapNode* bas);
KitapNode* kitapSil(KitapNode* bas);
KitapNode* kitapGuncelle(KitapNode* bas);
void kitapBilgiGoster(KitapNode* bas);
void raftakiKitaplariListele(KitapNode* bas);
void zamanindaTeslimEdilmeyenleriListele(void);
void kitapYazarEslestir(KitapNode* kitapBas, YazarNode* yazarBas, KitapYazarDizi* kyd);
void kitapYazarGuncelle(KitapNode* kitapBas, YazarNode* yazarBas, KitapYazarDizi* kyd);
void kitapOduncAl(OgrenciDizi* od, KitapNode* kitapBas);
void kitapIadeEt(OgrenciDizi* od, KitapNode* kitapBas);
void yazarListesiniSerbest(YazarNode* bas);
void kitapListesiniSerbest(KitapNode* bas);
void menuGoster(void);
void bugunTarih(char* buf){
    time_t t;
    struct tm* tmBilgi;
    t=time(NULL);
    tmBilgi=localtime(&t);
    sprintf(buf,"%04d-%02d-%02d",tmBilgi->tm_year + 1900,tmBilgi->tm_mon + 1,tmBilgi->tm_mday);
}
int tarihFarkGun(char* t1, char* t2){
    struct tm tm1;
    struct tm tm2;
    time_t zaman1;
    time_t zaman2;
    memset(&tm1,0,sizeof(tm1));
    memset(&tm2,0,sizeof(tm2));
    sscanf(t1,"%d-%d-%d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday);
    sscanf(t2,"%d-%d-%d",&tm2.tm_year,&tm2.tm_mon,&tm2.tm_mday);
    tm1.tm_year-=1900;
    tm1.tm_mon-=1;
    tm2.tm_year-=1900;
    tm2.tm_mon-=1;
    zaman1=mktime(&tm1);
    zaman2=mktime(&tm2);
    return (int)((zaman2-zaman1)/86400);
}
void ogrencileriDosyayaYaz(OgrenciDizi* od){
    FILE* f;
    int i;
    f=fopen("Ogrenciler.csv","w");
    if(f==NULL) {
        printf("[HATA] Ogrenciler.csv dosyasi acilamadi\n");
        return;
    }
    for(i=0; i < od->boyut; i++){
        fprintf(f,"%s,%s,%s,%d\n",od->dizi[i].ad,od->dizi[i].soyad,od->dizi[i].ogrenciNo,od->dizi[i].puan);
    }
    fclose(f);
}
void ogrencileriDosyadanOku(OgrenciDizi* od){
    FILE* f;
    char satir[200];
    f=fopen("Ogrenciler.csv", "r");
    if(f==NULL) return;
    while(fgets(satir, sizeof(satir), f)){
        if (od->boyut >= od->kapasite){
            od->kapasite *= 2;
            od->dizi=(Ogrenci*)realloc(od->dizi, sizeof(Ogrenci) * od->kapasite);
            if(od->dizi==NULL){
            	printf("HATA DIZI ICIN BELLEK TAHSIS EDILEMEDI\n");
			}
        }
        sscanf(satir,"%[^,],%[^,],%[^,],%d",od->dizi[od->boyut].ad,od->dizi[od->boyut].soyad,od->dizi[od->boyut].ogrenciNo,&od->dizi[od->boyut].puan);
        od->boyut++;
    }
    fclose(f);
}
void yazarlariDosyayaYaz(YazarNode* bas){
    FILE* f;
    YazarNode* curr;
    f=fopen("yazarlar.csv","w");
    if(f==NULL){
        printf("[HATA] yazarlar.csv dosyasi acilamadi\n");
        return;
    }
    curr=bas;
    while(curr!=NULL) {
        fprintf(f,"%d,%s,%s\n",curr->yazarId,curr->ad,curr->soyad);
        curr=curr->next;
    }
    fclose(f);
}

YazarNode* yazarlariDosyadanOku(void){
    FILE* f;
    YazarNode* bas;
    YazarNode* son;
    YazarNode* yeni;
    char satir[200];
    bas=NULL;
    son=NULL;
    f=fopen("yazarlar.csv", "r");
    if(f==NULL){
    	return NULL;
	}
    while(fgets(satir, sizeof(satir),f)){
        yeni=(YazarNode*)malloc(sizeof(YazarNode));
        sscanf(satir,"%d,%[^,],%[^\n]",&yeni->yazarId,yeni->ad,yeni->soyad);
        yeni->next=NULL;
        if(bas==NULL){
            bas=yeni;
            son=yeni;
        }else{
            son->next=yeni;
            son=yeni;
        }
    }
    fclose(f);
    return bas;
}

void kitaplariDosyayaYaz(KitapNode* bas){
    FILE* f;
    KitapNode* curr;
    int i;
    f=fopen("Kitaplar.csv","w");
    if(f==NULL){
        printf("[HATA] Kitaplar.csv dosyasi acilamadi\n");
        return;
    }
    curr=bas;
    while(curr!=NULL) {
        fprintf(f,"%s,%s,%d\n",curr->ad,curr->isbn,curr->adet);
        for(i=0;i<curr->adet;i++) {
            if(curr->ornekler[i].durum==RAFTA_ENUM){
                fprintf(f,"\t%s,RAFTA\n",curr->ornekler[i].etiketNo);
            } else {
                fprintf(f,"\t%s,%s\n", curr->ornekler[i].etiketNo,curr->ornekler[i].rafDurumu.ogrenciNo);
            }
        }
        curr=curr->next;
    }
    fclose(f);
}

KitapNode* kitaplariDosyadanOku(void){
    FILE* f;
    KitapNode* bas;
    KitapNode* son;
    KitapNode* curr;
    KitapNode* yeni;
    char satir[300];
    char etiket[MAX_ETIKET];
    char durum[10];
    int ornek_idx;
    bas=NULL;
    son=NULL;
    curr=NULL;
    ornek_idx = 0;
    f=fopen("Kitaplar.csv","r");
    if(f==NULL){
    	return NULL;
	}
    while(fgets(satir,sizeof(satir),f)){
        if(satir[0]==' '|| satir[0]=='\t'){
            if(curr!=NULL&&ornek_idx<curr->adet){
                sscanf(satir," %[^,],%[^\n]",etiket,durum);
                strcpy(curr->ornekler[ornek_idx].etiketNo,etiket);
                if(strcmp(durum,"RAFTA")==0) {
                    curr->ornekler[ornek_idx].durum=RAFTA_ENUM;
                    strcpy(curr->ornekler[ornek_idx].rafDurumu.rafta,"RAFTA");
                }else{
                    curr->ornekler[ornek_idx].durum=ODUNC_ENUM;
                    strcpy(curr->ornekler[ornek_idx].rafDurumu.ogrenciNo,durum);
                }
                ornek_idx++;
            }
        }else{
            yeni=(KitapNode*)malloc(sizeof(KitapNode));
            sscanf(satir,"%[^,],%[^,],%d",yeni->ad, yeni->isbn,&yeni->adet);
            yeni->ornekler=(KitapOrnegi*)malloc(sizeof(KitapOrnegi)*yeni->adet);
            yeni->next=NULL;
            ornek_idx=0;
            curr=yeni;
            if(bas==NULL){
                bas=yeni;
                son=yeni;
            }else{
                son->next=yeni;
                son=yeni;
            }
        }
    }
    fclose(f);
    return bas;
}

void kitapYazarDosyayaYaz(KitapYazarDizi* kyd){
    FILE* f;
    f=fopen("KitapYazar.dat","wb");
    if (f==NULL) {
        printf("[HATA] KitapYazar.dat dosyasi acilamadi\n");
        return;
    }
    fwrite(kyd->dizi,sizeof(KitapYazar),kyd->boyut,f);
    fclose(f);
}

void kitapYazarDosyadanOku(KitapYazarDizi* kyd) {
    FILE* f;
    KitapYazar ky;
    f=fopen("KitapYazar.dat","rb");
    if(f==NULL){
    	return;
	}
    while(fread(&ky, sizeof(KitapYazar),1,f)==1) {
        if (kyd->boyut>=kyd->kapasite) {
            kyd->kapasite*=2;
            kyd->dizi=(KitapYazar*)realloc(kyd->dizi, sizeof(KitapYazar) * kyd->kapasite);
            if(kyd->dizi==NULL){
            	printf("HATA DIZI ICIN BELLEK TAHSIS EDILEMEDI\n");
			}
        }
        kyd->dizi[kyd->boyut++]=ky;
    }
    fclose(f);
}

void oduncKayitEkleDosya(OduncKayit* kayit){
    FILE* f;
    f=fopen("OduncKayitlari.csv","a");
    if(f == NULL){
        printf("[HATA] OduncKayitlari.csv dosyasi acilamadi\n");
        return;
    }
    fprintf(f,"%s,%s,%d,%s\n",kayit->etiketNo,kayit->ogrenciNo,kayit->islemTuru,kayit->tarih);
    fclose(f);
}
int ogrenciBul(OgrenciDizi* od,char* no) {
    int i;
    for(i=0;i<od->boyut;i++){
        if(strcmp(od->dizi[i].ogrenciNo,no)==0){
        	return i;
		}
    }
    return -1;
}

int ogrenciBulAdSoyad(OgrenciDizi* od,char* ad,char* soyad){
    int i;
    for(i=0;i<od->boyut;i++) {
        if(strcmp(od->dizi[i].ad, ad) == 0 && strcmp(od->dizi[i].soyad, soyad) == 0){
        	return i;
		}
    }
    return -1;
}

void ogrenciEkle(OgrenciDizi* od){
    char no[9];
    char ad[MAX_AD];
    char soyad[MAX_AD];
    if (od->boyut>= od->kapasite){
        od->kapasite *= 2;
        od->dizi = (Ogrenci*)realloc(od->dizi, sizeof(Ogrenci) * od->kapasite);
            if(od->dizi==NULL){
            	printf("HATA DIZI ICIN BELLEK TAHSIS EDILEMEDI\n");
			}
    }
    printf("\n Ogrenci numarasi(8 hane):");
    scanf(" %s",no);
    if (ogrenciBul(od,no)!=-1){
        printf("Bu numara zaten kayitli!\n");
        return;
    }
    printf("Ad:");
    scanf(" %[^\n]", ad);
    printf("Soyad:");
    scanf(" %[^\n]",soyad);
    strcpy(od->dizi[od->boyut].ogrenciNo, no);
    strcpy(od->dizi[od->boyut].ad, ad);
    strcpy(od->dizi[od->boyut].soyad, soyad);
    od->dizi[od->boyut].puan=100;
    od->boyut++;
    ogrencileriDosyayaYaz(od);
    printf("\n Ogrenci basariyla eklendi. Baslangic puani:100\n");
}
void ogrenciSil(OgrenciDizi* od){
    char no[9];
    int idx;
    int i;
    printf("\nSilinecek ogrencinin numarasi:");
    scanf(" %s",no);
    idx=ogrenciBul(od, no);
    if (idx == -1) {
        printf("Bu numaraya ait ogrenci bulunamadi\n");
        return;
    }
    printf("%s %s adli ogrenci siliniyor...\n",od->dizi[idx].ad, od->dizi[idx].soyad);
    for(i=idx; i < od->boyut-1;i++) {
        od->dizi[i]=od->dizi[i+1];
    }
    od->boyut--;
    ogrencileriDosyayaYaz(od);
    printf("Ogrenci basariyla silindi\n");
}
void ogrenciGuncelle(OgrenciDizi* od){
    char no[9];
    char ad[MAX_AD];
    char soyad[MAX_AD];
    int idx;
    printf("\n Guncellenecek ogrencinin numarasi : ");
    scanf(" %s",no);
    idx=ogrenciBul(od, no);
    if(idx == -1){
        printf("Bu numaraya ait ogrenci bulunamadi\n");
        return;
    }
    printf("Mevcut ad: %s\n", od->dizi[idx].ad);
    printf("Yeni ad: ");
    scanf(" %[^\n]",ad);
    printf("Mevcut soyad: %s\n", od->dizi[idx].soyad);
    printf("Yeni soyad:");
    scanf(" %[^\n]",soyad);
    strcpy(od->dizi[idx].ad,ad);
    strcpy(od->dizi[idx].soyad,soyad);
    ogrencileriDosyayaYaz(od);
    printf("Ogrenci bilgileri guncellendi\n");
}

void ogrenciListele(OgrenciDizi* od) {
    int i;
    printf("\n");
    printf(" %-10s %-16s %-16s %-6s\n", "No", "Ad", "Soyad", "Puan");
    printf("--------------------------------------------------\n");
    for(i=0;i<od->boyut; i++){
        printf("%-10s %-16s %-16s %-6d\n",od->dizi[i].ogrenciNo,od->dizi[i].ad,od->dizi[i].soyad,od->dizi[i].puan);
    }
    printf("Toplam %d ogrenci listelendi\n", od->boyut);
}
void ogrenciBilgiGoster(OgrenciDizi* od){
    char no[9];
    char ad[MAX_AD];
    char soyad[MAX_AD];
    char satir[200];
    char etiket[MAX_ETIKET];
    char ogrNo[9];
    char tarih[MAX_TARIH];
    int idx;
    int tip;
    FILE* f;
    printf("\n Arama yontemi secin:\n");
    printf("1) Ogrenci numarasiyla\n");
    printf("2) Ad-Soyad ile\n");
    printf("Seciminiz: ");
    scanf(" %s",no);
    if(strcmp(no,"1")==0){
        printf("Ogrenci numarasi:");
        scanf(" %s",no);
        idx=ogrenciBul(od,no);
    }else{
        printf("Ad:");
        scanf(" %[^\n]",ad);
        printf("Soyad:");
        scanf(" %[^\n]",soyad);
        idx=ogrenciBulAdSoyad(od, ad, soyad);
    }
    if(idx==-1){
        printf("Ogrenci bulunamadi\n");
        return;
    }
    printf("   OGRENCI BILGILERI                        \n");
    printf("  ------------------------------------------\n");
    printf("   Numara : %s\n", od->dizi[idx].ogrenciNo);
    printf("   Ad     : %s\n", od->dizi[idx].ad);
    printf("   Soyad  : %s\n", od->dizi[idx].soyad);
    printf("   Puan   : %d\n", od->dizi[idx].puan);
    printf("  ------------------------------------------\n");
    printf("\n Kitap Hareketleri:\n");
    printf("%-22s %-10s %-12s\n", "Etiket No", "Islem", "Tarih");
    printf("  ----------------------------------------------\n");
    f=fopen("OduncKayitlari.csv", "r");
    if(f==NULL) {
        printf("Henuz hic kitap hareketi yok.\n");
        return;
    }
    while(fgets(satir, sizeof(satir), f)) {
        sscanf(satir, "%[^,],%[^,],%d,%[^\n]", etiket, ogrNo, &tip, tarih);
        if (strcmp(ogrNo, od->dizi[idx].ogrenciNo) == 0) {
            printf(" %-22s ", etiket);
            if(tip == 0){
                printf("%-10s ", "Odunc Alindi");
            }else{
                printf("%-10s ", "Iade Edildi");
            }
                printf("%-12s\n", tarih);
            }
    }
    fclose(f);
}

void kitapTeslimEtmemisListele(OgrenciDizi* od, KitapNode* kitapBas) {
    KitapNode* curr;
    int i;
    int idx;
    int bulundu;
    bulundu = 0;
    curr=kitapBas;
    printf("\nKitap teslim etmemis ogrenciler:\n");
    printf("%-30s %-22s %-10s\n","Kitap Adi","Etiket No","Ogrenci No");
    printf("---------------------------------------------------------------\n");
    while (curr != NULL){
        for(i=0;i<curr->adet;i++) {
            if(curr->ornekler[i].durum == ODUNC_ENUM){
                idx=ogrenciBul(od, curr->ornekler[i].rafDurumu.ogrenciNo);
                printf("%-30s %-22s %-10s",curr->ad,curr->ornekler[i].etiketNo,curr->ornekler[i].rafDurumu.ogrenciNo);
                if(idx!=-1){
                    printf("(%s %s)",od->dizi[idx].ad, od->dizi[idx].soyad);
                }
                printf("\n");
                bulundu=1;
            }
        }
        curr=curr->next;
    }
    if(bulundu==0){
        printf("Tum ogrenciler kitaplarini teslim etmis\n");
    }
}

void cezaliOgrenciListele(OgrenciDizi* od) {
    int i;
    int bulundu;
    bulundu = 0;
    printf("\nCezali ogrenciler (puani 100'un altinda olanlar):\n");
    printf(" %-10s %-16s %-16s %-6s\n", "No", "Ad", "Soyad", "Puan");
    printf(" --------------------------------------------------\n");
    for(i=0;i<od->boyut;i++) {
        if(od->dizi[i].puan < 100){
            printf("  %-10s %-16s %-16s %-6d\n",
                od->dizi[i].ogrenciNo,
                od->dizi[i].ad,
                od->dizi[i].soyad,
                od->dizi[i].puan);
            bulundu = 1;
        }
    }
    if (bulundu == 0) {
        printf("  Hicbir ogrenci ceza almamis\n");
    }
}
int yazarBul(YazarNode* bas, int id) {
    YazarNode* curr;
    curr = bas;
    while (curr != NULL) {
        if (curr->yazarId == id) return 1;
        curr = curr->next;
    }
    return 0;
}

int yazarMaxId(YazarNode* bas) {
    int max;
    YazarNode* curr;
    max = 0;
    curr = bas;
    while (curr != NULL) {
        if (curr->yazarId > max) max = curr->yazarId;
        curr = curr->next;
    }
    return max;
}

YazarNode* yazarEkle(YazarNode* bas) {
    char ad[MAX_AD];
    char soyad[MAX_AD];
    YazarNode* yeni;
    YazarNode* curr;
    YazarNode* prev;
    yeni = (YazarNode*)malloc(sizeof(YazarNode));
    curr = bas;
    prev = NULL;
    printf("\n  Yazar adi   : ");
    scanf(" %[^\n]", ad);
    printf("  Yazar soyadi: ");
    scanf(" %[^\n]", soyad);
    strcpy(yeni->ad, ad);
    strcpy(yeni->soyad, soyad);
    yeni->yazarId = yazarMaxId(bas) + 1;
    yeni->next = NULL;
    while (curr != NULL && curr->yazarId < yeni->yazarId) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        yeni->next = bas;
        bas = yeni;
    } else {
        yeni->next = curr;
        prev->next = yeni;
    }
    yazarlariDosyayaYaz(bas);
    printf("  Yazar eklendi. Atanan ID: %d\n", yeni->yazarId);
    return bas;
}

YazarNode* yazarSil(YazarNode* bas, KitapYazarDizi* kyd) {
    int id;
    int i;
    YazarNode* curr;
    YazarNode* prev;
    curr = bas;
    prev = NULL;
    printf("\n  Silinecek yazar ID: ");
    scanf("%d", &id);
    while (curr != NULL && curr->yazarId != id) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("  Bu ID'ye ait yazar bulunamadi.\n");
        return bas;
    }
    printf("%s %s adli yazar siliniyor...\n", curr->ad, curr->soyad);
    if (prev == NULL) {
        bas = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
    for (i = 0; i < kyd->boyut; i++) {
        if (kyd->dizi[i].yazarId == id) {
            kyd->dizi[i].yazarId = -1;
        }
    }
    kitapYazarDosyayaYaz(kyd);
    yazarlariDosyayaYaz(bas);
    printf("Yazar silindi. Ilgili kitap-yazar kayitlari -1 olarak guncellendi\n");
    return bas;
}

YazarNode* yazarGuncelle(YazarNode* bas) {
    int id;
    YazarNode* curr;
    curr = bas;
    printf("\nGuncellenecek yazar ID: ");
    scanf("%d", &id);
    while (curr != NULL && curr->yazarId != id) {
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("Bu ID'ye ait yazar bulunamadi\n");
        return bas;
    }
    printf("  Mevcut ad    : %s\n", curr->ad);
    printf("  Yeni ad      : ");
    scanf(" %[^\n]", curr->ad);
    printf("  Mevcut soyad : %s\n", curr->soyad);
    printf("  Yeni soyad   : ");
    scanf(" %[^\n]", curr->soyad);
    yazarlariDosyayaYaz(bas);
    printf("  Yazar bilgileri guncellendi\n");
    return bas;
}

void yazarListele(YazarNode* bas) {
    YazarNode* curr;
    curr = bas;
    printf("\n  %-5s %-16s %-16s\n", "ID", "Ad", "Soyad");
    printf("  ------------------------------------\n");
    while (curr != NULL) {
        printf("  %-5d %-16s %-16s\n", curr->yazarId, curr->ad, curr->soyad);
        curr = curr->next;
    }
}

void yazarBilgiGoster(YazarNode* bas, KitapYazarDizi* kyd, KitapNode* kitapBas) {
    char ad[MAX_AD];
    int id;
    int bulundu;
	int i;
    YazarNode* curr;
    KitapNode* k;
    id = -1;
    curr = bas;
    bulundu = 0;
    printf("\nYazar adi: ");
    scanf(" %[^\n]", ad);
    while (curr != NULL && bulundu == 0) {
        if (strcmp(curr->ad, ad) == 0) {
            id = curr->yazarId;
            bulundu = 1;
        }
        if (bulundu == 0) {
            curr = curr->next;
        }
    }
    if (id == -1) {
        printf("Bu isimde yazar bulunamadi.\n");
        return;
    }
    printf("\n  ------------------------------------------\n");
    printf("   YAZAR BILGILERI                          \n");
    printf("  ------------------------------------------\n");
    printf("   ID   : %d\n", curr->yazarId);
    printf("   Ad   : %s\n", curr->ad);
    printf("   Soyad: %s\n", curr->soyad);
    printf("  ------------------------------------------\n");
    printf("\n  Bu yazara ait kitaplar:\n");
    for (i = 0; i < kyd->boyut; i++) {
        if (kyd->dizi[i].yazarId == id) {
            k = kitapBas;
            while (k != NULL) {
                if (strcmp(k->isbn, kyd->dizi[i].isbn) == 0) {
                    printf("  - %s (ISBN: %s)\n", k->ad, k->isbn);
                }
                k = k->next;
            }
        }
    }
}
KitapNode* kitapBul(KitapNode* bas, char* isbn) {
    KitapNode* curr;
    curr = bas;
    while (curr != NULL) {
        if (strcmp(curr->isbn, isbn) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

KitapNode* kitapBulAd(KitapNode* bas, char* ad) {
    KitapNode* curr;
    curr = bas;
    while (curr != NULL) {
        if (strcmp(curr->ad, ad) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

KitapNode* kitapEkle(KitapNode* bas) {
    char ad[MAX_AD];
    char isbn[MAX_ISBN];
    int adet;
    int i;
    KitapNode* yeni;
    KitapNode* curr;
    KitapNode* prev;
    curr=bas;
    prev=NULL;
    printf("\n Kitap adi : ");
    scanf(" %[^\n]", ad);
    printf(" ISBN (13 hane): ");
    scanf("%s", isbn);
    if (kitapBul(bas, isbn) != NULL) {
        printf("Bu ISBN numarasi zaten kayitli\n");
        return bas;
    }
    printf("Kopya adedi: ");
    scanf("%d", &adet);
    yeni = (KitapNode*)malloc(sizeof(KitapNode));
    strcpy(yeni->ad, ad);
    strcpy(yeni->isbn, isbn);
    yeni->adet = adet;
    yeni->ornekler = (KitapOrnegi*)malloc(sizeof(KitapOrnegi) * adet);
    for (i = 0; i < adet; i++) {
        sprintf(yeni->ornekler[i].etiketNo, "%s_%d", isbn, i + 1);
        yeni->ornekler[i].durum = RAFTA_ENUM;
        strcpy(yeni->ornekler[i].rafDurumu.rafta, "RAFTA");
    }
    yeni->next = NULL;
    while (curr != NULL && strcmp(curr->ad, ad) < 0) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        yeni->next = bas;
        bas = yeni;
    } else {
        yeni->next = curr;
        prev->next = yeni;
    }
    kitaplariDosyayaYaz(bas);
    printf("Kitap eklendi %d adet etiketi olusturuldu\n", adet);
    return bas;
}

KitapNode* kitapSil(KitapNode* bas) {
    char isbn[MAX_ISBN];
    KitapNode* curr;
    KitapNode* prev;
    curr=bas;
    prev=NULL;
    printf("\nSilinecek kitabin ISBN numarasi: ");
    scanf(" %s", isbn);
    while (curr != NULL && strcmp(curr->isbn, isbn) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("Bu ISBN'e ait kitap bulunamadi\n");
        return bas;
    }
    printf("'%s' adli kitap siliniyor...\n", curr->ad);
    if (prev == NULL) {
        bas = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr->ornekler);
    free(curr);
    kitaplariDosyayaYaz(bas);
    printf("Kitap basariyla silindi\n");
    return bas;
}

KitapNode* kitapGuncelle(KitapNode* bas) {
    char isbn[MAX_ISBN];
    KitapNode* k;
    printf("\nGuncellenecek kitabin ISBN numarasi: ");
    scanf(" %s", isbn);
    k = kitapBul(bas, isbn);
    if (k == NULL) {
        printf("  Bu ISBN'e ait kitap bulunamadi\n");
        return bas;
    }
    printf("Mevcut ad: %s\n", k->ad);
    printf("Yeni ad: ");
    scanf(" %[^\n]", k->ad);
    kitaplariDosyayaYaz(bas);
    printf("Kitap bilgileri guncellendi\n");
    return bas;
}

void kitapBilgiGoster(KitapNode* bas) {
    char ad[MAX_AD];
    int i;
    KitapNode* k;
    printf("\nKitap adi: ");
    scanf(" %[^\n]", ad);
    k = kitapBulAd(bas, ad);
    if (k == NULL) {
        printf("Bu isimde kitap bulunamadi\n");
        return;
    }
    printf("\n  ------------------------------------------\n");
    printf("   KITAP BILGILERI                          \n");
    printf("  ------------------------------------------\n");
    printf("   Ad   : %s\n", k->ad);
    printf("  ISBN : %s\n", k->isbn);
    printf("   Adet : %d\n", k->adet);
    printf("  ------------------------------------------\n");
    printf("\n  %-25s %-15s\n", "Etiket No", "Durum");
    printf("  ----------------------------------------\n");
    for (i = 0; i < k->adet; i++) {
    printf("Etiket No: %s ", k->ornekler[i].etiketNo);

    if (k->ornekler[i].durum == RAFTA_ENUM) {
        printf("Durum: Rafta\n");
    } else {
        printf("Ogrenci No: %s\n",
            k->ornekler[i].rafDurumu.ogrenciNo);
        }
    }
}

void raftakiKitaplariListele(KitapNode* bas) {
    KitapNode* curr;
    int i;
    int bulundu;
    bulundu = 0;
    curr = bas;
    printf("\n Simdi rafta bulunan kitaplar:\n");
    printf(" %-30s %-25s\n", "Kitap Adi", "Etiket No");
    printf(" -------------------------------------------------------\n");
    while (curr!=NULL) {
        for (i = 0; i < curr->adet; i++){
            if (curr->ornekler[i].durum == RAFTA_ENUM){
                printf(" %-30s %-25s\n",curr->ad,curr->ornekler[i].etiketNo);
                bulundu = 1;
            }
        }
        curr = curr->next;
    }
    if (bulundu == 0) {
        printf("Simdi rafta hic kitap yok\n");
    }
}

void zamanindaTeslimEdilmeyenleriListele(void) {
    FILE* f;
    char satir[200];
    char bugun[MAX_TARIH];
    char etiket[MAX_ETIKET];
    char ogrNo[9];
    char tarih[MAX_TARIH];
    int tip;
    int fark;
    int bulundu;
    bulundu = 0;
    bugunTarih(bugun);
    printf("\n15 gunu gecmis odunc kayitlari (Bugun: %s):\n",bugun);
    printf(" %-22s %-10s %-12s %-5s\n", "Etiket No","Ogrenci No","Odunc Tarihi","Gun");
    printf("  -------------------------------------------------------\n");
    f = fopen("OduncKayitlari.csv","r");
    if (f == NULL) {
        printf("Henuz hic odunc kaydi yok\n");
        return;
    }
    while (fgets(satir, sizeof(satir), f)) {
        sscanf(satir,"%[^,],%[^,],%d,%[^\n]",etiket,ogrNo,&tip,tarih);
        if (tip == 0) {
            fark = tarihFarkGun(tarih, bugun);
            if (fark > 15) {
                printf(" %-22s %-10s %-12s %-5d\n",etiket,ogrNo,tarih,fark);
                bulundu = 1;
            }
        }
    }
    fclose(f);
    if (bulundu == 0) {
        printf("Zamaninda teslim edilmeyen kitap yok\n");
    }
}
void kitapYazarEslestir(KitapNode* kitapBas, YazarNode* yazarBas, KitapYazarDizi* kyd) {
    char isbn[MAX_ISBN];
    int yazarId;
    KitapYazar ky;
    printf("\nEslestirme yapilacak kitabin ISBN numarasi: ");
    scanf("%s",isbn);
    if (kitapBul(kitapBas, isbn) == NULL) {
        printf("Bu ISBN'e ait kitap sistemde kayitli degil\n");
        return;
    }
    printf("Eslestirme yapilacak yazar ID:");
    scanf("%d", &yazarId);
    if (yazarBul(yazarBas, yazarId) == 0) {
        printf("Bu ID'ye ait yazar sistemde kayitli degil\n");
        return;
    }
    if (kyd->boyut >= kyd->kapasite) {
        kyd->kapasite *= 2;
        kyd->dizi = (KitapYazar*)realloc(kyd->dizi, sizeof(KitapYazar) * kyd->kapasite);
            if(kyd->dizi==NULL){
            	printf("HATA DIZI ICIN BELLEK TAHSIS EDILEMEDI\n");
			}
    }
    strcpy(ky.isbn, isbn);
    ky.yazarId = yazarId;
    kyd->dizi[kyd->boyut++] = ky;
    kitapYazarDosyayaYaz(kyd);
    printf("Kitap-yazar eslestirmesi kaydedildi\n");
}
void kitapYazarGuncelle(KitapNode* kitapBas, YazarNode* yazarBas, KitapYazarDizi* kyd) {
    char isbn[MAX_ISBN];
    int eskiId;
    int yeniId;
    int i;
    printf("\n  Guncellenecek kaydin kitap ISBN numarasi: ");
    scanf(" %13s", isbn);
    if (kitapBul(kitapBas, isbn) == NULL) {
        printf("  Bu ISBN'e ait kitap bulunamadi.\n");
        return;
    }
    printf("Degistirilecek mevcut yazar ID (-1 ise silinmis yazar): ");
    scanf("%d", &eskiId);
    printf("Yeni yazar ID: ");
    scanf("%d", &yeniId);
    if (yazarBul(yazarBas, yeniId) == 0) {
        printf("  Yeni yazar ID sistemde kayitli degil.\n");
        return;
    }
    for (i=0;i<kyd->boyut;i++) {
        if (strcmp(kyd->dizi[i].isbn, isbn) == 0 && kyd->dizi[i].yazarId == eskiId) {
            kyd->dizi[i].yazarId=yeniId;
            kitapYazarDosyayaYaz(kyd);
            printf("Yazar guncellendi.\n");
            return;
        }
    }
    printf("Bu eslestirme bulunamadi.\n");
}
void kitapOduncAl(OgrenciDizi* od, KitapNode* kitapBas) {
    char ogrNo[9];
    char isbn[MAX_ISBN];
    int ogrIdx;
    int i;
    KitapNode* k;
    OduncKayit kayit;
    printf("\nOgrenci numarasi:");
    scanf(" %s",ogrNo);
    ogrIdx = ogrenciBul(od, ogrNo);
    if (ogrIdx == -1) {
        printf("Bu numaraya ait ogrenci bulunamadi\n");
        return;
    }
    if (od->dizi[ogrIdx].puan < 0) {
        printf("Kitap verilemez! Ogrencinin puani negatif (%d)\n",od->dizi[ogrIdx].puan);
        return;
    }
    printf("Odunc alinacak kitabin ISBN:");
    scanf(" %s",isbn);
    k=kitapBul(kitapBas, isbn);
    if (k==NULL) {
        printf("Bu ISBN'e ait kitap bulunamadi\n");
        return;
    }
    for(i=0;i<k->adet; i++){
        if(k->ornekler[i].durum==RAFTA_ENUM){
            k->ornekler[i].durum=ODUNC_ENUM;
            strcpy(k->ornekler[i].rafDurumu.ogrenciNo,ogrNo);
            strcpy(kayit.etiketNo,k->ornekler[i].etiketNo);
            strcpy(kayit.ogrenciNo,ogrNo);
            kayit.islemTuru=0;
            bugunTarih(kayit.tarih);
            oduncKayitEkleDosya(&kayit);
            kitaplariDosyayaYaz(kitapBas);
            printf("Kitap basariyla odunc verildi\n");
            printf("Etiket No : %s\n", k->ornekler[i].etiketNo);
            printf("Tarih     : %s\n", kayit.tarih);
            return;
        }
    }
    printf("ISLEM BASARISIZ: Bu kitabin tum kopyalari odunc alinmis durumda\n");
}

void kitapIadeEt(OgrenciDizi* od, KitapNode* kitapBas) {
    char etiket[MAX_ETIKET];
    char ogrNo[9];
    char satir[200];
    char oduncTarih[MAX_TARIH];
    char bugun[MAX_TARIH];
    char e[MAX_ETIKET];
    char o[9];
    char t[MAX_TARIH];
    int i;
    int tip;
    int fark;
    int ogrIdx;
    KitapNode* curr;
    FILE* f;
    OduncKayit kayit;
    oduncTarih[0] = '\0';
    curr = kitapBas;
    printf("\nIade edilecek kitabin etiket numarasi:");
    scanf(" %s",etiket);
    printf("Iade eden ogrencinin numarasi: ");
    scanf(" %s",ogrNo);
    if (ogrenciBul(od,ogrNo)==-1) {
        printf("Bu numaraya ait ogrenci bulunamadi\n");
        return;
    }
    f=fopen("OduncKayitlari.csv","r");
    if(f!=NULL) {
        while(fgets(satir,sizeof(satir),f)) {
            sscanf(satir,"%[^,],%[^,],%d,%[^\n]",e,o,&tip,t);
            if(strcmp(e, etiket) == 0 && strcmp(o,ogrNo)==0&&tip==0){
                strcpy(oduncTarih,t);
            }
        }
        fclose(f);
    }
    while(curr!=NULL) {
        for(i=0;i<curr->adet;i++){
            if(strcmp(curr->ornekler[i].etiketNo, etiket)==0) {
                curr->ornekler[i].durum = RAFTA_ENUM;
                strcpy(curr->ornekler[i].rafDurumu.rafta,"RAFTA");
                strcpy(kayit.etiketNo,etiket);
                strcpy(kayit.ogrenciNo,ogrNo);
                kayit.islemTuru = 1;
                bugunTarih(bugun);
                strcpy(kayit.tarih, bugun);
                oduncKayitEkleDosya(&kayit);
                kitaplariDosyayaYaz(kitapBas);
                if (oduncTarih[0]!='\0'){
                    fark=tarihFarkGun(oduncTarih, bugun);
                    if (fark>15) {
                        ogrIdx=ogrenciBul(od, ogrNo);
                        if(ogrIdx!=-1) {
                            od->dizi[ogrIdx].puan-=10;
                            ogrencileriDosyayaYaz(od);
                            printf(" Uyari: Kitap %d gun gec iade edildi. -10 ceza puani uygulanmistir.\n",fark);
                            printf(" Guncel puan: %d\n",od->dizi[ogrIdx].puan);
                        }
                    }
                }
                printf("Kitap basariyla iade alindi. Tarih: %s\n",bugun);
                return;
            }
        }
        curr=curr->next;
    }
    printf("Bu etikete sahip kitap bulunamadi.\n");
}
void yazarListesiniSerbest(YazarNode* bas) {
    YazarNode* curr;
    YazarNode* temp;
    curr=bas;
    while(curr!=NULL) {
        temp=curr->next;
        free(curr);
        curr=temp;
    }
}

void kitapListesiniSerbest(KitapNode* bas) {
    KitapNode* curr;
    KitapNode* temp;
    curr=bas;
    while(curr!=NULL){
        temp=curr->next;
        free(curr->ornekler);
        free(curr);
        curr=temp;
    }
}
void menuGoster(void) {
    printf("\n");
    printf("  =================================================\n");
    printf("          KUTUPHANE OTOMASYON SISTEMI                \n");
    printf("  =================================================\n");
    printf("    -- OGRENCI ISLEMLERI --                        \n");
    printf("     1. Ogrenci Ekle                               \n");
    printf("     2. Ogrenci Sil                                \n");
    printf("     3. Ogrenci Guncelle                           \n");
    printf("     4. Ogrenci Bilgisi Goster                     \n");
    printf("     5. Kitap Teslim Etmemis Ogrencileri Listele   \n");
    printf("     6. Cezali Ogrencileri Listele                 \n");
    printf("     7. Tum Ogrencileri Listele                    \n");
    printf("     8. Kitap Odunc Al                             \n");
    printf("     9. Kitap Iade Et                              \n");
    printf("    -- KITAP ISLEMLERI --                          \n");
    printf("    10. Kitap Ekle                                 \n");
    printf("    11. Kitap Sil                                  \n");
    printf("    12. Kitap Guncelle                             \n");
    printf("    13. Kitap Bilgisi Goster                       \n");
    printf("    14. Raftaki Kitaplari Listele                  \n");
    printf("    15. Zamaninda Teslim Edilmeyenleri Listele     \n");
    printf("    16. Kitap-Yazar Eslestir                       \n");
    printf("    17. Kitabin Yazarini Guncelle                  \n");
    printf("    -- YAZAR ISLEMLERI --                          \n");
    printf("    18. Yazar Ekle                                 \n");
    printf("    19. Yazar Sil                                  \n");
    printf("    20. Yazar Guncelle                             \n");
    printf("    21. Yazar Bilgisi Goster                       \n");
    printf("    22. Tum Yazarlari Listele                      \n");
    printf("     0. Cikis                                      \n");
    printf("  Seciminiz: ");
}
int main() {
    OgrenciDizi od;
    KitapYazarDizi kyd;
    YazarNode* yazarBas;
    KitapNode* kitapBas;
    int secim;
    yazarBas=NULL;
    kitapBas=NULL;
    od.kapasite=10;
    od.boyut=0;
    od.dizi=(Ogrenci*)malloc(sizeof(Ogrenci) * od.kapasite);
    if(od.dizi==NULL){
        printf("HATA DIZI ICIN BELLEK TAHSIS EDILEMEDI\n");
	}
    kyd.kapasite=10;
    kyd.boyut=0;
    kyd.dizi=(KitapYazar*)malloc(sizeof(KitapYazar) * kyd.kapasite);
    if(kyd.dizi==NULL){
        printf("HATA DIZI ICIN BELLEK TAHSIS EDILEMEDI\n");
	}
    ogrencileriDosyadanOku(&od);
    yazarBas=yazarlariDosyadanOku();
    kitapBas=kitaplariDosyadanOku();
    kitapYazarDosyadanOku(&kyd);
    secim=-1;
    while(secim!=0) {
        menuGoster();
        scanf("%d", &secim);
        if(secim == 1){
        	ogrenciEkle(&od);
		}else if (secim == 2){
			ogrenciSil(&od);
		}else if (secim == 3){
			ogrenciGuncelle(&od);
		}else if (secim == 4){
			ogrenciBilgiGoster(&od);
		}else if (secim == 5){
			kitapTeslimEtmemisListele(&od, kitapBas);
		}else if (secim == 6){
			cezaliOgrenciListele(&od);
		}else if (secim == 7){
			ogrenciListele(&od);
		}else if (secim == 8){
			kitapOduncAl(&od, kitapBas);
		}else if (secim == 9){
			kitapIadeEt(&od, kitapBas);
		}else if (secim == 10){
			kitapBas = kitapEkle(kitapBas);
		}else if (secim == 11){
			kitapBas = kitapSil(kitapBas);
		}else if (secim == 12){
			kitapBas = kitapGuncelle(kitapBas);
		}else if (secim == 13){
			kitapBilgiGoster(kitapBas);
		}else if (secim == 14){
			raftakiKitaplariListele(kitapBas);
		}else if (secim == 15){
			zamanindaTeslimEdilmeyenleriListele();
		}else if (secim == 16){
			kitapYazarEslestir(kitapBas, yazarBas, &kyd);
		}else if (secim == 17){
			kitapYazarGuncelle(kitapBas, yazarBas, &kyd);
		}else if (secim == 18){
			yazarBas = yazarEkle(yazarBas);
		}else if (secim == 19){
			yazarBas = yazarSil(yazarBas, &kyd);
		}else if (secim == 20){
			yazarBas = yazarGuncelle(yazarBas);
		}else if (secim == 21){
			yazarBilgiGoster(yazarBas, &kyd, kitapBas);
		}else if (secim == 22){
			yazarListele(yazarBas);
		}else if (secim != 0){
			printf("Gecersiz secim. Lutfen tekrar deneyin\n");
		}
    }
    free(od.dizi);
    free(kyd.dizi);
    yazarListesiniSerbest(yazarBas);
    kitapListesiniSerbest(kitapBas);
    printf("\n Program kapatildi   Iyi gunler :) \n\n");
    return 0;
}
