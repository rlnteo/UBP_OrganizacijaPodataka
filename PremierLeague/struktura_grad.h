#ifndef STRUKTURA_GRAD_H_INCLUDED
#define STRUKTURA_GRAD_H_INCLUDED

#define FAKTOR_BLOKIRANJA_GR 3
#define OZNAKA_KRAJ_GR "*"

typedef struct SlogGrad{
    char nazivGrada[20+1];
    double prosecniBudzet;
    int deleted;
}SLOG_GRAD;

typedef struct BlokGrad{
    SLOG_GRAD slogovi_grad[FAKTOR_BLOKIRANJA_GR];
}BLOK_GRAD;

#endif // STRUKTURA_GRAD_H_INCLUDED
