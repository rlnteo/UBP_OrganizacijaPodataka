#ifndef STRUKTURA_PODATAKA_H_INCLUDED
#define STRUKTURA_PODATAKA_H_INCLUDED

#define FAKTOR_BLOKIRANJA 4
#define OZNAKA_KRAJA_DATOTEKE "******"

typedef enum StatusPozajmice{
    ACTIVE,
    RETURNED
}STATUS;

typedef struct Slog{
    char evidBroj[10+1];
    char brClanske[6+1];
    char ISBN[13+1];
    char nazivKnjige[15+1];
    char datumVreme[16+1];
    STATUS stPozajmice;
    int deleted;
}SLOG;

typedef struct Blok{
    SLOG slogovi[FAKTOR_BLOKIRANJA];
}BLOK;

#endif // STRUKTURA_PODATAKA_H_INCLUDED
