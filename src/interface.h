#ifndef INTERFACE
#define INTERFACE
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "callback.h"
#include "drawelem.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

GtkWidget *create_main_window (void);
GtkWidget *create_about_dialog (void);
GtkWidget *create_open_dialog (void);
GtkWidget *create_save_dialog (void);
GtkWidget *create_pal_dialog (void);

#endif
