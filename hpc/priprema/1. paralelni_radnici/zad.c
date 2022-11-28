#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#define NELEM 100000000
#define NOTIMPLEMENTED vector<int>()
#define BROJ_KOFA 29
#define ZELJENI_BROJ_RADNIKA 8
#define MAKS_TEZINA_KOFE 15


typedef struct
{
    int idRadnika;
    int ukupanRad;
} ucinakRadnika;

void fasadaPremestiKofu(int idRadnika, int brojKofe)
{
    #ifdef STAMPAJ_INFORMACIJE
    printf("Radnik %d premešta kofu %d\n", idRadnika, brojKofe);
    #endif
}

ucinakRadnika sekvencijalniRadnici() {

    //vektor koji čuva težinu svake od kofa
    int tezineKofa[BROJ_KOFA]; 

    //centralizovani zapisnik; vektor koji čuva količinu posla koju je uradio svaki od radnika, odnosno ukupnu težinu prenetih kofa
    int kolicinaZavrsenogPosla[ZELJENI_BROJ_RADNIKA] = { 0 }; 

    #ifdef STAMPAJ_INFORMACIJE
    printf("Sekvencijalne tezine: ");
    #endif

    //postavljanje težina kofa, zaduženje "preduzeća"
    for (int i = 0; i < BROJ_KOFA; i++)
    {
        //način na koji se nasumično postavlja težina svake kofe; iskoristiti istu ovu računicu i u paralelnom algoritmu
        tezineKofa[i] = rand() % MAKS_TEZINA_KOFE + 1;

        #ifdef STAMPAJ_INFORMACIJE
        printf("%d ", tezineKofa[i]);
        #endif
    }
    printf("\n");


    //broj kofa dodeljen svakom od radnika; ukupan broj kofa deli se na broj ranika i zaokružuje na gornje celo
    //prvih ovoliko kofa premešta prvi radnik, sledećih toliko drugi i td.; poslednji radnik će prestiti manje kofa,
    //ukoliko broj kofa nije deljiv sa brojem radnika
    //iskoristiti istu računicu i u paralelnom algoritmu
    int brojKofaPoRadniku = (int) (((float) BROJ_KOFA) / ((float) ZELJENI_BROJ_RADNIKA) + 0.5);


    //premeštanje kofa, zaduženje "radnika"
    for (int i = 0; i < BROJ_KOFA; i++)
    {
        //određivanje id-a trenutnog radnika
        int trenutniRadnik = i / brojKofaPoRadniku;

        //simulacija premeštanja kofe; funkcija ne radi ništa važno, samo ispisuje poruku ukoliko je definisan odgovarajući makro
        fasadaPremestiKofu(trenutniRadnik, i);

        //ažuriranje zapisnika, odnosno odgovarajućeg polja u vektoru
        kolicinaZavrsenogPosla[trenutniRadnik] += tezineKofa[i];

        #ifdef STAMPAJ_INFORMACIJE
        printf("Radnik %d je uradio sledecu kolicinu posla: %d; dodaje se element %d: %d\n", 
            trenutniRadnik, kolicinaZavrsenogPosla[trenutniRadnik], i, tezineKofa[i]);
        #endif
    }

    printf("\n\n");

    ucinakRadnika ur;

    //algoritam kojim se određuje ko je najvredniji radnik i koliko je posla uradio
    //zaduženje "preduzeća"
    ur.ukupanRad = kolicinaZavrsenogPosla[0];
    ur.idRadnika = 0;
    for (int i = 1; i < ZELJENI_BROJ_RADNIKA; i++) 
    {
        if (kolicinaZavrsenogPosla[i] > ur.ukupanRad)
        {
            ur.ukupanRad = kolicinaZavrsenogPosla[i];
            ur.idRadnika = i;
        }
    }

    return ur;

}

ucinakRadnika paralelniRadnici() {

    //iskoristiti ovaj broj u "schedule" klauzuli "omp for" direktive, na sledeći način: schedule(static, brojKofaPoRadniku)
    //ovo se radi kako bi raspodela posla po radnicima bila ista kao kod sekvencijalnog programa i kako bi rešenja bila uporediva
    int brojKofaPoRadniku = (int) (((float) BROJ_KOFA) / ((float) ZELJENI_BROJ_RADNIKA) + 0.5);

    //promenljiva kojom svaki radnik vodi evidenciju o količini posla koju je uradio
    //zabranjeno je koristiti niz u ove svrhe, kao što je rađeno u sekvencijalnom algoritmu
    int ukupanRadJednogRadnika = 0;

    //promenljiva koja se po potrebi ažurira kako bi na kraju algoritma sadržala podatke o najvrednijem radniku i njegovoj količini posla
    //ova promenljiva je povratna vrednost funkcije
    ucinakRadnika ur;
    ur.idRadnika = -1;
    ur.ukupanRad = -1;

    //vektor koji čuva težine pojedinačnih kofa; njih postavlja jedan od radnika koristeći računicu iz sekvencijalnog algoritma
    int tezineKofa[BROJ_KOFA];

    //TODO: implementirati paralelni algoritam

    printf("\n\n");

    return ur;
}


int main() {

    time_t randVreme = time(0);

    srand(randVreme);
    ucinakRadnika paralelniRez = paralelniRadnici();


    srand(randVreme);
    ucinakRadnika sekvencijalniRez = sekvencijalniRadnici();
    
    printf("---- Paralelni rezultat ----\nNajvredniji radnik: %d\nKolicina uradjenog posla: %d\n\n", paralelniRez.idRadnika, paralelniRez.ukupanRad);
    printf("---- Sekvencijalni razultat ----\nNajvredniji radnik: %d\nKolicina uradjenog posla: %d\n\n", sekvencijalniRez.idRadnika, sekvencijalniRez.ukupanRad);

    return 0;
}
