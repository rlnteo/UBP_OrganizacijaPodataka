#include <stdio.h>
#include <stdlib.h>

#include "operacije_nad_datotekom.h"
#include "struktura_klub.h"
#include "struktura_grad.h"

int main()
{
    printf("\nVelicina sloga kluba je %u\n", sizeof(SLOG_KLUB));
    printf("\nVelicina bloka kluba je %u\n", sizeof(BLOK_KLUB));

    printf("Velicina sloga grad je %u\n",sizeof(SLOG_GRAD));
    printf("Velicina bloka grad je %u\n",sizeof(BLOK_GRAD));

    kreirajSerijskuDatoteku("test_ser_sek.dat");
    FILE *fajl = otvoriSerijskuDatoteku("test_ser_sek.dat");
    ucitajIzTxt(fajl);

    printf("\nIspis pre logickog brisanja: ");
    printf("\n");
    ispisiSveSlogoveSerijske(fajl);

    logickoBrisanjeSerijske(fajl, 12);
    printf("\nIspis nakon logickog brisanja: ");
    printf("\n");
    ispisiSveSlogoveSerijske(fajl);

    kreirajSekvencijalnuDatoteku("sekvencijalna.dat");
    FILE *fileSekvencijalna = otvoriSekvencijalnuDatoteku("sekvencijalna.dat");

    izSerijskeUSekvencijalnu(fajl, fileSekvencijalna);
    ispisiSveSlogoveSekvencijalne(fileSekvencijalna);

    pretragaKlubova(fajl, 400.50, 600.50);

    fclose(fajl);
    fclose(fileSekvencijalna);
    return 0;
}
