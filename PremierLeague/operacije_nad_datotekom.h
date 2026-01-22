#ifndef OPERACIJE_NAD_DATOTEKOM_H_INCLUDED
#define OPERACIJE_NAD_DATOTEKOM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struktura_klub.h"
#include "struktura_grad.h"

FILE *otvoriSerijskuDatoteku(char *nazivDatoteke);
void kreirajSerijskuDatoteku(char *nazivDatoteke);
SLOG_KLUB *pronadjiSlogSerijske(FILE *fajl, int sifraKluba);
void dodajSlogUSerijsku(FILE *fajl, SLOG_KLUB *slog_kl);
void ispisiSlogSerijske(SLOG_KLUB *slog_kl);
void ispisiSveSlogoveSerijske(FILE *fajl);
void logickoBrisanjeSerijske(FILE *fajl, int sifraKluba);

FILE *otvoriSekvencijalnuDatoteku(char *nazivDatoteke);
void kreirajSekvencijalnuDatoteku(char *nazivDatoteke);
SLOG_GRAD *pronadjiSlogSekvencijalne(FILE *fajl, char *nazivGrada);
void dodajSlogUSekvencijalnu(FILE *fajl, SLOG_GRAD *slog);
void ispisiSlogSekvencijalne(SLOG_GRAD *slog);
void ispisiSveSlogoveSekvencijalne(FILE *fajl);
void izSerijskeUSekvencijalnu(FILE *fileSerijska, FILE *fileSekvencijalna);

int postojiGrad(SLOG_GRAD *slog, char *nazivGrada, int broj);
double racuanjeBudzeta(char *nazivGrada);
void pretragaKlubova(FILE *fajl, double minimalni, double maksimalni);

#endif // OPERACIJE_NAD_DATOTEKOM_H_INCLUDED
