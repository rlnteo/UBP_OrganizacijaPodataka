#include "operacije_nad_datotekom.h"

void ucitajIzTxt(FILE *serijskaDat)
{
    FILE *txt = fopen("zadatak.txt", "r");
    if (txt == NULL)
    {
        printf("\nGreska prilikom otvaranja tekstualne datoteke.");
    }
    if (serijskaDat == NULL)
    {
        printf("\nGreska prilikom otvaranja serijske datoteke.");
        fclose(txt);
        return;
    }
    SLOG_KLUB slog;
    while (fscanf(txt, "%d %s %s %d %d %d %lf",
                  &slog.sifraKluba,
                  slog.nazivKluba,
                  slog.nazivGrada,
                  &slog.postignutiGolovi,
                  &slog.primljeniGolovi,
                  &slog.bodovi,
                  &slog.godisnjiBudzet) == 7)
    {
        slog.deleted = 0;
        printf("%d %25s %20s %d %d %d %.2lf %d\n",
               slog.sifraKluba,
               slog.nazivKluba,
               slog.nazivGrada,
               slog.postignutiGolovi,
               slog.primljeniGolovi,
               slog.bodovi,
               slog.godisnjiBudzet,
               slog.deleted);
        dodajSlogUSerijsku(serijskaDat, &slog);
    }
    fclose(txt);
}

FILE *otvoriSerijskuDatoteku(char *nazivDatoteke)
{
    FILE *fajl = fopen(nazivDatoteke, "rb+");
    if (fajl == NULL)
    {
        printf("\nGreska, neuspesno otvorena datoteka.");
        return NULL;
    }
    else
    {
        printf("\nCestitam! Uspesno otvorena serijska datoteka!");
    }
    return fajl;
}

void kreirajSerijskuDatoteku(char *nazivDatoteke)
{
    FILE *fajl = fopen(nazivDatoteke, "wb");
    if (fajl == NULL)
    {
        printf("\nGreska, neuspesno otvorena datoteka.");
        return;
    }
    BLOK_KLUB blok;
    blok.slogovi_kluba[0].sifraKluba = OZNAKA_KRAJ_KL;
    fseek(fajl, 0, SEEK_SET);
    fwrite(&blok, sizeof(BLOK_KLUB), 1, fajl);
    fclose(fajl);
}

SLOG_KLUB *pronadjiSlogSerijske(FILE *fajl, int sifraKluba)
{
    if (fajl == NULL)
    {
        printf("\nGreska, neuspesno otvorena datoteka.");
        return NULL;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK_KLUB blok;
    while (fread(&blok, sizeof(BLOK_KLUB), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_KL; i++)
        {
            if (blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL)
            {
                printf("\nSlog nije pronadjen! Pokusajte ponovo.");
                break;
            }
            if (blok.slogovi_kluba[i].sifraKluba == sifraKluba)
            {
                SLOG_KLUB *temp = (SLOG_KLUB *)malloc(sizeof(SLOG_KLUB));
                memcpy(temp, &blok.slogovi_kluba[i], sizeof(SLOG_KLUB));
                return temp;
            }
        }
    }
    return NULL;
}

void dodajSlogUSerijsku(FILE *fajl, SLOG_KLUB *slog_kl)
{
    if (fajl == NULL)
    {
        printf("\nGreska, neuspesno otvorena datoteka.");
        return;
    }
    SLOG_KLUB *stariSlog = pronadjiSlogSerijske(fajl, slog_kl->sifraKluba);
    if (stariSlog != NULL)
    {
        printf("\nGreska! Slog sa tom sifrom vec postoji.");
        free(stariSlog);
        return;
    }
    BLOK_KLUB blok;
    fseek(fajl, -sizeof(BLOK_KLUB), SEEK_END);
    fread(&blok, sizeof(BLOK_KLUB), 1, fajl);

    int i;
    for(i = 0; i < FAKTOR_BLOKIRANJA_KL; i++){
        if(blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL){
            memcpy(&blok.slogovi_kluba[i], slog_kl, sizeof(SLOG_KLUB));
            break;
        }
    }
    i++;
    if(i < FAKTOR_BLOKIRANJA_KL){
        blok.slogovi_kluba[i].sifraKluba = OZNAKA_KRAJ_KL;
        fseek(fajl, -sizeof(BLOK_KLUB), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK_KLUB), 1, fajl);
        fflush(fajl);
    }else{
        fseek(fajl, -sizeof(BLOK_KLUB), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK_KLUB), 1, fajl);
        BLOK_KLUB noviBlok;
        noviBlok.slogovi_kluba[0].sifraKluba = OZNAKA_KRAJ_KL;
        fwrite(&noviBlok, sizeof(BLOK_KLUB), 1, fajl);
    }
}


void ispisiSlogSerijske(SLOG_KLUB *slog_kl)
{
    printf("%d %25s %20s %d %d %d %.2lf %d",
           slog_kl->sifraKluba,
           slog_kl->nazivKluba,
           slog_kl->nazivGrada,
           slog_kl->postignutiGolovi,
           slog_kl->primljeniGolovi,
           slog_kl->bodovi,
           slog_kl->godisnjiBudzet,
           slog_kl->deleted);
}

void ispisiSveSlogoveSerijske(FILE *fajl)
{
    if (fajl == NULL)
    {
        printf("\nGreska prilikom otvaranja serijske datoteke.");
        return;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK_KLUB blok;
    int rbBloka = 0;
    while (fread(&blok, sizeof(BLOK_KLUB), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_KL; i++)
        {
            if (blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL)
            {
                printf("B%d S%d  *\n", rbBloka, i);
                return;
            }
            if (!blok.slogovi_kluba[i].deleted)
            {
                printf("B%d S%d ", rbBloka, i);
                ispisiSlogSerijske(&blok.slogovi_kluba[i]);
            }
        }
        rbBloka++;
    }
}

void logickoBrisanjeSerijske(FILE *fajl, int sifraKluba)
{
    if (fajl == NULL)
    {
        printf("\nGreska prilikom otvaranja datoteke.");
        return;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK_KLUB blok;
    while (fread(&blok, sizeof(BLOK_KLUB), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_KL; i++)
        {
            if (blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL)
            {
                printf("\nNeuspesno obrisan slog. Pokusajte ponovo.");
                return;
            }
            if (blok.slogovi_kluba[i].sifraKluba == sifraKluba)
            {
                blok.slogovi_kluba[i].deleted = 1;
                fseek(fajl, -sizeof(BLOK_KLUB), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK_KLUB), 1, fajl);
                printf("\nUspesno obrisan slog.");
                fflush(fajl);
            }
        }
    }
}

FILE *otvoriSekvencijalnuDatoteku(char *nazivDatoteke)
{
    FILE *fajl = fopen(nazivDatoteke, "rb+");
    if (fajl == NULL)
    {
        printf("\nGreska prilikom otvaranja datoteke.");
        return NULL;
    }
    else
    {
        printf("\nCestitam, uspesno otvorena sekvencijalna datoteka.");
    }
    return fajl;
}

void kreirajSekvencijalnuDatoteku(char *nazivDatoteke)
{
    FILE *fajl = fopen(nazivDatoteke, "wb");
    if (fajl == NULL)
    {
        printf("\nGreska prilikom otvaranja datoteke.");
        return;
    }
    BLOK_GRAD blok;
    strcpy(blok.slogovi_grad[0].nazivGrada, OZNAKA_KRAJ_GR);
    fseek(fajl, 0, SEEK_SET);
    fwrite(&blok, sizeof(BLOK_GRAD), 1, fajl);
    fclose(fajl);
}

SLOG_GRAD *pronadjiSlogSekvencijalne(FILE *fajl, char *nazivGrada)
{
    if (fajl == NULL)
    {
        printf("\nGreska prilikom otvaranja datoteke.");
        return NULL;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK_GRAD blok;
    while (fread(&blok, sizeof(BLOK_GRAD), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_GR; i++)
        {
            if (strcmp(blok.slogovi_grad[i].nazivGrada, OZNAKA_KRAJ_GR) == 0 || strcmp(blok.slogovi_grad[i].nazivGrada, nazivGrada) > 0)
            {
                //NECE BITI DALJE U OVOJ DAT
                break;
            }
            if (strcmp(blok.slogovi_grad[i].nazivGrada, nazivGrada) == 0 && !blok.slogovi_grad[i].deleted)
            {
                SLOG_GRAD *slog = (SLOG_GRAD *)malloc(sizeof(SLOG_GRAD));
                if(slog == NULL){
                    return NULL;
                }
                memcpy(slog, &blok.slogovi_grad[i], sizeof(SLOG_GRAD));
                return slog;
            }
        }
    }
    return NULL;
}

void dodajSlogUSekvencijalnu(FILE *fajl, SLOG_GRAD *slog)
{
    if (fajl == NULL)
    {
        printf("\nGreska.");
        return;
    }

    SLOG_GRAD *stariSlog = pronadjiSlogSekvencijalne(fajl, slog->nazivGrada);
    if (stariSlog != NULL)
    {
        printf("\nGreska, slog vec postoji.");
        return;
    }
    BLOK_GRAD blok;
    SLOG_GRAD slogZaDodavanje;
    fseek(fajl, 0, SEEK_SET);
    int i;
    while (fread(&blok, sizeof(BLOK_GRAD), 1, fajl))
    {
        for (i = 0; i < FAKTOR_BLOKIRANJA_GR; i++)
        {
            if (strcmp(blok.slogovi_grad[i].nazivGrada, OZNAKA_KRAJ_GR) == 0)
            {
                memcpy(&blok.slogovi_grad[i], slog, sizeof(SLOG_GRAD));
                i++; // kraj se pomera za jedno mesto unapred
                if (i < FAKTOR_BLOKIRANJA_GR)
                {
                    strcpy(blok.slogovi_grad[i].nazivGrada, OZNAKA_KRAJ_GR);
                    fseek(fajl, -sizeof(BLOK_GRAD), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_GRAD), 1, fajl);
                    fflush(fajl);
                    return;
                }
                else
                {
                    fseek(fajl, -sizeof(BLOK_GRAD), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_GRAD), 1, fajl);
                    BLOK_GRAD noviBlok;
                    strcpy(noviBlok.slogovi_grad[0].nazivGrada, OZNAKA_KRAJ_GR);
                    fwrite(&noviBlok, sizeof(BLOK_GRAD), 1, fajl);
                    fflush(fajl);
                    return;
                }
            }
            else if (strcmp(blok.slogovi_grad[i].nazivGrada, slogZaDodavanje.nazivGrada) == 0)
            {
                if (!blok.slogovi_grad[i].deleted)
                {
                    printf("\nSlog se vec nalazi u datoteci.");
                    return;
                }
                else
                {
                    fseek(fajl, -sizeof(BLOK_GRAD), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_GRAD), 1, fajl);
                    fflush(fajl);
                    return;
                }
            }
            else if (strcmp(blok.slogovi_grad[i].nazivGrada, slogZaDodavanje.nazivGrada) > 0)
            {
                SLOG_GRAD tmp;
                memcpy(&tmp, &blok.slogovi_grad[i], sizeof(SLOG_GRAD));
                memcpy(&blok.slogovi_grad[i], &slogZaDodavanje, sizeof(SLOG_GRAD));
                memcpy(&slogZaDodavanje, &tmp, sizeof(SLOG_GRAD));

                if (i == FAKTOR_BLOKIRANJA_GR - 1)
                {
                    fseek(fajl, -sizeof(BLOK_GRAD), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK_GRAD), 1, fajl);
                    fflush(fajl);
                }
            }
        }
    }
}

void ispisiSlogSekvencijalne(SLOG_GRAD *slog)
{
    printf("%20s %.2lf %d",
           slog->nazivGrada,
           slog->prosecniBudzet,
           slog->deleted);
}

void ispisiSveSlogoveSekvencijalne(FILE *fajl)
{
    if (fajl == NULL)
    {
        printf("\nGreska");
        return;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK_GRAD blok;
    int rbBloka = 0;
    while (fread(&blok, sizeof(BLOK_GRAD), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_GR; i++)
        {
            if (strcmp(blok.slogovi_grad[i].nazivGrada, OZNAKA_KRAJ_GR) == 0)
            {
                printf("\nB%d S%d  *\n", rbBloka, i);
                return;
            }
            if (!blok.slogovi_grad[i].deleted)
            {
                printf("\nB%d S%d ", rbBloka, i);
                ispisiSlogSekvencijalne(&blok.slogovi_grad[i]);
            }
        }
        rbBloka++;
    }
}

void izSerijskeUSekvencijalnu(FILE *fileSerijska, FILE *fileSekvencijalna)
{
    if (fileSerijska == NULL)
    {
        printf("\nGreska prilikom otvaranja serijske datoteke.");
        return;
    }
    if (fileSekvencijalna == NULL)
    {
        printf("\nGreska prilikom otvaranja sekvencijalne datoteke.");
        return;
    }
    // koja je zapravo logika - izvlacimo jedinstvene gradove,
    // racunamo prosek po budzetu
    // upisujemo u sekvencijalnu

    // PRIPREMAMO MEMORIJU U RAM-U
    SLOG_GRAD niz[100];   // privremena lista gradova
    int broj_gradova = 0; // prati koliko zapravo gradova ima

    BLOK_KLUB blok;
    fseek(fileSerijska, 0, SEEK_SET);
    while (fread(&blok, sizeof(BLOK_KLUB), 1, fileSerijska))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_KL; i++)
        {
            if (blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL)
            {
                printf("\nKraj datoteke.");
                break;
            }
            if (!blok.slogovi_kluba[i].deleted)
            {
                // samo aktivni klubovi ulaze u racunicu
                int postoji = postojiGrad(niz, blok.slogovi_kluba[i].nazivGrada, broj_gradova);
                if (postoji == 0)
                {
                    double rez = racuanjeBudzeta(blok.slogovi_kluba[i].nazivGrada);
                    printf("Grad %s -> %.2lf \n", blok.slogovi_kluba[i].nazivGrada, rez);
                    strcpy(niz[broj_gradova].nazivGrada, blok.slogovi_kluba[i].nazivGrada);
                    niz[broj_gradova].prosecniBudzet = rez;
                    niz[broj_gradova].deleted = 0;
                    broj_gradova++;
                }
            }
        }
    }
    for (int j = 0; j < broj_gradova; j++)
    {
        dodajSlogUSekvencijalnu(fileSekvencijalna, &niz[j]);
    }
}

int postojiGrad(SLOG_GRAD *niz, char *nazivGrada, int broj)
{
    for (int i = 0; i < broj; i++)
    {
        if (strcmp(niz[i].nazivGrada, nazivGrada) == 0)
        {
            return -1;
        }
    }
    return 0;
}

double racuanjeBudzeta(char *nazivGrada)
{
    FILE *fajl = otvoriSerijskuDatoteku("test_ser_sek.dat");
    if (fajl == NULL)
    {
        printf("\nGreska pri otvaranju serijske datoteke u racunanju budzeta.");
        return 0.0;
    }

    BLOK_KLUB blok;
    fseek(fajl, 0, SEEK_SET);
    int brojac = 0;
    double budzet = 0.0;
    while (fread(&blok, sizeof(BLOK_KLUB), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_KL; i++)
        {
            if (blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL)
            {
                /* kraj */
                if (brojac == 0) {
                    fclose(fajl);
                    return 0.0;
                }
                double rez = budzet / (double)brojac;
                fclose(fajl);
                return rez;
            }
            if (!blok.slogovi_kluba[i].deleted && strcmp(blok.slogovi_kluba[i].nazivGrada, nazivGrada) == 0)
            {
                budzet += blok.slogovi_kluba[i].godisnjiBudzet;
                brojac++;
            }
        }
    }
    fclose(fajl);
    if (brojac == 0) return 0.0;
    return budzet / (double)brojac;
}
void pretragaKlubova(FILE *fajl, double minimalni, double maksimalni)
{
    if (fajl == NULL)
    {
        printf("\nGreska.");
        return;
    }
    BLOK_KLUB blok;
    int postojeKlubovi = -1;
    fseek(fajl, 0, SEEK_SET);
    while (fread(&blok, sizeof(BLOK_KLUB), 1, fajl))
    {
        for (int i = 0; i < FAKTOR_BLOKIRANJA_KL; i++)
        {
            if (blok.slogovi_kluba[i].sifraKluba == OZNAKA_KRAJ_KL)
            {
                printf("\nDosli smo do kraja datoteke.");
                return;
            }
            if (!blok.slogovi_kluba[i].deleted && blok.slogovi_kluba[i].godisnjiBudzet >= minimalni && blok.slogovi_kluba[i].godisnjiBudzet <= maksimalni)
            {
                int ukupnaGolRazlika = blok.slogovi_kluba[i].postignutiGolovi - blok.slogovi_kluba[i].primljeniGolovi;
                printf("\nKlub %s ima ukupnu gol razliku: %d.", blok.slogovi_kluba[i].nazivKluba, ukupnaGolRazlika);
                postojeKlubovi = 0;
            }
        }
    }
}
