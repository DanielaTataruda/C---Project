#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "modul_hgt.h"

const int MAX = 1201;
FILE *plik;
unsigned char *dane = NULL;

char *generuj_nazwe(int szerokosc, int dlugosc)
{
    char *nazwa = (char*)malloc(25 * sizeof(char));

    if(szerokosc >= 0 && dlugosc >= 0)
    {
        sprintf(nazwa, "dane/N%02dE%03d.hgt", szerokosc, dlugosc);
    }

    if(szerokosc >= 0 && dlugosc < 0)
    {
        sprintf(nazwa, "dane/N%02dW%03d.hgt", szerokosc, -dlugosc);
    }

    if(szerokosc < 0 && dlugosc >= 0)
    {
        sprintf(nazwa, "dane/S%02dE%03d.hgt", -szerokosc, dlugosc);
    }

    if(szerokosc < 0 && dlugosc < 0)
    {
        sprintf(nazwa, "dane/S%02dW%03d.hgt", -szerokosc, -dlugosc);
    }

    return nazwa;
}

bool otworz_plik(char *nazwa)
{
    printf("%s\n", nazwa);
    plik = fopen(nazwa, "r");

    if(plik == NULL)
    {
        printf("Blad przy otwieraniu pliku\n");
        return false;
    }

    if(dane == NULL)
    {
        dane = (unsigned char*)malloc(2 * MAX * MAX * sizeof(unsigned char));
    }

    fread(dane, 1, 2 * MAX * MAX, plik);

    return true;
}

short wysokosc(int pixelX, int pixelY)
{
    if(plik == NULL)
    {
        // biały
        return -32768;
        // niebieski
        // return 0;
    }

    int pozycja = (pixelX * MAX + pixelY) * 2;

    unsigned char *bufor = &dane[pozycja];

    short wysokosc = (bufor[0] << 8) + bufor[1];

    return wysokosc;
}

void zamknij_plik()
{
    if(dane != NULL)
    {
        free(dane);
        dane = NULL;
    }

    if(plik != NULL)
    {
        fclose(plik);
        plik = NULL;
    }
}
