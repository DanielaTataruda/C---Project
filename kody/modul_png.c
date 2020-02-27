#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <png.h>

#include "modul_png.h"

Pixel *wez_pixel(int rzad, int kolumna, Obraz *O)
{
    return *(O->dane + rzad) + kolumna;
}

void stworz_PNG(char *nazwa, Obraz *O)
{
    if(O->wysokosc * O->szerokosc > limit)
    {
        printf("Za duzy obraz\n");
        exit(1);
    }

    png_structp png = NULL;
    png_infop info = NULL;

    FILE *plik = fopen(nazwa, "wb");

    if(plik == NULL)
    {
        printf("Blad przy otwieraniu pliku PNG\n");
        exit(1);
    }

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(png == NULL)
    {
        printf("Blad PNG (1)\n");
        exit(1);
    }

    info = png_create_info_struct(png);

    if(info == NULL)
    {
        printf("Blad PNG (2)\n");
        exit(1);
    }

    png_set_IHDR(png, info, O->szerokosc, O->wysokosc, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_byte **tablica;

    tablica = (png_byte**)png_malloc(png, O->wysokosc * sizeof(png_byte*));

    for(int i = 0; i < O->wysokosc; i++)
    {
        png_byte *rzad;
        rzad = (png_byte*)png_malloc(png, O->szerokosc * sizeof(unsigned char) * 3);

        *(tablica + i) = rzad;

        for(int j = 0; j < O->szerokosc; j++)
        {
            Pixel *aktualny = wez_pixel(i, j, O);
            *rzad = aktualny->red;
            rzad++;
            *rzad = aktualny->green;
            rzad++;
            *rzad = aktualny->blue;
            rzad++;
        }
    }

    png_init_io(png, plik);
    png_set_rows(png, info, tablica);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

    for(int i = 0; i < O->wysokosc; i++)
        free(*(tablica + i));

    free(tablica);

    fclose(plik);
}

void konstruktor(Pixel *P, unsigned char red, unsigned char green, unsigned char blue)
{
    P->red = red;
    P->green = green;
    P->blue = blue;
}

void ustaw_kolory(Pixel *P, Pixel *dolny, Pixel *gorny, double wysokosc, double prog_dolny, double prog_gorny)
{
    double roznica1 = wysokosc - prog_dolny;
    double roznica2 = prog_gorny - wysokosc;

    // srednia wazona
    P->red = (unsigned char)((roznica2 * (double)dolny->red + roznica1 * (double)gorny->red) / (roznica1 + roznica2));

    P->green = (unsigned char)((roznica2 * (double)dolny->green + roznica1 * (double)gorny->green) / (roznica1 + roznica2));

    P->blue = (unsigned char)((roznica2 * (double)dolny->blue + roznica1 * (double)gorny->blue) / (roznica1 + roznica2));
}

void zamien_wysokosc(Pixel *P, short wysokosc)
{
    if(wysokosc == -32768)
    {
        // pixel bez danych
        konstruktor(P, 255, 255, 255);
    }
    else if(wysokosc <= 0)
    {
        // woda
        konstruktor(P, 0, 0, 255);
    }
    else if(wysokosc <= 400)
    {
        // niziny
        ustaw_kolory(P, &nizina, &wyzyna, wysokosc, 0, 400);
    }
    else if(wysokosc <= 1500)
    {
        // wyzyny
        ustaw_kolory(P, &wyzyna, &gora, wysokosc, 400, 1500);
    }
    else if(wysokosc <= 5000)
    {
        // gory
        Pixel czarny;
        konstruktor(&czarny, 0, 0, 0);
        ustaw_kolory(P, &gora, &czarny, wysokosc, 1500, 5000);
    }
    else
    {
        // najwyzsze gory
        konstruktor(P, 0, 0, 0);
    }
}

Obraz *stworz_obraz(int wysokosc, int szerokosc, short **teren)
{
    Obraz *O = (Obraz*)malloc(sizeof(Obraz));

    O->wysokosc = wysokosc;
    O->szerokosc = szerokosc;

    if(O->wysokosc * O->szerokosc > limit)
    {
        printf("Za duzy obraz\n");
        exit(1);
    }

    O->dane = (Pixel**)malloc(wysokosc * sizeof(Pixel*));

    for(int i = 0; i < wysokosc; i++)
    {
        Pixel *rzad = (Pixel*)malloc(szerokosc * sizeof(Pixel));
        *(O->dane + i) = rzad;

        for(int j = 0; j < szerokosc; j++)
        {
            zamien_wysokosc(rzad, teren[i][j]);
            rzad++;
        }
    }

    return O;
}
