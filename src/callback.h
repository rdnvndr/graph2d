#ifndef CALLBACK
#define CALLBACK
#include <gtk/gtk.h>
#include "interface.h"

GtkWidget *lookup_widget (GtkWidget * widget, const gchar * widget_name);
void on_about_ok (GtkWidget * widget, gpointer Data);
void on_about_dialog (GtkWidget * widget, gpointer Data);
void on_close_main_window (GtkWidget * widget, gpointer Data);

void on_open_dialog (GtkWidget * widget, gpointer Data);
void on_open_dialog_delete (GtkWidget * widget, gpointer Data);
void on_open_dialog_ok (GtkWidget * widget, gpointer Data);
void on_open_dialog_cancel (GtkWidget * widget, gpointer Data);

void on_save_dialog (GtkWidget * widget, gpointer Data);
void on_saveas_dialog (GtkWidget * widget, gpointer Data);
void on_save_dialog_delete (GtkWidget * widget, gpointer Data);
void on_save_dialog_ok (GtkWidget * widget, gpointer Data);
void on_save_dialog_cancel (GtkWidget * widget, gpointer Data);

void on_zoomin (GtkWidget * widget, gpointer Data);
void on_zoomout (GtkWidget * widget, gpointer Data);

void on_pal_dialog (GtkWidget * widget, gpointer Data);
void on_pal_dialog_ok (GtkWidget * widget, gpointer Data);

void on_setka (GtkWidget * widget, gpointer Data);
void on_nap (GtkWidget * widget, gpointer Data);
void on_def (GtkWidget * widget, gpointer Data);

void on_elem (GtkWidget * widget, gpointer Data);
void on_knot (GtkWidget * widget, gpointer Data);
void on_inter (GtkWidget * widget, gpointer Data);

void on_menu_setka (GtkWidget * widget, gpointer Data);
void on_button_setka (GtkWidget * widget, gpointer Data);
void on_menu_nap (GtkWidget * widget, gpointer Data);
void on_button_nap (GtkWidget * widget, gpointer Data);
void on_menu_def (GtkWidget * widget, gpointer Data);
void on_button_def (GtkWidget * widget, gpointer Data);
void on_menu_mtr (GtkWidget * widget, gpointer Data);
void on_button_mtr (GtkWidget * widget, gpointer Data);

void on_menu_elem (GtkWidget * widget, gpointer Data);
void on_button_elem (GtkWidget * widget, gpointer Data);

void on_menu_knot (GtkWidget * widget, gpointer Data);
void on_button_knot (GtkWidget * widget, gpointer Data);

void on_menu_inter (GtkWidget * widget, gpointer Data);
void on_button_inter (GtkWidget * widget, gpointer Data);
gint drawelem_motion_notify_event(GtkWidget *widget, GdkEventMotion *event);
#endif
