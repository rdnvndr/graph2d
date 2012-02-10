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

void on_options_dialog (GtkWidget * widget, gpointer Data);
void on_options_dialog_ok (GtkWidget * widget, gpointer Data);
void on_options_dialog_apply (GtkWidget * widget, gpointer Data);
void on_options_dialog_cancel (GtkWidget * widget, gpointer Data);

void on_button_elem_font (GtkWidget * widget, gpointer Data);
void on_button_elem_color (GtkWidget * widget, gpointer Data);
void on_button_knot_font (GtkWidget * widget, gpointer Data);
void on_button_knot_color (GtkWidget * widget, gpointer Data);

void on_color_dialog_ok (GtkWidget * widget, gpointer Data);
void on_color_dialog_cancel (GtkWidget * widget, gpointer Data);
void on_color_dialog_help (GtkWidget * widget, gpointer Data);

void on_font_dialog_ok (GtkWidget * widget, gpointer Data);
void on_font_dialog_cancel (GtkWidget * widget, gpointer Data);
void on_font_dialog_apply (GtkWidget * widget, gpointer Data);

gint on_opt_configure_event_elem(GtkWidget * widget, GdkEventConfigure * event);
gint on_opt_expose_event_elem(GtkWidget * widget, GdkEventExpose * event);
gint on_opt_configure_event_knot (GtkWidget * widget, GdkEventConfigure * event);
gint on_opt_expose_event_knot (GtkWidget * widget, GdkEventExpose * event);

#endif
