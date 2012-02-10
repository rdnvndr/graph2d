#include "interface.h"
#include "pixmaps.h"
#include "intl.h"
#include <stdlib.h>
GtkWidget *create_main_window (void)
{
    GtkWidget *main_window;
    GtkWidget *main_vbox;
    GtkWidget *main_statusbar;

    GtkWidget *menubar;
    GtkWidget *item_menu;
    GtkWidget *main_menu;
    GtkAccelGroup *main_menu_accels;
    GtkWidget *item;
    GSList *menugroup = NULL;

    GtkWidget *menu_handlebox;
    GtkWidget *scrolledwindow;
    GtkWidget *viewport;
    GtkWidget *table;
    GtkWidget *drawelem;
    GtkAccelGroup *accel_group;

    GtkWidget *toolbar_handlebox;
    GtkWidget *toolbar;
    GtkWidget *tmp_toolbar_icon;
    GdkBitmap *mask;
    GdkPixmap *pixmap;
    GtkStyle *style;
    GtkWidget *button;
    GtkWidget *button_s;
    GtkWidget *button_d;
    GtkWidget *button_n;
    GtkWidget *button_m;

    chdir (getenv("HOME"));

    accel_group = gtk_accel_group_new ();

    main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (main_window), 500, 400);
    gtk_window_set_title (GTK_WINDOW (main_window), _("Программа Graph2D"));
    gtk_signal_connect (GTK_OBJECT (main_window), "delete_event",
			GTK_SIGNAL_FUNC (on_close_main_window), NULL);
    gtk_widget_show (main_window);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (main_window), main_vbox);
    gtk_widget_show (main_vbox);

    main_statusbar = gtk_statusbar_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "statusbar",
			 main_statusbar);
    gtk_box_pack_end (GTK_BOX (main_vbox), main_statusbar, FALSE, FALSE,
		      0);
    gtk_statusbar_push (GTK_STATUSBAR (main_statusbar), 1,
			_("Добро пожаловать в Graph2D"));
    gtk_widget_show (main_statusbar);

    menu_handlebox = gtk_handle_box_new ();
    gtk_box_pack_start (GTK_BOX (main_vbox), menu_handlebox, FALSE, TRUE,
			0);
    gtk_widget_show (menu_handlebox);

    menubar = gtk_menu_bar_new ();
    gtk_container_add (GTK_CONTAINER (menu_handlebox), menubar);
    gtk_widget_show (menubar);

    item_menu = gtk_menu_item_new_with_label (_("Файл"));
    gtk_container_add (GTK_CONTAINER (menubar), item_menu);
    gtk_widget_show (item_menu);

    main_menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (item_menu), main_menu);

    item = gtk_menu_item_new_with_label (_("Открыть..."));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_add_accelerator (item, "activate", accel_group,
				GDK_O, GDK_CONTROL_MASK,
				GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_open_dialog), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label (_("Сохранить"));
    gtk_object_set_data (GTK_OBJECT (main_window), "save", item);
    gtk_widget_set_sensitive (item, FALSE);
    gtk_widget_add_accelerator (item, "activate", accel_group,
				GDK_S, GDK_CONTROL_MASK,
				GTK_ACCEL_VISIBLE);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_save_dialog), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label (_("Сохранить как..."));
    gtk_object_set_data (GTK_OBJECT (main_window), "saveas", item);
    gtk_widget_set_sensitive (item, FALSE);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_saveas_dialog), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label (_("Палитра..."));
    gtk_object_set_data (GTK_OBJECT (main_window), "pal", item);
    gtk_widget_set_sensitive (item, FALSE);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_pal_dialog), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new ();
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label (_("Выход"));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_add_accelerator (item, "activate", accel_group,
				GDK_Q, GDK_CONTROL_MASK,
				GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_close_main_window), NULL);
    gtk_widget_show (item);

    item_menu = gtk_menu_item_new_with_label (_("Просмотр"));
    gtk_object_set_data (GTK_OBJECT (main_window), "view", item_menu);
    gtk_widget_set_sensitive (item_menu, FALSE);
    gtk_container_add (GTK_CONTAINER (menubar), item_menu);
    gtk_widget_show (item_menu);
    main_menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (item_menu), main_menu);

    item = gtk_menu_item_new_with_label (_("Увеличить"));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_add_accelerator (item, "activate", accel_group,
				GDK_equal, 0, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_zoomin), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label (_("Уменьшить"));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_add_accelerator (item, "activate", accel_group,
				GDK_minus, 0, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_zoomout), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new ();
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup,_("Сетка"));

    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_setka), NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_s", item);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup,_("Дефорации"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_d", item);
    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_def), NULL);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup,_("Напряжения"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_n", item);
    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_nap), NULL);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup,_("Поврежден."));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_m", item);
    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_mtr), NULL);
    gtk_widget_show (item);

    item = gtk_menu_item_new ();
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_show (item);
    
    item = gtk_check_menu_item_new_with_label (_("Интерполяция"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_inter", item);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_inter), NULL);
    gtk_widget_show (item);

    item = gtk_check_menu_item_new_with_label (_("Элементы"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_elem", item);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_elem), NULL);
    gtk_widget_show (item);

    item = gtk_check_menu_item_new_with_label (_("Узлы"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_knot", item);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_knot), NULL);
    gtk_widget_show (item);

    item_menu = gtk_menu_item_new_with_label (_("Справка"));
    gtk_container_add (GTK_CONTAINER (menubar), item_menu);
    gtk_menu_item_right_justify (GTK_MENU_ITEM (item_menu));
    gtk_widget_show (item_menu);
    main_menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (item_menu), main_menu);

    item = gtk_menu_item_new_with_label (_("О Программе"));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_about_dialog), NULL);
    gtk_widget_show (item);

    toolbar_handlebox = gtk_handle_box_new ();
    gtk_container_set_border_width (GTK_CONTAINER (toolbar_handlebox), 2);
    gtk_box_pack_start (GTK_BOX (main_vbox), toolbar_handlebox, FALSE,
			TRUE, 0);
    gtk_widget_show (toolbar_handlebox);

    toolbar =
	gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    gtk_container_add (GTK_CONTAINER (toolbar_handlebox), toolbar);
    gtk_toolbar_set_space_style (GTK_TOOLBAR (toolbar),
				 GTK_TOOLBAR_SPACE_LINE);
    gtk_toolbar_set_button_relief (GTK_TOOLBAR (toolbar), GTK_RELIEF_NONE);
    gtk_widget_show (toolbar);


    style = gtk_widget_get_style (main_window);
    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) open_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_BUTTON,
					 NULL,
					 _("Открыть"),
					 _("Открыть"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_open_dialog),
					 NULL);

    gtk_widget_show (button);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) save_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_BUTTON,
					 NULL,
					 _("Сохранить"),
					 _("Сохранить"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_save_dialog),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_save", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) pal_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_BUTTON,
					 NULL,
					 _("Палитра"),
					 _("Палитра"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_pal_dialog),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_pal", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);

    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) zoomin_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_BUTTON,
					 NULL,
					 _("Увеличить"),
					 _("Увеличить"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_zoomin),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_in", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) zoomout_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_BUTTON,
					 NULL,
					 _("Уменьшить"),
					 _("Уменьшить"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_zoomout),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_out", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);

    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) setka_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button_s = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					   GTK_TOOLBAR_CHILD_RADIOBUTTON,
					   NULL,
					   _("Сетка"),
					   _("Сетка"), NULL,
					   tmp_toolbar_icon,
					   GTK_SIGNAL_FUNC
					   (on_button_setka), NULL);
    gtk_widget_set_name (GTK_WIDGET (button_s),"setka");
    gtk_object_set_data (GTK_OBJECT (main_window), "button_s", button_s);
    gtk_widget_set_sensitive (button_s, FALSE);
    gtk_widget_show (button);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) epsilon_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button_d = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					   GTK_TOOLBAR_CHILD_RADIOBUTTON,
					   button_s,
					   _("Деформации"),
					   _("Деформации"), NULL,
					   tmp_toolbar_icon,
					   GTK_SIGNAL_FUNC (on_button_def),
					   NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_d", button_d);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (button_d), FALSE);
    gtk_widget_set_sensitive (button_d, FALSE);
    gtk_widget_show (button_d);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) sigma_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button_n = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					   GTK_TOOLBAR_CHILD_RADIOBUTTON,
					   button_s,
					   _("Напряжения"),
					   _("Напряжения"), NULL,
					   tmp_toolbar_icon,
					   GTK_SIGNAL_FUNC (on_button_nap),
					   NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_n", button_n);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (button_n), FALSE);
    gtk_widget_set_sensitive (button_n, FALSE);
    gtk_widget_show (button_n);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) trech_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button_m = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					   GTK_TOOLBAR_CHILD_RADIOBUTTON,
					   button_s,
					   _("Поврежден."),
					   _("Поврежден."), NULL,
					   tmp_toolbar_icon,
					   GTK_SIGNAL_FUNC (on_button_mtr),
					   NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_m", button_m);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (button_m), FALSE);
    gtk_widget_set_sensitive (button_m, FALSE);
    gtk_widget_show (button_m);

    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) elem_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
					 NULL,
					 _("Элементы"),
					 _("Элементы"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_button_elem),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_elem", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);

    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) knot_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
					 NULL,
					 _("Узлы"),
					 _("Узлы"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_button_knot),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_knot", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);
    
    pixmap = gdk_pixmap_create_from_xpm_d (main_window->window, &mask,
					   &style->bg[GTK_STATE_NORMAL],
					   (gchar **) inter_xpm);
    tmp_toolbar_icon = gtk_pixmap_new (pixmap, mask);
    gdk_pixmap_unref (pixmap);
    gdk_bitmap_unref (mask);
    button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
					 GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
					 NULL,
					 _("Интерполяция"),
					 _("Интерполяция"), NULL,
					 tmp_toolbar_icon,
					 GTK_SIGNAL_FUNC (on_button_inter),
					 NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "button_inter", button);
    gtk_widget_set_sensitive (button, FALSE);
    gtk_widget_show (button);

    scrolledwindow = gtk_scrolled_window_new (0, 0);
    gtk_box_pack_start (GTK_BOX (main_vbox), scrolledwindow, TRUE, TRUE,
			0);
    gtk_widget_show (scrolledwindow);

    viewport = gtk_viewport_new (0, 0);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), viewport);
    gtk_widget_show (viewport);

    table = gtk_table_new (1, 1, FALSE);
    gtk_container_add (GTK_CONTAINER (viewport), table);
    gtk_widget_show (table);

    drawelem = drawelem_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "drawelem", drawelem);
    gtk_table_attach (GTK_TABLE (table), drawelem, 0, 1, 0, 1,
		      (GtkAttachOptions) (GTK_EXPAND),
		      (GtkAttachOptions) (GTK_EXPAND), 0, 0);
    gtk_widget_show (drawelem);

    gtk_window_add_accel_group (GTK_WINDOW (main_window), accel_group);
    return (main_window);
};

GtkWidget *create_about_dialog (void)
{
    GtkWidget *about_dialog;
    GtkWidget *about_vbox;
    GtkWidget *about_hbox;
    GtkWidget *about_label;
    
    GtkWidget *about_frame;
    GtkWidget *about_pixmapwid;
    GdkBitmap *mask;
    GdkPixmap *about_pixmap;
    GtkStyle *style;

    about_dialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (about_dialog), _("О Программе"));
    gtk_widget_set_usize (GTK_WIDGET (about_dialog), 240, 140);
    gtk_window_set_policy (GTK_WINDOW (about_dialog), FALSE, FALSE, FALSE);
    gtk_widget_show (about_dialog);

    about_frame = gtk_frame_new (NULL);
    gtk_container_set_border_width (GTK_CONTAINER (about_frame), 3);
    gtk_container_add (GTK_CONTAINER (about_dialog), about_frame);
    gtk_widget_show (about_frame);
    
    about_hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (about_hbox), 5);
    gtk_container_add (GTK_CONTAINER (about_frame), about_hbox);
    gtk_widget_show (about_hbox);
    
    about_frame = gtk_frame_new (NULL);
    gtk_container_set_border_width (GTK_CONTAINER (about_frame), 0);
    gtk_box_pack_start (GTK_BOX (about_hbox),about_frame, FALSE,FALSE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (about_frame), GTK_SHADOW_IN);
    gtk_widget_show (about_frame);

    style = gtk_widget_get_style (about_dialog);
    about_pixmap =
	gdk_pixmap_create_from_xpm_d (about_dialog->window, &mask,
				      &style->bg[GTK_STATE_NORMAL],
				      (gchar **) about_xpm);
    about_pixmapwid = gtk_pixmap_new (about_pixmap, mask);
    gtk_widget_show (about_pixmapwid);
    gtk_container_add (GTK_CONTAINER (about_frame), about_pixmapwid);
    gtk_widget_show (about_pixmapwid);
    
    about_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX(about_hbox), about_vbox, FALSE, FALSE, 10);
    gtk_widget_show (about_vbox);
    
    about_pixmap =
	gdk_pixmap_create_from_xpm_d (about_dialog->window, &mask,
				      &style->bg[GTK_STATE_NORMAL],
				      (gchar **) logo_xpm);
    about_pixmapwid = gtk_pixmap_new (about_pixmap, mask);
    gtk_box_pack_start (GTK_BOX(about_vbox),about_pixmapwid, FALSE, FALSE, 0);
    gtk_widget_show (about_pixmapwid);
    
    about_label = gtk_label_new (_("Version 0.3.6\n\nCopyright (c) 2001 by\nRodionov Andrey\nAll rights reserved."));
    gtk_box_pack_start (GTK_BOX(about_vbox), about_label, FALSE, FALSE, 0);
    gtk_widget_show (about_label);

    return (about_dialog);
}

GtkWidget *create_open_dialog (void)
{
    GtkWidget *open_dialog;
    GtkWidget *ok_button;
    GtkWidget *cancel_button;

    open_dialog = gtk_file_selection_new (_("Открытие"));
    gtk_container_set_border_width (GTK_CONTAINER (open_dialog), 10);

    ok_button = GTK_FILE_SELECTION (open_dialog)->ok_button;
    gtk_widget_show (ok_button);
    GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

    cancel_button = GTK_FILE_SELECTION (open_dialog)->cancel_button;
    gtk_widget_show (cancel_button);
    GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (open_dialog), "delete_event",
			GTK_SIGNAL_FUNC (on_open_dialog_delete), NULL);
    gtk_signal_connect (GTK_OBJECT (ok_button), "clicked",
			GTK_SIGNAL_FUNC (on_open_dialog_ok), NULL);
    gtk_signal_connect (GTK_OBJECT (cancel_button), "clicked",
			GTK_SIGNAL_FUNC (on_open_dialog_cancel), NULL);

    return (open_dialog);
}

GtkWidget *create_save_dialog (void)
{
    GtkWidget *save_dialog;
    GtkWidget *ok_button;
    GtkWidget *cancel_button;

    save_dialog = gtk_file_selection_new (_("Сохранение"));
    gtk_container_set_border_width (GTK_CONTAINER (save_dialog), 10);

    ok_button = GTK_FILE_SELECTION (save_dialog)->ok_button;
    gtk_widget_show (ok_button);
    GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

    cancel_button = GTK_FILE_SELECTION (save_dialog)->cancel_button;
    gtk_widget_show (cancel_button);
    GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (save_dialog), "delete_event",
			GTK_SIGNAL_FUNC (on_save_dialog_delete), NULL);
    gtk_signal_connect (GTK_OBJECT (ok_button), "clicked",
			GTK_SIGNAL_FUNC (on_save_dialog_ok), NULL);
    gtk_signal_connect (GTK_OBJECT (cancel_button), "clicked",
			GTK_SIGNAL_FUNC (on_save_dialog_cancel), NULL);

    return (save_dialog);
}

GtkWidget *create_pal_dialog (void)
{
    GtkWidget *open_dialog;
    GtkWidget *ok_button;
    GtkWidget *cancel_button;

    chdir ("/usr/local/share/graph2d/palettes");

    open_dialog = gtk_file_selection_new (_("Открытие"));
    gtk_container_set_border_width (GTK_CONTAINER (open_dialog), 10);

    ok_button = GTK_FILE_SELECTION (open_dialog)->ok_button;
    gtk_widget_show (ok_button);
    GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

    cancel_button = GTK_FILE_SELECTION (open_dialog)->cancel_button;
    gtk_widget_show (cancel_button);
    GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (open_dialog), "delete_event",
			GTK_SIGNAL_FUNC (on_open_dialog_delete), NULL);
    gtk_signal_connect (GTK_OBJECT (ok_button), "clicked",
			GTK_SIGNAL_FUNC (on_pal_dialog_ok), NULL);
    gtk_signal_connect (GTK_OBJECT (cancel_button), "clicked",
			GTK_SIGNAL_FUNC (on_open_dialog_cancel), NULL);

    return (open_dialog);
}
