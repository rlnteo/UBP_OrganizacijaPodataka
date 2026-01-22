#ifndef STRUKTURA_KLUB_H_INCLUDED
#define STRUKTURA_KLUB_H_INCLUDED

#define FAKTOR_BLOKIRANJA_KL 4
#define OZNAKA_KRAJ_KL -1

typedef struct SlogKlub{
    int sifraKluba;
    char nazivKluba[25+1];
    char nazivGrada[20+1];
    int postignutiGolovi;
    int primljeniGolovi;
    int bodovi;
    double godisnjiBudzet;
    int deleted;
}SLOG_KLUB;

typedef struct BlokKlub{
    SLOG_KLUB slogovi_kluba[FAKTOR_BLOKIRANJA_KL];
}BLOK_KLUB;

#endif // STRUKTURA_KLUB_H_INCLUDED
