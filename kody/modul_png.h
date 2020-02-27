typedef struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;

typedef struct obraz {
    Pixel **dane;
    int wysokosc;
    int szerokosc;
} Obraz;

Pixel nizina, wyzyna, gora;

static const int limit = 25000000;

Pixel *wez_pixel(int rzad, int kolumna, Obraz *O);

void stworz_PNG(char *nazwa, Obraz *O);

void konstruktor(Pixel *P, unsigned char red, unsigned char green, unsigned char blue);

void ustaw_kolory(Pixel *P, Pixel *dolny, Pixel *gorny, double wysokosc, double prog_dolny, double prog_gorny);

void zamien_wysokosc(Pixel *P, short wysokosc);

Obraz *stworz_obraz(int wysokosc, int szerokosc, short **teren);
