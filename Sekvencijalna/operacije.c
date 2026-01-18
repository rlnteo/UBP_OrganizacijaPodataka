#include "operacije.h"
#include "struktura_podataka.h"

void citajTxtUpisiUBinarnu(char *nazivTxt){
    FILE *txt = fopen(nazivTxt, "r");
    if(txt == NULL){
        printf("\nGreska prilikom otvaranja tekstualne datoteke.");
        return;
    }else{
        printf("\nUspesno otvorena tekstualna datoteka.");
    }
    kreirajDatoteku("podaci.dat");
    FILE *bin = otvoriDatoteku("podaci.dat");
    if(bin == NULL){
        printf("\nGreska prilikom otvaranja binarne datoteke.");
        return;
    }else{
        printf("\nUspesno otvorena binarna datoteka.");
    }
    SLOG *temp = (SLOG*)malloc(sizeof(SLOG));
    if(temp ==  NULL) return;

    char tipString[9];
    fseek(txt, 0, SEEK_SET);
    while(fscanf(txt, "%10s %6s %13s %15s %16s %9s %d",
                 temp->evidBroj,
                 temp->brClanske,
                 temp->ISBN,
                 temp->nazivKnjige,
                 temp->datumVreme,
                 tipString,
                 &temp->deleted)==7){
        if(strcmp(tipString, "ACTIVE") == 0) temp->stPozajmice = ACTIVE;
        else if(strcmp(tipString, "RETURNED") == 0) temp->stPozajmice = RETURNED;
        dodajSlog(bin, temp);
    }
    fflush(bin);
    fclose(txt);
    fclose(bin);
    free(temp);
}

FILE *otvoriDatoteku(char *nazivDatoteke){
    FILE *fajl = fopen(nazivDatoteke, "rb+");
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja binarne datoteke.");
        return;
    }else{
        printf("\nCestitam, uspesno ste otvorili binarnu datoteku.");
    }
    return fajl;
}

void kreirajDatoteku(char *nazivDatoteke){
    FILE *fajl = fopen(nazivDatoteke, "wb");
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja datoteke.");
        return;
    }else{
        BLOK blok;
        strcpy(blok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        fclose(fajl);
    }
}

SLOG *pronadjiSlog(FILE *fajl, char *evidBroj){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja datoteke. Pokusajte ponovo.");
        return;
    }else{
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0 || atoi(blok.slogovi[i].evidBroj) > atoi(evidBroj)){
                    return NULL;
                }
                if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0){
                    SLOG *slog = (SLOG*)malloc(sizeof(SLOG));
                    memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                    return slog;
                }
            }
        }
        return NULL;
    }
}

void dodajSlog(FILE *fajl, SLOG *slog){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja fajla.");
        return;
    }else{
        SLOG slogKojiUpisujemo;
        memcpy(&slogKojiUpisujemo, slog, sizeof(SLOG));

        fseek(fajl, 0, SEEK_SET);
        BLOK blok;

        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                    memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));

                    //ako jos ima slobodnih blokova
                    if(i != FAKTOR_BLOKIRANJA-1){
                        strcpy(blok.slogovi[i+1].evidBroj, OZNAKA_KRAJA_DATOTEKE);
                        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOK), 1, fajl);
                        printf("\nNovi slog evidentiran.");
                        return;
                    }else{
                        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOK), 1, fajl);

                        //pravimo novi blok
                        BLOK noviBlok;
                        strcpy(noviBlok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
                        fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
                        printf("\nNovi slog evidentiran u datoteci. Takodje je dodat i novi blok.");
                        return;
                    }
                }else if(strcmp(blok.slogovi[i].evidBroj, slogKojiUpisujemo.evidBroj) == 0){
                    printf("\nSlog sa evidencionim brojem vec postoji. Pokusajte ponovo.");
                    return;
                }else if(atoi(blok.slogovi[i].evidBroj) > atoi(slogKojiUpisujemo.evidBroj)){
                    SLOG temp;
                    memcpy(&temp, &blok.slogovi[i], sizeof(SLOG));
                    memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));
                    memcpy(&slogKojiUpisujemo, &temp, sizeof(SLOG));

                    if(i == FAKTOR_BLOKIRANJA - 1){
                        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOK), 1, fajl);
                        fflush(fajl);
                    }
                }
            }
        }
    }
}

void ispisiSlog(SLOG* slog){
    char tipString[8];
    printf("%s %s %s %s %s %s %d",
           slog->evidBroj,
           slog->brClanske,
           slog->ISBN,
           slog->nazivKnjige,
           slog->datumVreme,
           tipString,
           &slog->deleted);
        if(strcmp(tipString, "ACTIVE") == 0) slog->stPozajmice = ACTIVE;
        else if(strcmp(tipString, "RETURNED") == 0) slog->stPozajmice =RETURNED;
}

void ispisiSveSlogove(FILE *fajl){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja fajla. Pokusajte ponovo.");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;
    int rbBloka = 0;
    printf("BL SL EvidBroj  BrClanske  ISBN  Naziv  DatumVreme  Status  Deleted");
    while(fread(&blok, sizeof(BLOK), 1, fajl)){
        for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                printf("B%d S%d *\n", rbBloka, i);
                return;
            }
            if(!blok.slogovi[i].deleted){
                printf("B%d S%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
        }
    }
}

void modifikujSlog(FILE *fajl, char *evidBroj, char *nazivKnjige){
    if(fajl == NULL){
        printf("\nGreska.");
        return;
    }else{
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                    printf("\nNema sloga za brisanje.");
                    return;
                }
                if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted){
                    strcpy(blok.slogovi[i].nazivKnjige, nazivKnjige);
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);
                    printf("\nSlog uspesno modidfikovan.");
                    return;
                }
            }
        }
    }
}


void logikcoBrisanjeSloga(FILE *fajl, char *evidBroj){
    if(fajl == NULL){
        printf("\nGreska.");
        return;
    }else{
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                    printf("Ne mozemo pronaci slog za brisanje.");
                    return;
                }
                if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0){
                    blok.slogovi[i].deleted = 1;

                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);

                    printf("\nSLOG JE LOGICKI OBRISAN.");
                    return;
                }
            }
        }
    }
}
