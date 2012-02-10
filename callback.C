#include "callback.h"
char* savename;
GtkWidget* lookup_widget(GtkWidget *widget,const gchar *widget_name)
{
  GtkWidget *parent, *found_widget;
  
  for (;;)
    {
      if (GTK_IS_MENU (widget))
        parent = gtk_menu_get_attach_widget (GTK_MENU (widget));
      else
        parent = widget->parent;
      if (parent == NULL)
        break;
      widget = parent;
    }

  found_widget = (GtkWidget*) gtk_object_get_data (GTK_OBJECT (widget),widget_name);
  
  if (!found_widget)
    g_warning ("Widget not found: %s", widget_name);
  return found_widget;
}

void on_about_ok(GtkWidget *widget ,gpointer Data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_about_dialog(GtkWidget *widget ,gpointer Data)
{
   GtkWidget *about;
   about=create_about_dialog();
   gtk_widget_show(about);
}

void on_close_main_window(GtkWidget *widget ,gpointer Data)
{
   gtk_main_quit();
}

void on_open_dialog(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *opendialog;
  GtkWidget *drawelem;
  
  drawelem=lookup_widget(widget,"drawelem");
  
  opendialog=create_open_dialog();
  
  gtk_object_set_data(GTK_OBJECT(opendialog), "drawelem",drawelem);
  gtk_object_set_data(GTK_OBJECT(opendialog), "menuitem",widget);
  
  gtk_widget_show(opendialog);
}

void on_open_dialog_ok(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *filesel;
  GtkWidget *draw;
  GtkWidget *item;
  GtkWidget *button;
  GtkWidget *menuitem;
  static char *filename=NULL;
  static char *dirname=NULL;
  int i,j;
  
  
  filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
  gtk_widget_hide (filesel);
  
  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  for (i=strlen(filename);i>=0;i--)
    if (filename[i]==*"/")
      break;
  i++;
  
  dirname=new char[i];
  
  for (j=0;j<i;j++)
     dirname[j]=filename[j];
  dirname[i]=*"\0";
  g_print("Open dir: ");
  g_print(dirname);
  g_print("\n");
  
  draw=lookup_widget(filesel,"drawelem");
  savename=NULL;
  drawelem_load_all(DRAWELEM(draw),dirname);
  
  menuitem=lookup_widget(filesel,"menuitem");
  item=lookup_widget(menuitem,"save");
  gtk_widget_set_sensitive(item,TRUE);
  
  item=lookup_widget(menuitem,"saveas");
  gtk_widget_set_sensitive(item,TRUE);
  
  item=lookup_widget(menuitem,"pal");
  gtk_widget_set_sensitive(item,TRUE);
  
  item=lookup_widget(menuitem,"view");
  gtk_widget_set_sensitive(item,TRUE);
  
  button=lookup_widget(menuitem,"button_save");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_pal");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_in");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_out");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_s");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_d");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_n");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_elem");
  gtk_widget_set_sensitive(button,TRUE);
  
  button=lookup_widget(menuitem,"button_knot");
  gtk_widget_set_sensitive(button,TRUE);
}

void on_open_dialog_cancel(GtkWidget *widget ,gpointer Data)
{
    gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_open_dialog_delete(GtkWidget *widget ,gpointer Data)
{
   gtk_widget_hide (gtk_widget_get_toplevel (widget));
}

void on_save_dialog(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *savedialog;
  GtkWidget *drawelem;
  drawelem=lookup_widget(widget,"drawelem");
  if (savename==NULL)
  {
     savedialog=create_save_dialog();
     gtk_object_set_data(GTK_OBJECT(savedialog), "drawelem",drawelem);
     gtk_widget_show(savedialog);
  }
  else
  {
    g_print("save: ");
    g_print(savename);
    g_print("\n");
    drawelem_save(DRAWELEM(drawelem),savename);
  }
}

void on_saveas_dialog(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *savedialog;
  GtkWidget *drawelem;
  
  drawelem=lookup_widget(widget,"drawelem");
  
  savedialog=create_save_dialog();
  
  gtk_object_set_data(GTK_OBJECT(savedialog), "drawelem",drawelem);
  
  gtk_widget_show(savedialog);
}

void on_save_dialog_ok(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *filesel;
  GtkWidget *draw;
  int i,j;
  
  
  filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
  gtk_widget_hide (filesel);
  
  savename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  
  if (savename[strlen(savename)-1]==*"/")
  {
     g_print("Файл не найден.\n");
     savename=NULL;
     return;
  }
  
  g_print("save: ");
  g_print(savename);
  g_print("\n");
  
  draw=lookup_widget(filesel,"drawelem");
  drawelem_save(DRAWELEM(draw),savename);
 
}


void on_save_dialog_cancel(GtkWidget *widget ,gpointer Data)
{
    gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
}

void on_save_dialog_delete(GtkWidget *widget ,gpointer Data)
{
   gtk_widget_hide (gtk_widget_get_toplevel (widget));
}

void on_zoomin(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *drawelem;
  
  drawelem=lookup_widget(widget,"drawelem");
  DRAWELEM(drawelem)->mash++;
  drawelem_fit(DRAWELEM(drawelem));
}

void on_zoomout(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *drawelem;
  
  drawelem=lookup_widget(widget,"drawelem");
  DRAWELEM(drawelem)->mash--;
  drawelem_fit(DRAWELEM(drawelem));
}

void on_pal_dialog(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *opendialog;
  GtkWidget *drawelem;
  
  drawelem=lookup_widget(widget,"drawelem");
  opendialog=create_pal_dialog();
  gtk_object_set_data(GTK_OBJECT(opendialog), "drawelem",drawelem);
  gtk_widget_show(opendialog);
}

void on_pal_dialog_ok(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *filesel;
  GtkWidget *draw;
  static char *filename=NULL;
  int i,j;
  
  
  filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
  gtk_widget_hide (filesel);
  
  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  
  g_print("Open pal: ");
  g_print(filename);
  g_print("\n");
  
  drawelem_loadpal(filename);
  draw=lookup_widget(filesel,"drawelem");
  drawelem_fit(DRAWELEM(draw));
}

void on_setka(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *draw;
  GtkWidget *status_bar;
  
  status_bar=lookup_widget(widget,"statusbar");
  gtk_statusbar_push(GTK_STATUSBAR(status_bar),1,"Просмотр элементов");
  
  draw=lookup_widget(widget,"drawelem");
  drawelem_set(0);
  drawelem_fit(DRAWELEM(draw));
  
}

void on_nap(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *draw;
  GtkWidget *status_bar;
  
  status_bar=lookup_widget(widget,"statusbar");
  gtk_statusbar_push(GTK_STATUSBAR(status_bar),1,"Просмотр напряжений");
  
  draw=lookup_widget(widget,"drawelem");
  drawelem_load_nds(DRAWELEM(draw),"sg_na.poc");
  drawelem_set(1);
  drawelem_fit(DRAWELEM(draw));
  
}

void on_def(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *draw;
  GtkWidget *status_bar;
  
  status_bar=lookup_widget(widget,"statusbar");
  gtk_statusbar_push(GTK_STATUSBAR(status_bar),1,"Просмотр деформаций");
  
  draw=lookup_widget(widget,"drawelem");
  drawelem_load_nds(DRAWELEM(draw),"na_df.poc");
  drawelem_set(2);
  drawelem_fit(DRAWELEM(draw));
  
}

void on_elem(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *draw;
  
  draw=lookup_widget(widget,"drawelem");
  drawelem_show_nelem();
  drawelem_fit(DRAWELEM(draw));
}

void on_knot(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *draw;
  
  draw=lookup_widget(widget,"drawelem");
  drawelem_show_nknot();
  drawelem_fit(DRAWELEM(draw));
}

void on_menu_setka(GtkWidget *widget ,gpointer Data)
{
   GtkWidget *button;
   if (GTK_CHECK_MENU_ITEM(widget)->active==TRUE)
   {
      button=lookup_widget(widget,"button_s");
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==FALSE)
      {
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),TRUE);
	 return;
      }
      on_setka(widget,Data);
   }
}
void on_button_setka(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *item;
  
  if (GTK_TOGGLE_BUTTON(widget)->active==TRUE)
  {
     item=lookup_widget(widget,"menu_s");
     if (GTK_CHECK_MENU_ITEM(item)->active==FALSE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_setka(widget,Data);
  }
}
void on_menu_nap(GtkWidget *widget ,gpointer Data)
{
   GtkWidget *button;
   if (GTK_CHECK_MENU_ITEM(widget)->active==TRUE)
   {
      button=lookup_widget(widget,"button_n");
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==FALSE)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),TRUE); 
	 return;
      }
      on_nap(widget,Data);
   }
}
void on_button_nap(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *item;
  
  if (GTK_TOGGLE_BUTTON(widget)->active==TRUE)
  {
     item=lookup_widget(widget,"menu_n");
     if (GTK_CHECK_MENU_ITEM(item)->active==FALSE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_nap(widget,Data);
  }
}
void on_menu_def(GtkWidget *widget ,gpointer Data)
{
   GtkWidget *button;
   if (GTK_CHECK_MENU_ITEM(widget)->active==TRUE)
   {
      button=lookup_widget(widget,"button_d");
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==FALSE)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),TRUE);
	 return;
      }
      on_def(widget,Data);
   }
}
void on_button_def(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *item;
  
  if (GTK_TOGGLE_BUTTON(widget)->active==TRUE)
  {
     item=lookup_widget(widget,"menu_d");
     if (GTK_CHECK_MENU_ITEM(item)->active==FALSE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_def(widget,Data);
  }
     
}

void on_menu_elem(GtkWidget *widget ,gpointer Data)
{
   GtkWidget *button;
   button=lookup_widget(widget,"button_elem");
   if (GTK_CHECK_MENU_ITEM(widget)->active==TRUE)
   {
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==FALSE)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),TRUE);
	 return;
      }
      on_elem(widget,Data);
   }
   else
   {
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==TRUE)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),FALSE);
	 return;
      }
      on_elem(widget,Data);
   }
}
void on_button_elem(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *item;
  item=lookup_widget(widget,"menu_elem");
  if (GTK_TOGGLE_BUTTON(widget)->active==TRUE)
  {
     if (GTK_CHECK_MENU_ITEM(item)->active==FALSE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_elem(widget,Data);
  } 
  else
  {
     if (GTK_CHECK_MENU_ITEM(item)->active==TRUE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_elem(widget,Data);
  } 
}

void on_menu_knot(GtkWidget *widget ,gpointer Data)
{
  
   GtkWidget *button;
   button=lookup_widget(widget,"button_knot");
   if (GTK_CHECK_MENU_ITEM(widget)->active==TRUE)
   {
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==FALSE)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),TRUE);
	 return;
      }
      on_knot(widget,Data);
   }
   else
   {
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button))==TRUE)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),FALSE);
	 return;
      }
      on_knot(widget,Data);
   }
}

void on_button_knot(GtkWidget *widget ,gpointer Data)
{
  GtkWidget *item;
  item=lookup_widget(widget,"menu_knot");
  if (GTK_TOGGLE_BUTTON(widget)->active==TRUE)
  {
     if (GTK_CHECK_MENU_ITEM(item)->active==FALSE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_knot(widget,Data);
  } 
  else
  {
     if (GTK_CHECK_MENU_ITEM(item)->active==TRUE)
     {
        gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return;
     }
     on_knot(widget,Data);
  } 
}