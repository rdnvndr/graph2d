/********************************************************************
* Description: Программа предназначена для графического 
               представления    результатов   численого 
	       моделирования методом конечных элементов
* Author: Rodionov Andrey
* Created at: Sun Oct 21 00:25:59 UTC 2001
* Computer: debian 
* System: Linux 2.2.17 on i586
*    
* Copyright (c) 2001 Rodionov Andrey  All rights reserved.
*
********************************************************************/


#include <gtk/gtk.h>
#include <gdk_imlib.h>
#include "interface.h"
#include "callback.h"

main (int argc, char *argv[])
{

    GtkWidget *main_window;
    gtk_init (&argc, &argv);
    gdk_imlib_init ();

    gtk_widget_push_visual (gdk_imlib_get_visual ());
    gtk_widget_push_colormap (gdk_imlib_get_colormap ());

    main_window = create_main_window ();

    gtk_main ();
    return (0);
}
