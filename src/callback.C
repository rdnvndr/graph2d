#include "callback.h"
#include "intl.h"
#include "drawelem.h"
#include <stdlib.h>
static char *dirname = NULL;
GdkPixmap *pixmap_knot = NULL;
GdkPixmap *pixmap_elem = NULL;
static GdkGC     *color_knot = NULL;
static GdkGC     *color_elem = NULL;
char *savename = "";

GdkGC *color_to_gc (GtkWidget * widget, gdouble* fcolor)
{
    GdkColor color;
    GdkGC *colorgc;
    GdkGCValues gc_values;
    color.red = gushort(fcolor[0] * 256 * 255);
    color.green = gushort(fcolor[1] * 256 * 255);
    color.blue =gushort( fcolor[2] * 256 * 255);
    if (!gdk_color_alloc (gdk_colormap_get_system (), &color))
	g_print ("Ошибка: Цвет не найден.\n ");
    gc_values.foreground = color;
    colorgc = gdk_gc_new_with_values (GTK_WIDGET (widget)->window,
				      &gc_values, GDK_GC_FOREGROUND);
    return (colorgc);
};

GtkWidget *lookup_widget (GtkWidget * widget, const gchar * widget_name)
{
    GtkWidget *parent, *found_widget;

    for (;;) {
	if (GTK_IS_MENU (widget))
	    parent = gtk_menu_get_attach_widget (GTK_MENU (widget));
	else
	    parent = widget->parent;
	if (parent == NULL)
	    break;
	widget = parent;
    }

    found_widget =
	(GtkWidget *) gtk_object_get_data (GTK_OBJECT (widget),
					   widget_name);

    if (!found_widget)
	g_warning ("Widget not found: %s", widget_name);
    return found_widget;
}

void on_about_ok (GtkWidget * widget, gpointer Data)
{
    GtkWidget *about;
    about = gtk_widget_get_toplevel (GTK_WIDGET (widget));
    gtk_widget_destroy (about);
}

void on_about_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *about;
    about = create_about_dialog ();
    gtk_widget_show (about);
}

void on_close_main_window (GtkWidget * widget, gpointer Data)
{
    gtk_main_quit ();
}

void on_open_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *opendialog;
    GtkWidget *drawelem;

    drawelem = lookup_widget (widget, "drawelem");

    if (dirname != NULL)
	chdir (dirname);

    opendialog = create_open_dialog ();
    gtk_object_set_data (GTK_OBJECT (opendialog), "drawelem", drawelem);
    gtk_object_set_data (GTK_OBJECT (opendialog), "menuitem", widget);

    gtk_widget_show (opendialog);
}

void refresh_rulers (GtkWidget * draw)
{
    GtkWidget *ver_ruler;
    GtkWidget *hor_ruler;
    GtkWidget *scrolledwindow;
    gint height, width;
    float max;

    ver_ruler = lookup_widget (draw, "ver_ruler");
    hor_ruler = lookup_widget (draw, "hor_ruler");
    scrolledwindow = lookup_widget (draw, "scrolledwindow");

    gdk_window_get_size (DRAWELEM (draw)->pixmap, &width, &height);

    gtk_widget_set_usize (ver_ruler,-1, height + 5);
    gtk_widget_set_usize (hor_ruler, width + 5, -1);

    max = (scrolledwindow->allocation.height > height) ?
	scrolledwindow->allocation.height : height;
    float delta_y =
	(scrolledwindow->allocation.height >
	 height) ? ((scrolledwindow->allocation.height - height) / 2) : 0;
    gtk_ruler_set_range (GTK_RULER (ver_ruler),
			 (DRAWELEM (draw)->zero_y +
			  delta_y) / DRAWELEM (draw)->mash,
			 (-max + DRAWELEM (draw)->zero_y +
			  delta_y) / DRAWELEM (draw)->mash, 10,
			 (-max + DRAWELEM (draw)->zero_y +
			  delta_y) / DRAWELEM (draw)->mash);

    max = (scrolledwindow->allocation.width > width) ?
	scrolledwindow->allocation.width : width;
    float delta_x =
	(scrolledwindow->allocation.width >
	 width) ? ((scrolledwindow->allocation.width - width) / 2 +
		   20) : 20;
    gtk_ruler_set_range (GTK_RULER (hor_ruler),
			 ((DRAWELEM (draw)->zero_x - delta_x) /
			  DRAWELEM (draw)->mash),
			 ((max + (DRAWELEM (draw)->zero_x - delta_x)) /
			  DRAWELEM (draw)->mash), 0,
			 (max +
			  (DRAWELEM (draw)->zero_x -
			   delta_x)) / DRAWELEM (draw)->mash);

    /*
       g_print("%d %s",scrolledwindow->allocation.width," width: ");
       g_print("%d %s  %f",width," ",max);
     */


}

void on_open_dialog_ok (GtkWidget * widget, gpointer Data)
{
    GtkWidget *filesel;
    GtkWidget *draw;
    GtkWidget *item;
    GtkWidget *button;
    GtkWidget *menuitem;

    static char *filename = NULL;

    int i, j;

    filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
    gtk_widget_hide (filesel);

    filename =
	gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
    for (i = strlen (filename); i >= 0; i--)
	if (filename[i] == *"/")
	    break;
    i++;

    if (dirname == NULL)
	delete (dirname);
    dirname = new char[i];

    for (j = 0; j < i; j++)
	dirname[j] = filename[j];
    dirname[i] = *"\0";
    g_print ("Open dir: ");
    g_print (dirname);
    g_print ("\n");

    draw = lookup_widget (filesel, "drawelem");

    savename = NULL;
    drawelem_load_all (DRAWELEM (draw), dirname);
    refresh_rulers (draw);

    menuitem = lookup_widget (filesel, "menuitem");
    item = lookup_widget (menuitem, "save");
    gtk_widget_set_sensitive (item, TRUE);

    item = lookup_widget (menuitem, "saveas");
    gtk_widget_set_sensitive (item, TRUE);

    item = lookup_widget (menuitem, "pal");
    gtk_widget_set_sensitive (item, TRUE);

    item = lookup_widget (menuitem, "view");
    gtk_widget_set_sensitive (item, TRUE);

    button = lookup_widget (menuitem, "button_save");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_pal");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_in");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_out");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_s");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_d");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_n");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_m");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_elem");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_knot");
    gtk_widget_set_sensitive (button, TRUE);

    button = lookup_widget (menuitem, "button_inter");
    gtk_widget_set_sensitive (button, TRUE);
}

void on_open_dialog_cancel (GtkWidget * widget, gpointer Data)
{
    gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_open_dialog_delete (GtkWidget * widget, gpointer Data)
{
    gtk_widget_hide (gtk_widget_get_toplevel (widget));
}

void on_save_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *savedialog;
    GtkWidget *drawelem;
    drawelem = lookup_widget (widget, "drawelem");
    if (savename == NULL) {
	savedialog = create_save_dialog ();
	gtk_object_set_data (GTK_OBJECT (savedialog), "drawelem",
			     drawelem);
	gtk_widget_show (savedialog);
    } else {
	g_print ("save: ");
	g_print (savename);
	g_print ("\n");
	drawelem_save (DRAWELEM (drawelem), savename);
    }
}

void on_saveas_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *savedialog;
    GtkWidget *drawelem;

    drawelem = lookup_widget (widget, "drawelem");

    savedialog = create_save_dialog ();

    gtk_object_set_data (GTK_OBJECT (savedialog), "drawelem", drawelem);

    gtk_widget_show (savedialog);
}

void on_save_dialog_ok (GtkWidget * widget, gpointer Data)
{
    GtkWidget *filesel;
    GtkWidget *draw;
    int i, j;


    filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
    gtk_widget_hide (filesel);

    savename =
	gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));

    if (savename[strlen (savename) - 1] == *"/") {
	g_print ("Файл не найден.\n");
	savename = NULL;
	return;
    }

    g_print ("save: ");
    g_print (savename);
    g_print ("\n");

    draw = lookup_widget (filesel, "drawelem");
    drawelem_save (DRAWELEM (draw), savename);

}


void on_save_dialog_cancel (GtkWidget * widget, gpointer Data)
{
    gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_save_dialog_delete (GtkWidget * widget, gpointer Data)
{
    gtk_widget_hide (gtk_widget_get_toplevel (widget));
}

void on_zoomin (GtkWidget * widget, gpointer Data)
{
    GtkWidget *drawelem;

    drawelem = lookup_widget (widget, "drawelem");
    DRAWELEM (drawelem)->mash++;
    drawelem_fit (DRAWELEM (drawelem));
    refresh_rulers (drawelem);
}

void on_zoomout (GtkWidget * widget, gpointer Data)
{
    GtkWidget *drawelem;

    drawelem = lookup_widget (widget, "drawelem");
    DRAWELEM (drawelem)->mash--;
    drawelem_fit (DRAWELEM (drawelem));
    refresh_rulers (drawelem);
}

void on_pal_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *opendialog;
    GtkWidget *drawelem;

    drawelem = lookup_widget (widget, "drawelem");
    opendialog = create_pal_dialog ();
    gtk_object_set_data (GTK_OBJECT (opendialog), "drawelem", drawelem);
    gtk_widget_show (opendialog);
}


void on_pal_dialog_ok (GtkWidget * widget, gpointer Data)
{
    GtkWidget *filesel;
    GtkWidget *draw;
    static char *filename = NULL;
    int i, j;

    filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
    gtk_widget_hide (filesel);

    filename =
	gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));

    g_print ("Open pal: ");
    g_print (filename);
    g_print ("\n");


    draw = lookup_widget (filesel, "drawelem");
    drawelem_loadpal (DRAWELEM (draw), filename);
    drawelem_fit (DRAWELEM (draw));
}

void on_setka (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;
    GtkWidget *status_bar;

    status_bar = lookup_widget (widget, "statusbar");
    gtk_statusbar_push (GTK_STATUSBAR (status_bar), 1,
			_("Просмотр элементов"));

    draw = lookup_widget (widget, "drawelem");
    drawelem_set (0);
    drawelem_fit (DRAWELEM (draw));

}

void on_nap (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;
    GtkWidget *status_bar;

    status_bar = lookup_widget (widget, "statusbar");
    gtk_statusbar_push (GTK_STATUSBAR (status_bar), 1,
			_("Просмотр напряжений"));

    draw = lookup_widget (widget, "drawelem");
    drawelem_set (1);
    if (dirname != NULL)
	chdir (dirname);
    drawelem_load_nds (DRAWELEM (draw), "sg_na.poc");
    drawelem_fit (DRAWELEM (draw));

}

void on_def (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;
    GtkWidget *status_bar;

    status_bar = lookup_widget (widget, "statusbar");
    gtk_statusbar_push (GTK_STATUSBAR (status_bar), 1,
			_("Просмотр деформаций"));

    draw = lookup_widget (widget, "drawelem");
    drawelem_set (2);
    if (dirname != NULL)
	chdir (dirname);
    drawelem_load_nds (DRAWELEM (draw), "na_df.poc");
    drawelem_fit (DRAWELEM (draw));

}

void on_mtr (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;
    GtkWidget *status_bar;

    status_bar = lookup_widget (widget, "statusbar");
    gtk_statusbar_push (GTK_STATUSBAR (status_bar), 1,
			_("Просмотр микротрещин"));

    draw = lookup_widget (widget, "drawelem");
    drawelem_set (3);
    if (dirname != NULL)
	chdir (dirname);
    drawelem_load_nds (DRAWELEM (draw), "ntg.poc");
    drawelem_fit (DRAWELEM (draw));

}

void on_elem (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;

    draw = lookup_widget (widget, "drawelem");
    drawelem_show_nelem ();
    drawelem_fit (DRAWELEM (draw));
}

void on_knot (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;

    draw = lookup_widget (widget, "drawelem");
    drawelem_show_nknot ();
    drawelem_fit (DRAWELEM (draw));
}

void on_inter (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;

    draw = lookup_widget (widget, "drawelem");
    drawelem_inter ();
    drawelem_fit (DRAWELEM (draw));
}


void on_menu_setka (GtkWidget * widget, gpointer Data)
{
    GtkWidget *button;
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	button = lookup_widget (widget, "button_s");
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_setka (widget, Data);
    }
}
void on_button_setka (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;

    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	item = lookup_widget (widget, "menu_s");
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_setka (widget, Data);
    }
}
void on_menu_nap (GtkWidget * widget, gpointer Data)
{
    GtkWidget *button;
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	button = lookup_widget (widget, "button_n");
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_nap (widget, Data);
    }
}
void on_button_nap (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;

    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	item = lookup_widget (widget, "menu_n");
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_nap (widget, Data);
    }
}
void on_menu_def (GtkWidget * widget, gpointer Data)
{
    GtkWidget *button;
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	button = lookup_widget (widget, "button_d");
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_def (widget, Data);
    }
}
void on_button_def (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;

    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	item = lookup_widget (widget, "menu_d");
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_def (widget, Data);
    }

}

void on_menu_mtr (GtkWidget * widget, gpointer Data)
{
    GtkWidget *button;
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	button = lookup_widget (widget, "button_m");
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_mtr (widget, Data);
    }
}
void on_button_mtr (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;

    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	item = lookup_widget (widget, "menu_m");
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_mtr (widget, Data);
    }

}

void on_menu_elem (GtkWidget * widget, gpointer Data)
{
    GtkWidget *button;
    button = lookup_widget (widget, "button_elem");
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_elem (widget, Data);
    } else {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    TRUE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  FALSE);
	    return;
	}
	on_elem (widget, Data);
    }
}
void on_button_elem (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;
    item = lookup_widget (widget, "menu_elem");
    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_elem (widget, Data);
    } else {
	if (GTK_CHECK_MENU_ITEM (item)->active == TRUE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_elem (widget, Data);
    }
}

void on_menu_knot (GtkWidget * widget, gpointer Data)
{

    GtkWidget *button;
    button = lookup_widget (widget, "button_knot");
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_knot (widget, Data);
    } else {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    TRUE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  FALSE);
	    return;
	}
	on_knot (widget, Data);
    }
}

void on_button_knot (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;
    item = lookup_widget (widget, "menu_knot");
    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_knot (widget, Data);
    } else {
	if (GTK_CHECK_MENU_ITEM (item)->active == TRUE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_knot (widget, Data);
    }
}

void on_menu_inter (GtkWidget * widget, gpointer Data)
{

    GtkWidget *button;
    button = lookup_widget (widget, "button_inter");
    if (GTK_CHECK_MENU_ITEM (widget)->active == TRUE) {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    FALSE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  TRUE);
	    return;
	}
	on_inter (widget, Data);
    } else {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)) ==
	    TRUE) {
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button),
					  FALSE);
	    return;
	}
	on_inter (widget, Data);
    }
}

void on_button_inter (GtkWidget * widget, gpointer Data)
{
    GtkWidget *item;
    item = lookup_widget (widget, "menu_inter");
    if (GTK_TOGGLE_BUTTON (widget)->active == TRUE) {
	if (GTK_CHECK_MENU_ITEM (item)->active == FALSE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_inter (widget, Data);
    } else {
	if (GTK_CHECK_MENU_ITEM (item)->active == TRUE) {
	    gtk_menu_item_activate (GTK_MENU_ITEM (item));
	    return;
	}
	on_inter (widget, Data);
    }
}

gint drawelem_motion_notify_event (GtkWidget * widget,
				   GdkEventMotion * event)
{
    int x, y;
    GdkModifierType state;
    float result_nds;
    char txt[10] = "";
    char txt1[17] = " НДС: ";

    GtkWidget *draw;
    GtkWidget *status_bar_nds;

    if (event->is_hint)
	gdk_window_get_pointer (event->window, &x, &y, &state);



    status_bar_nds = lookup_widget (widget, "statusbar_nds");
    draw = lookup_widget (widget, "drawelem");

    if (drawelem_get () > 0) {
	result_nds = drawelem_get_nds (DRAWELEM (draw), x, y);

	gcvt (double ((int) (result_nds * 10000)) / 10000, 10, txt);
	strcat (txt1, txt);
	gtk_statusbar_push (GTK_STATUSBAR (status_bar_nds), 1, txt1);
    } else {
	gtk_statusbar_push (GTK_STATUSBAR (status_bar_nds), 1,
			    _(" НДС: не обнаружено "));
    }
    return TRUE;
}

void on_options_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *options;
    GtkWidget *drawelem;
    GtkWidget *toolbar;
    
    GtkWidget *draw_elem_color;
    GtkWidget *draw_knot_color;
    GtkWidget *entry_elem;
    GtkWidget *entry_knot;
    
    GtkWidget *radiobutton_it;
    GtkWidget *radiobutton_i;
    GtkWidget *radiobutton_t;
    
    GtkWidget *check_show_toolbar;
    GtkWidget *check_show_tooltip;
    GtkWidget *check_raised_button;
    GtkWidget *toolbar_handlebox;
    
    options = create_options_dialog ();
    drawelem = lookup_widget (widget, "drawelem");
    toolbar = lookup_widget (widget, "toolbar");
    
    draw_elem_color = lookup_widget (options, "opt_drawingarea_elem_color");
    draw_knot_color = lookup_widget (options, "opt_drawingarea_knot_color");
    entry_elem = lookup_widget (options, "opt_entry_elem_font");
    entry_knot = lookup_widget (options, "opt_entry_knot_font");
    radiobutton_it = lookup_widget (options,"opt_radiobutton_toolbar_it");
    radiobutton_i = lookup_widget (options,"opt_radiobutton_toolbar_i");
    radiobutton_t = lookup_widget (options,"opt_radiobutton_toolbar_t");
    check_show_toolbar = lookup_widget (options,"opt_check_show_toolbar");
    check_show_tooltip = lookup_widget (options,"opt_check_show_tooltip");
    check_raised_button = lookup_widget (options,"opt_check_raised_button");
    toolbar_handlebox = lookup_widget (widget,"toolbar_handlebox");
    
    gtk_object_set_data (GTK_OBJECT (options), "drawelem", drawelem);
    gtk_object_set_data (GTK_OBJECT (options), "toolbar", toolbar);
    gtk_object_set_data (GTK_OBJECT (options),"toolbar_handlebox",toolbar_handlebox);
    
    gtk_entry_set_text(GTK_ENTRY(entry_elem),drawelem_get_font_elem (DRAWELEM(drawelem)));
    gtk_entry_set_text(GTK_ENTRY(entry_knot),drawelem_get_font_knot (DRAWELEM(drawelem)));
    
    color_elem = gdk_gc_ref(drawelem_get_color_elem (DRAWELEM(drawelem)));
    color_knot = gdk_gc_ref(drawelem_get_color_knot (DRAWELEM(drawelem)));
    
    if (GTK_TOOLBAR(toolbar)->style==GTK_TOOLBAR_BOTH)
       gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_it),TRUE);
    else
    if (GTK_TOOLBAR(toolbar)->style==GTK_TOOLBAR_TEXT)
       gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_t),TRUE);
    else
    if (GTK_TOOLBAR(toolbar)->style==GTK_TOOLBAR_ICONS)
       gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_i),TRUE);
    
    if (gtk_toolbar_get_button_relief(GTK_TOOLBAR(toolbar)) == GTK_RELIEF_NONE)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_raised_button),TRUE);
	
    if (GTK_TOOLBAR(toolbar)->tooltips->enabled == 1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_show_tooltip),TRUE);
    
    if (GTK_WIDGET_VISIBLE(toolbar_handlebox)==TRUE)
       gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_show_toolbar),TRUE);
    
    gtk_widget_show (options);
    
}


void on_options_dialog_apply (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw_elem_color;
    GtkWidget *draw_knot_color;
    GtkWidget *entry_elem;
    GtkWidget *entry_knot;
    GtkWidget *drawelem;
    GtkWidget *toolbar;
    char *text;
    
    GtkWidget *radiobutton_it;
    GtkWidget *radiobutton_i;
    GtkWidget *radiobutton_t;
    GtkWidget *check_show_toolbar;
    GtkWidget *check_show_tooltip;
    GtkWidget *check_raised_button;
    GtkWidget *toolbar_handlebox;
    
    draw_elem_color = lookup_widget (widget, "opt_drawingarea_elem_color");
    draw_knot_color = lookup_widget (widget, "opt_drawingarea_knot_color");
    entry_elem = lookup_widget (widget, "opt_entry_elem_font");
    entry_knot = lookup_widget (widget, "opt_entry_knot_font");
    toolbar = lookup_widget (widget, "toolbar");
    radiobutton_it = lookup_widget (widget,"opt_radiobutton_toolbar_it");
    radiobutton_i = lookup_widget (widget,"opt_radiobutton_toolbar_i");
    radiobutton_t = lookup_widget (widget,"opt_radiobutton_toolbar_t");
    check_show_toolbar = lookup_widget (widget,"opt_check_show_toolbar");
    check_show_tooltip = lookup_widget (widget,"opt_check_show_tooltip");
    check_raised_button = lookup_widget (widget,"opt_check_raised_button");
    toolbar_handlebox = lookup_widget (widget,"toolbar_handlebox");
    
    drawelem = lookup_widget (widget, "drawelem");
    
    text = gtk_entry_get_text(GTK_ENTRY(entry_knot));
    
    if (text!=NULL)
        drawelem_set_font_knot (DRAWELEM(drawelem),text);
    
    text = gtk_entry_get_text(GTK_ENTRY(entry_elem));
    
    if (text!=NULL)
       drawelem_set_font_elem (DRAWELEM(drawelem),text);
    
    drawelem_set_color_elem (DRAWELEM(drawelem),color_elem);
    drawelem_set_color_knot (DRAWELEM(drawelem),color_knot);
    
    drawelem_fit (DRAWELEM (drawelem));
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_it))==TRUE)
       gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_BOTH);
    else
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_t))==TRUE)
       gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_TEXT);
    else
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_i))==TRUE)
       gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);
       
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_show_toolbar))==TRUE)
        gtk_widget_show(toolbar_handlebox);
    else
        gtk_widget_hide(toolbar_handlebox);
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_show_tooltip))==TRUE)
        gtk_toolbar_set_tooltips (GTK_TOOLBAR(toolbar),TRUE);
    else
        gtk_toolbar_set_tooltips (GTK_TOOLBAR(toolbar),FALSE);
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_raised_button))==TRUE)
        gtk_toolbar_set_button_relief (GTK_TOOLBAR(toolbar),GTK_RELIEF_NONE);
    else
        gtk_toolbar_set_button_relief (GTK_TOOLBAR(toolbar),GTK_RELIEF_NORMAL);
    
    return;
}

void on_options_dialog_ok (GtkWidget * widget, gpointer Data)
{
    on_options_dialog_apply(widget,Data);
    gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
    return ;
}

void on_options_dialog_cancel (GtkWidget * widget, gpointer Data)
{
   gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_button_elem_font (GtkWidget * widget, gpointer Data)
{
    GtkWidget *font;
    GtkWidget *entry;
    
    font = create_font_dialog();
    entry = lookup_widget (widget, "opt_entry_elem_font");
    gtk_object_set_data (GTK_OBJECT(font), "opt_entry",entry);
    gtk_widget_show (font);
}

void on_button_elem_color (GtkWidget * widget, gpointer Data)
{
    GtkWidget *color;
    GtkWidget *draw;
    
    color = create_color_dialog();
    draw = lookup_widget (widget, "opt_drawingarea_elem_color");
    gtk_object_set_data (GTK_OBJECT(color), "opt_draw",draw);
    gtk_widget_show (color);
}

void on_button_knot_font (GtkWidget * widget, gpointer Data)
{
    GtkWidget *font;
    GtkWidget *entry;
    
    font = create_font_dialog();
    entry = lookup_widget (widget, "opt_entry_knot_font");
    gtk_object_set_data (GTK_OBJECT(font), "opt_entry",entry);
    gtk_widget_show (font);
}

void on_button_knot_color (GtkWidget * widget, gpointer Data)
{
    GtkWidget *color;
    GtkWidget *draw;
    
    color = create_color_dialog();
    draw = lookup_widget (widget, "opt_drawingarea_knot_color");
    gtk_object_set_data (GTK_OBJECT(color), "opt_draw",draw);
    gtk_widget_show (color);
}

void on_color_dialog_ok (GtkWidget * widget, gpointer Data)
{
    GtkWidget *draw;
    GtkWidget *colorseldialog;
    GtkColorSelection *colorsel;
    gdouble color[4];
    
    colorseldialog = gtk_widget_get_toplevel (GTK_WIDGET (widget));
    colorsel=GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(colorseldialog)->colorsel);

    gtk_color_selection_get_color(colorsel,color);
    gtk_color_selection_set_color(colorsel,color);
    draw = lookup_widget (widget, "opt_draw");
    
    if (strcmp(gtk_widget_get_name (draw),"opt_drawingarea_elem_color")==0)
    {
       if (color_elem!=NULL)
         gdk_gc_unref(color_elem);
       color_elem = color_to_gc(draw,color);
       gdk_draw_rectangle(pixmap_elem,
                            color_elem,
			    TRUE,
			    0, 0,
			    draw->allocation.width,
     		    draw->allocation.height);
     }
     else
     {
         if (color_knot!=NULL)
             gdk_gc_unref(color_knot);
         color_knot = color_to_gc(draw,color);
         gdk_draw_rectangle(pixmap_knot,
                            color_knot,
			    TRUE,
			    0, 0,
			    draw->allocation.width,
			    draw->allocation.height);
     }
     gtk_widget_hide(draw);
     gtk_widget_show(draw);
     gtk_widget_hide (colorseldialog);
    
}

void on_color_dialog_cancel (GtkWidget * widget, gpointer Data)
{
   gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_color_dialog_help (GtkWidget * widget, gpointer Data)
{

}

void on_font_dialog_apply (GtkWidget * widget, gpointer Data)
{
    GtkWidget *entry;
    GtkWidget *fontsel;
    gchar *fontname;
    
    fontsel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
    fontname = gtk_font_selection_dialog_get_font_name	(GTK_FONT_SELECTION_DIALOG(fontsel));
    
    entry = lookup_widget (widget, "opt_entry");
    if (fontname!=NULL)
      gtk_entry_set_text(GTK_ENTRY(entry),fontname);
}


void on_font_dialog_ok (GtkWidget * widget, gpointer Data)
{
    on_font_dialog_apply(widget,Data);
    gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_font_dialog_cancel (GtkWidget * widget, gpointer Data)
{
   gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

gint on_opt_configure_event_elem(GtkWidget * widget, GdkEventConfigure * event)
{
     if (!pixmap_elem) {
	if (pixmap_elem!= NULL)
	    gdk_pixmap_unref (pixmap_elem);
	pixmap_elem = gdk_pixmap_new (widget->window,
						    widget->
						    allocation.width,
						    widget->
						    allocation.height, -1);

	gdk_draw_rectangle (pixmap_elem,
			    color_elem,
			    TRUE,
			    0, 0,
			    widget->allocation.width,
			    widget->allocation.height);
    }	
   
    return TRUE;
}

gint on_opt_expose_event_elem(GtkWidget * widget, GdkEventExpose * event)
{
        gdk_draw_pixmap (widget->window,
		     widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		     pixmap_elem,
		     event->area.x, event->area.y,
		     event->area.x, event->area.y,
		     widget->allocation.width, widget->allocation.height);
    return FALSE;
    
}

gint on_opt_configure_event_knot(GtkWidget * widget, GdkEventConfigure * event)
{
     if (!pixmap_knot) {
	if (pixmap_knot!= NULL)
	    gdk_pixmap_unref (pixmap_knot);
	pixmap_knot = gdk_pixmap_new (widget->window,
						    widget->
						    allocation.width,
						    widget->
						    allocation.height, -1);

	gdk_draw_rectangle (pixmap_knot,
			    color_knot,
			    TRUE,
			    0, 0,
			    widget->allocation.width,
			    widget->allocation.height);
    }	
    return TRUE;
}

gint on_opt_expose_event_knot(GtkWidget * widget, GdkEventExpose * event)
{
   gdk_draw_pixmap (widget->window,
		     widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		     pixmap_knot,
		     event->area.x, event->area.y,
		     event->area.x, event->area.y,
		     widget->allocation.width, widget->allocation.height);

    return FALSE;
    
}

void on_save_options (GtkWidget * widget, gpointer Data)
{

}

void on_load_options (GtkWidget * widget, gpointer Data)
{

}
