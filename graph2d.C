#include <gtk/gtk.h>
#include <gdk_imlib.h>
#include "interface.h"
#include "callback.h"

main(int argc,char *argv[])
{
   
   GtkWidget *main_window;
   gtk_init(&argc,&argv);
   gdk_imlib_init();

   gtk_widget_push_visual(gdk_imlib_get_visual());
   gtk_widget_push_colormap(gdk_imlib_get_colormap());
   
   main_window = create_main_window();
   
   gtk_main();
   return(0);
}