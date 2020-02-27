#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <png.h>
#include <gtk/gtk.h>

#include "modul_hgt.h"
#include "modul_png.h"
#include "modul_gtk.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    wybor_palety();

    start();

    gtk_main();

    return 0;
}
