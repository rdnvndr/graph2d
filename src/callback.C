#include "callback.h"
#include "intl.h"
#include "drawelem.h"
#include <stdlib.h>
static char *dirname = NULL;

char *savename = "";
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
    about=gtk_widget_get_toplevel (GTK_WIDGET (widget));
    gtk_widget_destroy (about);
}

void on_about_dialog (GtkWidget * widget, gpointer Data)
{
    GtkWidget *about;
/*    if (gtk_object_get_data (GTK_OBJECT (widget),"about_dialog")==NULL)*/
    {
       about = create_about_dialog ();
/*       g_print(gtk_widget_get_name(widget->parent));
       g_print("\n");
       gtk_object_set_data (GTK_OBJECT(widget), "about_dialog", about);*/
       gtk_widget_show (about);
    }
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
    
    if (dirname!=NULL)
       chdir(dirname);
    
    opendialog = create_open_dialog ();

    gtk_object_set_data (GTK_OBJECT (opendialog), "drawelem", drawelem);
    gtk_object_set_data (GTK_OBJECT (opendialog), "menuitem", widget);

    gtk_widget_show (opendialog);
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
    
    if (dirname==NULL)
       delete(dirname);
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
}

void on_zoomout (GtkWidget * widget, gpointer Data)
{
    GtkWidget *drawelem;

    drawelem = lookup_widget (widget, "drawelem");
    DRAWELEM (drawelem)->mash--;
    drawelem_fit (DRAWELEM (drawelem));
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
    drawelem_loadpal (DRAWELEM (draw),filename);
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
    if (dirname!=NULL)
       chdir(dirname);
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
    if (dirname!=NULL)
       chdir(dirname);
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
    if (dirname!=NULL)
       chdir(dirname);
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

gint drawelem_motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
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
  
   if (drawelem_get()>0)
   {
      result_nds = drawelem_get_nds(DRAWELEM(draw),x,y);
      
      gcvt (double ((int) (result_nds*10000))/10000, 10, txt);
      strcat(txt1,txt);
      gtk_statusbar_push (GTK_STATUSBAR (status_bar_nds), 1,
      txt1);
   }
   else
   {
     gtk_statusbar_push (GTK_STATUSBAR (status_bar_nds), 1,
			_(" НДС: не обнаружено "));
   }
   return TRUE;
}
