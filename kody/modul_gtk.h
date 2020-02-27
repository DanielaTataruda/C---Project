typedef struct zakres {
    GtkWidget *textA;
    GtkWidget *textB;
    GtkWidget *textC;
    GtkWidget *textD;
    GtkWidget *nazwa_obrazka;
} Zakres;

void rysuj_wspolrzedna(GtkWidget *widget, gpointer dane);

void obszar(int A, int B, int C, int D, char *nazwa_obrazka, int skala, bool czy_srednia);

void rysuj_wlasne(GtkWidget *widget, gpointer dane);

void wlasne(GtkWidget *widget, gpointer dane);

void wspolrzedne(GtkWidget *widget, gpointer dane);

void europa(GtkWidget *widget, gpointer dane);

void afryka(GtkWidget *widget, gpointer dane);

void azja(GtkWidget *widget, gpointer dane);

void ameryki(GtkWidget *widget, gpointer dane);

void australia(GtkWidget *widget, gpointer dane);

void start();

void wybierz(GtkWidget *widget, gpointer dane);

void wybor_palety();
