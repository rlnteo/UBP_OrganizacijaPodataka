#include <stdio.h>
#include <stdlib.h>

#include "operacije.h"

int main()
{
    int running = 1;
    int userInput;

    FILE *fajl = NULL;

    char txt[20];
    printf("Unesite naziv tekstualne datoteke: ");
    scanf("%s", txt);
    getc(stdin);

    citajTxtUpisiUBinarnu(txt);
    printf("\n");

    while(running){
        printf("\n-----------Opcije meni-----------");
        printf("\n1) Otvori datoteku");
        printf("\n2) Kreiraj datoteku");
        printf("\n3) Unesi novi slog");
        printf("\n4) Ispisi sve slogove");
        printf("\n5) Pretrazi datoteku");
        printf("\n6) Modifikuj slog");
        printf("\n7) Logicki obrisi slog");
        printf("\n0) Izadji iz programa");
        printf("\nOpcija: ");

        scanf("%d", &userInput);
        getc(stdin);

        switch(userInput){
        case 1:
        {
            char nazivDatoteke[20];
            printf("\nUnesite naziv datoteke koju zelite da otvorite: ");
            scanf("%s", &nazivDatoteke[0]);
            fajl = otvoriDatoteku(nazivDatoteke);
            printf("\n");
            break;
        }
        case 2:
        {
            char nazivDatoteke[20];
            printf("\nKako zelite da nazovete datoteku? Prosledite naziv: ");
            scanf("%s", nazivDatoteke);
            kreirajDatoteku(nazivDatoteke);
            printf("\n");
            break;
        }
        case 3:
        {
            if(fajl ==  NULL){
                printf("\nNeuspesno otvorena datoteka.");
                return;
            }
            SLOG temp;
            char tipString[8];
            printf("\nUnesite: ");
            printf("\nEvidencioni broj   Broj Clanske karte:    ISBN:   Naziv knjige:   Datum:   Status pozajmice:\n");
            scanf("%-10s %-6s %-13s %-15s %-16s %-8s",
                  temp.evidBroj,
                  temp.brClanske,
                  temp.ISBN,
                  temp.nazivKnjige,
                  temp.datumVreme,
                  tipString);
            if(strcmp(tipString, "ACTIVE") == 0) temp.stPozajmice = ACTIVE;
            else if(strcmp(tipString, "RETURNED") == 0) temp.stPozajmice = RETURNED;

            temp.deleted = 0;
            dodajSlog(fajl, &temp);
            break;
        }
        case 4:
        {
            ispisiSveSlogove(fajl);
            break;
        }
        case 5:
        {
            //pretraga datoteke
            char evidBroj[10+1];
            printf("Unesite evid. broj trazenog prijema: ");
            scanf("%s", evidBroj);
            SLOG *slog = pronadjiSlog(fajl, evidBroj);
            if (slog == NULL) {
                printf("Trazeni slog ne postoji!\n");
            } else {
                printf("\nPodaci....\n");
                ispisiSlog(slog);
                printf("\n");
            }
            printf("\n");
            break;
        }
        case 6:
        {
            //modifikuj
            char evidBroj[10+1];
            printf("\nUnesite evid. broj: ");
            scanf("%s", evidBroj);

            char naziv[15+1];
            printf("\nUnesite naziv: ");
            scanf("%s", naziv);
            modifikujSlog(fajl, evidBroj, naziv);
            break;
        }
        case 7:
        {
            char evidBroj[10+1];
            printf("\nUnesi ev br: ");
            scanf("%s", evidBroj);
            logikcoBrisanjeSloga(fajl, evidBroj);
            break;
        }
        case 0:
        {
            if(fajl != NULL){
                fclose(fajl);
            }
            running = 0;
        }
        }
    }

    return 0;
}
