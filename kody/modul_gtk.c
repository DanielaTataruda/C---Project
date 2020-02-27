#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <png.h>
#include <gtk/gtk.h>

#include "modul_gtk.h"
#include "modul_hgt.h"
#include "modul_png.h"

void rysuj_wspolrzedna(GtkWidget *widget, gpointer dane)
{
    char nazwa[20], sciezka[50];
    strcpy(nazwa, gtk_entry_get_text(GTK_ENTRY(dane)));
    printf("Otwieram plik %s.hgt\n", nazwa);

    sprintf(sciezka, "dane/%s.hgt", nazwa);

    const int MAX = 1201;

    if(otworz_plik(sciezka) == false)
    {
        return;
    }
    short **teren = (short**)malloc(MAX * sizeof(short*));

    for(int i = 0; i < MAX; i++)
    {
        teren[i] = (short*)malloc(MAX * sizeof(short));
        for(int j = 0; j < MAX; j++)
        {
            teren[i][j] = wysokosc(i, j);
        }
    }

    zamknij_plik();

    Obraz *O = stworz_obraz(MAX, MAX, teren);

    char nazwa_obrazka[50];
    sprintf(nazwa_obrazka, "obrazki/%s.png", nazwa);
    stworz_PNG(nazwa_obrazka, O);

    GtkWidget *podglad = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *image = gtk_image_new_from_file(nazwa_obrazka);

    gtk_window_set_title(GTK_WINDOW(podglad), nazwa_obrazka);
    gtk_window_set_position(GTK_WINDOW(podglad), GTK_WIN_POS_CENTER);
    gtk_container_add(GTK_CONTAINER(podglad), image);

    gtk_widget_show_all(podglad);
}

void obszar(int A, int B, int C, int D, char *nazwa_obrazka, int skala, bool czy_srednia)
{
    const int N1 = B, N2 = A, E1 = C, E2 = D;

    const int MAX_I = skala * (N1 - N2 + 1), MAX_J = skala * (E2 - E1 + 1);

    short **teren = (short**)malloc(MAX_I * sizeof(short*));

    for(int i = 0; i < MAX_I; i++)
        teren[i] = (short*)malloc(MAX_J * sizeof(short));

    for(int i = N1; i >= N2; i--)
    {
        for(int j = E1; j <= E2; j++)
        {
            otworz_plik( generuj_nazwe(i, j) );

            for(int i2 = 0; i2 < skala; i2++)
            {
                for(int j2 = 0; j2 < skala; j2++)
                {
                    if(czy_srednia == true)
                    {
                        // liczymy srednia
                        int srednia = 0, ile_zlych = 0;
                        for(int i3 = 1200 / skala * i2; i3 < 1200 / skala * i2 + 1200 / skala; i3++)
                        {
                            for(int j3 = 1200 / skala * j2; j3 < 1200 / skala * j2 + 1200 / skala; j3++)
                            {
                                short wartosc = wysokosc(i3, j3);
                                if(wartosc != -32768)
                                {
                                    srednia += wartosc;
                                }
                                else
                                {
                                    ile_zlych++;
                                }
                            }
                        }

                        if(ile_zlych != 1200 / skala * 1200 / skala)
                        {
                            srednia /= (1200 / skala * 1200 / skala - ile_zlych);
                        }
                        else
                        {
                            srednia = -32768;
                        }

                        int pozX = (N1 - i) * skala + i2;
                        int pozY = (j - E1) * skala + j2;

                        teren[pozX][pozY] = srednia;
                    }
                    else
                    {
                        int wartosc = wysokosc(1200 / skala * i2, 1200 / skala * j2);

                        int pozX = (N1 - i) * skala + i2;
                        int pozY = (j - E1) * skala + j2;

                        teren[pozX][pozY] = wartosc;
                    }
                }
            }

            printf("Wczytano %d %d\n", i, j);
            zamknij_plik();
        }
    }

    Obraz *O = stworz_obraz(MAX_I, MAX_J, teren);

    stworz_PNG(nazwa_obrazka, O);

    GtkWidget *podglad = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *image = gtk_image_new_from_file(nazwa_obrazka);

    gtk_window_set_title(GTK_WINDOW(podglad), nazwa_obrazka);
    gtk_window_set_position(GTK_WINDOW(podglad), GTK_WIN_POS_CENTER);
    gtk_container_add(GTK_CONTAINER(podglad), image);

    gtk_widget_show_all(podglad);
}

void rysuj_wlasne(GtkWidget *widget, gpointer dane)
{
    char pomoc[20], nazwa_obrazka[30];
    Zakres *Z = dane;

    strcpy(pomoc, gtk_entry_get_text(GTK_ENTRY(Z->textA)));
    int A = atoi(pomoc);
    printf("%d\n", A);

    strcpy(pomoc, gtk_entry_get_text(GTK_ENTRY(Z->textB)));
    int B = atoi(pomoc);
    printf("%d\n", B);

    strcpy(pomoc, gtk_entry_get_text(GTK_ENTRY(Z->textC)));
    int C = atoi(pomoc);
    printf("%d\n", C);

    strcpy(pomoc, gtk_entry_get_text(GTK_ENTRY(Z->textD)));
    int D = atoi(pomoc);
    printf("%d\n", D);

    strcpy(pomoc, gtk_entry_get_text(GTK_ENTRY(Z->nazwa_obrazka)));
    sprintf(nazwa_obrazka, "obrazki/%s.png", pomoc);

    if(A > B || C > D)
    {
        printf("Bledny zakres\n");
        return;
    }

    obszar(A, B, C, D, nazwa_obrazka, 100, true);
}

void wlasne(GtkWidget *widget, gpointer dane)
{
    if(dane != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(dane));
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Mapy");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    GtkWidget *label;

    label = gtk_label_new("Proszę podać zakres współrzędnych geograficznych w formacie:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 4, 1);

    label = gtk_label_new("min S/N, max S/N, min W/E, max W/E");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 4, 1);


    label = gtk_label_new("liczba ujemna oznacza S/W, a nieujemna N/E");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 4, 1);


    label = gtk_label_new("np. -5, 2, 3, 5 -- oznacza zakres S05 do N02 i E003 do E005");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 4, 1);

    GtkWidget *textA = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(textA), 5);
    gtk_entry_set_alignment(GTK_ENTRY(textA), 0);
    gtk_entry_set_text(GTK_ENTRY(textA), "");
    gtk_grid_attach(GTK_GRID(grid), textA, 0, 4, 1, 1);

    GtkWidget *textB = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(textB), 5);
    gtk_entry_set_alignment(GTK_ENTRY(textB), 0);
    gtk_entry_set_text(GTK_ENTRY(textB), "");
    gtk_grid_attach(GTK_GRID(grid), textB, 1, 4, 1, 1);

    GtkWidget *textC = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(textC), 5);
    gtk_entry_set_alignment(GTK_ENTRY(textC), 0);
    gtk_entry_set_text(GTK_ENTRY(textC), "");
    gtk_grid_attach(GTK_GRID(grid), textC, 2, 4, 1, 1);

    GtkWidget *textD = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(textD), 5);
    gtk_entry_set_alignment(GTK_ENTRY(textD), 0);
    gtk_entry_set_text(GTK_ENTRY(textD), "");
    gtk_grid_attach(GTK_GRID(grid), textD, 3, 4, 1, 1);

    label = gtk_label_new("Nazwa wygenerowanego pliku PNG:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 5, 2, 1);


    GtkWidget *nazwa_obrazka = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(nazwa_obrazka), 20);
    gtk_entry_set_alignment(GTK_ENTRY(nazwa_obrazka), 0);
    gtk_entry_set_text(GTK_ENTRY(nazwa_obrazka), "");
    gtk_grid_attach(GTK_GRID(grid), nazwa_obrazka, 2, 5, 2, 1);

    Zakres *dane2 = (Zakres*)malloc(sizeof(Zakres));
    dane2->textA = textA;
    dane2->textB = textB;
    dane2->textC = textC;
    dane2->textD = textD;
    dane2->nazwa_obrazka = nazwa_obrazka;

    GtkWidget *button;

    button = gtk_button_new_with_label("Stwórz");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(rysuj_wlasne), dane2);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 4, 1);

    gtk_widget_show_all(window);
}

void wspolrzedne(GtkWidget *widget, gpointer dane)
{
    if(dane != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(dane));
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Mapy");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    GtkWidget *label;

    label = gtk_label_new("Proszę podać współrzędne geograficzne w formacie:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    label = gtk_label_new("N00E000");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

    GtkWidget *text = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(text), 20);
    gtk_entry_set_alignment(GTK_ENTRY(text), 0);
    gtk_entry_set_text(GTK_ENTRY(text), "");
    gtk_grid_attach(GTK_GRID(grid), text, 0, 2, 1, 1);

    GtkWidget *button;

    button = gtk_button_new_with_label("Stwórz");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(rysuj_wspolrzedna), text);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);

    gtk_widget_show_all(window);
}

void europa(GtkWidget *widget, gpointer dane)
{
    obszar(35, 58, 5, 35, "obrazki/europa.png", 30, false);
}

void afryka(GtkWidget *widget, gpointer dane)
{
    obszar(-34, 37, -17, 51, "obrazki/afryka.png", 20, false);
}

void azja(GtkWidget *widget, gpointer dane)
{
    obszar(1, 77, 26, 179, "obrazki/azja.png", 10, false);
}

void ameryki(GtkWidget *widget, gpointer dane)
{
    obszar(-55, 71, -169, -55, "obrazki/ameryki.png", 10, false);
}

void australia(GtkWidget *widget, gpointer dane)
{
    obszar(-39, -10, 113, 153, "obrazki/australia.png", 30, true);
}

void start()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Mapy");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    GtkWidget *button;

    button = gtk_button_new_with_label("Współrzędne geograficzne");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wspolrzedne), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label("Własny obszar");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wlasne), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label("Europa");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(europa), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    button = gtk_button_new_with_label("Afryka");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(afryka), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);

    button = gtk_button_new_with_label("Azja");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(azja), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 4, 1, 1);

    button = gtk_button_new_with_label("Ameryki");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(ameryki), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);

    button = gtk_button_new_with_label("Australia");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(australia), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 1, 1);

    gtk_widget_show_all(window);
}

void wybierz(GtkWidget *widget, gpointer dane)
{
    if(dane == (void *)1)
    {
        konstruktor(&nizina, 0, 200, 0);
        konstruktor(&wyzyna, 255, 255, 0);
        konstruktor(&gora, 255, 0, 0);
    }
    if(dane == (void *)2)
    {
        konstruktor(&nizina, 255, 0, 0);
        konstruktor(&wyzyna, 255, 255, 0);
        konstruktor(&gora, 0, 255, 0);
    }
    if(dane == (void *)3)
    {
        konstruktor(&nizina, 241, 173, 244);
        konstruktor(&wyzyna, 194, 101, 198);
        konstruktor(&gora, 115, 29, 119);
    }
    if(dane == (void *)4)
    {
        konstruktor(&nizina, 255, 255, 255);
        konstruktor(&wyzyna, 100, 100, 100);
        konstruktor(&gora, 0, 0, 0);
    }
    if(dane == (void *)5)
    {
        konstruktor(&nizina, 255, 155, 255);
        konstruktor(&wyzyna, 0, 0, 255);
        konstruktor(&gora, 0, 255, 255);
    }
}

void wybor_palety()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Paleta kolorów");
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    konstruktor(&nizina, 0, 200, 0);
    konstruktor(&wyzyna, 255, 255, 0);
    konstruktor(&gora, 255, 0, 0);

    GtkWidget *label = gtk_label_new("Wybór palety kolorów: (niziny, wyzyny, gory)");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    GtkWidget *button;

    button = gtk_button_new_with_label("Domyślny (zielony, żółty, czerwony)");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wybierz), (void *)1);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label("Odwrócenie (czerwony, żółty, zielony)");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wybierz), (void *)2);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    button = gtk_button_new_with_label("Kobiecy (odcienie różu)");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wybierz), (void *)3);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);

    button = gtk_button_new_with_label("Czarno-biały (biały, szary, czarny)");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wybierz), (void *)4);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 4, 1, 1);

    button = gtk_button_new_with_label("Inwersja (255 - domyślny)");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(wybierz), (void *)5);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);

    gtk_widget_show_all(window);
}
