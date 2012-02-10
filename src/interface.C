#include "interface.h"
#include "pixmaps.h"
#include "intl.h"
#include <stdlib.h>
GtkWidget *create_main_window (void)
{
    GtkWidget *main_window;
    GtkWidget *main_vbox;
    GtkWidget *main_hbox;
    GtkWidget *main_statusbar;
    GtkWidget *main_statusbar_nds;

    GtkWidget *menubar;
    GtkWidget *item_menu;
    GtkWidget *main_menu;
    GtkAccelGroup *main_menu_accels;
    GtkWidget *item;
    GSList *menugroup = NULL;

    GtkWidget *menu_handlebox;
    GtkWidget *scrolledwindow;
    GtkWidget *table_drawelem;
    GtkWidget *table;
    GtkWidget *table_scrolledwindow;
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
    GtkWidget *hor_ruler;
    GtkWidget *ver_ruler;
    GtkWidget *hor_scrollbar;
    GtkWidget *ver_scrollbar;
    GtkObject *hor_adj;
    GtkObject *ver_adj;

    GtkWidget *hor_win_ruler;
    GtkWidget *ver_win_ruler;
    GtkWidget *hor_viewport;
    GtkWidget *ver_viewport;
    GtkWidget *frame_table;

    chdir (getenv ("HOME"));

    accel_group = gtk_accel_group_new ();

    main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (main_window), 450, 400);
    gtk_window_set_title (GTK_WINDOW (main_window),
			  _("Программа Graph2D"));
    gtk_signal_connect (GTK_OBJECT (main_window), "delete_event",
			GTK_SIGNAL_FUNC (on_close_main_window), NULL);
    gtk_widget_show (main_window);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (main_window), main_vbox);
    gtk_widget_show (main_vbox);

    main_hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_end (GTK_BOX (main_vbox), main_hbox, FALSE, FALSE, 0);
    gtk_widget_show (main_hbox);



    main_statusbar_nds = gtk_statusbar_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "statusbar_nds",
			 main_statusbar_nds);
    gtk_box_pack_end (GTK_BOX (main_hbox), main_statusbar_nds, FALSE,
		      FALSE, 0);
    gtk_statusbar_push (GTK_STATUSBAR (main_statusbar_nds), 1,
			_(" НДС: не обнаружено "));
    gtk_widget_show (main_statusbar_nds);

    main_statusbar = gtk_statusbar_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "statusbar",
			 main_statusbar);
    gtk_box_pack_end (GTK_BOX (main_hbox), main_statusbar, TRUE, TRUE, 0);
    gtk_statusbar_push (GTK_STATUSBAR (main_statusbar), 1,
			_(" Добро пожаловать в Graph2D"));
    gtk_widget_show (main_statusbar);

    menu_handlebox = gtk_handle_box_new ();
    gtk_box_pack_start (GTK_BOX (main_vbox), menu_handlebox, FALSE, TRUE,
			0);
    gtk_widget_show (menu_handlebox);
    gtk_widget_set_usize (menu_handlebox, 10, -2);

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
    
    item = gtk_menu_item_new ();
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label (_("Палитра..."));
    gtk_object_set_data (GTK_OBJECT (main_window), "pal", item);
    gtk_widget_set_sensitive (item, FALSE);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_pal_dialog), NULL);
    gtk_widget_show (item);
    
    item = gtk_menu_item_new_with_label (_("Настройки..."));
    gtk_object_set_data (GTK_OBJECT (main_window), "Options", item);
    gtk_widget_set_sensitive (item, TRUE);
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_options_dialog), NULL);
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

    item = gtk_radio_menu_item_new_with_label (menugroup, _("Сетка"));

    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_setka), NULL);
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_s", item);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup, _("Дефорации"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_d", item);
    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_def), NULL);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup, _("Напряжения"));
    gtk_object_set_data (GTK_OBJECT (main_window), "menu_n", item);
    menugroup = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (item));
    gtk_container_add (GTK_CONTAINER (main_menu), item);
    gtk_signal_connect (GTK_OBJECT (item), "activate",
			GTK_SIGNAL_FUNC (on_menu_nap), NULL);
    gtk_widget_show (item);

    item = gtk_radio_menu_item_new_with_label (menugroup, _("Поврежден."));
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
    gtk_object_set_data (GTK_OBJECT (main_window), "toolbar_handlebox", toolbar_handlebox);
    gtk_container_set_border_width (GTK_CONTAINER (toolbar_handlebox), 2);
    gtk_box_pack_start (GTK_BOX (main_vbox), toolbar_handlebox, FALSE,
			TRUE, 0);
    gtk_widget_show (toolbar_handlebox);
    gtk_widget_set_usize (toolbar_handlebox, 10, -2);

    toolbar =
	gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    
    gtk_object_set_data (GTK_OBJECT (main_window), "toolbar",toolbar);
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
    gtk_widget_set_name (GTK_WIDGET (button_s), "setka");
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

    table = gtk_table_new (2, 2, FALSE);
    gtk_box_pack_start (GTK_BOX (main_vbox), table, TRUE, TRUE, 0);
    gtk_widget_show (table);

    hor_adj = gtk_adjustment_new (0, 0, 400, 0.1, 1, 1);
    hor_scrollbar = gtk_hscrollbar_new (GTK_ADJUSTMENT (hor_adj));
    gtk_object_set_data (GTK_OBJECT (main_window), "hor_scrollbar",
			 hor_scrollbar);
    gtk_table_attach (GTK_TABLE (table), hor_scrollbar, 0, 1, 1, 2,
		      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_show (hor_scrollbar);

    ver_adj = gtk_adjustment_new (0, 0, 400, 0.1, 1, 1);
    gtk_object_set_data (GTK_OBJECT (main_window), "ver_adj", ver_adj);
    ver_scrollbar = gtk_vscrollbar_new (GTK_ADJUSTMENT (ver_adj));
    gtk_object_set_data (GTK_OBJECT (main_window), "ver_scrollbar",
			 ver_scrollbar);
    gtk_table_attach (GTK_TABLE (table), ver_scrollbar, 1, 2, 0, 1,
		      (GtkAttachOptions) (GTK_FILL),
		      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_widget_show (ver_scrollbar);

    table_scrolledwindow = gtk_table_new (2, 2, FALSE);
    gtk_table_attach (GTK_TABLE (table), table_scrolledwindow, 0, 1, 0, 1,
		      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_widget_show (table_scrolledwindow);


    scrolledwindow =
	gtk_scrolled_window_new (GTK_ADJUSTMENT (hor_adj),
				 GTK_ADJUSTMENT (ver_adj));
    gtk_object_set_data (GTK_OBJECT (main_window), "scrolledwindow",
			 scrolledwindow);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow),
				    GTK_POLICY_NEVER, GTK_POLICY_NEVER);
    gtk_widget_set_usize (scrolledwindow, 10, 10);
    gtk_object_set_data (GTK_OBJECT (main_window), "scrolledwindow",
			 scrolledwindow);
    gtk_table_attach (GTK_TABLE (table_scrolledwindow), scrolledwindow, 1,
		      2, 1, 2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

    gtk_widget_show (scrolledwindow);

    table_drawelem = gtk_table_new (1, 1, FALSE);
    gtk_object_set_data (GTK_OBJECT (main_window), "table_drawelem",
			 table_drawelem);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW
					   (scrolledwindow),
					   table_drawelem);
    gtk_widget_show (table_drawelem);

    drawelem = drawelem_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "drawelem", drawelem);
    gtk_signal_connect (GTK_OBJECT (drawelem), "motion_notify_event",
			(GtkSignalFunc) drawelem_motion_notify_event,
			NULL);
    gtk_widget_set_events (GTK_WIDGET (drawelem),
			   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK |
			   GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK
			   | GDK_POINTER_MOTION_HINT_MASK);
    gtk_table_attach (GTK_TABLE (table_drawelem), drawelem, 0, 1, 0, 1,
		      (GtkAttachOptions) (GTK_EXPAND),
		      (GtkAttachOptions) (GTK_EXPAND), 0, 0);
    gtk_widget_show (drawelem);

    hor_win_ruler =
	gtk_scrolled_window_new (GTK_ADJUSTMENT (hor_adj), NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (hor_win_ruler),
				    GTK_POLICY_NEVER, GTK_POLICY_NEVER);
    gtk_object_set_data (GTK_OBJECT (main_window), "hor_win_ruler",
			 hor_win_ruler);
    gtk_table_attach (GTK_TABLE (table_scrolledwindow), hor_win_ruler, 1,
		      2, 0, 1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_usize (hor_win_ruler, 1, 20);
    gtk_widget_show (hor_win_ruler);

    hor_viewport = gtk_viewport_new (NULL, NULL);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (hor_viewport),
				  GTK_SHADOW_NONE);
    gtk_container_add (GTK_CONTAINER (hor_win_ruler), hor_viewport);
    gtk_widget_show (hor_viewport);

    hor_ruler = gtk_hruler_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "hor_ruler", hor_ruler);
    gtk_ruler_set_metric (GTK_RULER (hor_ruler), GTK_PIXELS);
    gtk_ruler_set_range (GTK_RULER (hor_ruler), 0, 500, 0, 500);
    gtk_widget_set_usize (hor_ruler, 10, 10);
    gtk_signal_connect_object (GTK_OBJECT (drawelem),
			       "motion_notify_event",
			       (GtkSignalFunc)
			       GTK_WIDGET_CLASS (GTK_OBJECT (hor_ruler)->
						 klass)->
			       motion_notify_event,
			       GTK_OBJECT (hor_ruler));
    gtk_container_add (GTK_CONTAINER (hor_viewport), hor_ruler);

    gtk_widget_show (hor_ruler);

    ver_win_ruler =
	gtk_scrolled_window_new (NULL, GTK_ADJUSTMENT (ver_adj));
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ver_win_ruler),
				    GTK_POLICY_NEVER, GTK_POLICY_NEVER);
    gtk_object_set_data (GTK_OBJECT (main_window), "ver_win_ruler",
			 ver_win_ruler);
    gtk_table_attach (GTK_TABLE (table_scrolledwindow), ver_win_ruler, 0,
		      1, 1, 2, (GtkAttachOptions) (GTK_FILL),
		      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_widget_set_usize (ver_win_ruler, 20, 1);
    gtk_widget_show (ver_win_ruler);

    ver_viewport = gtk_viewport_new (NULL, GTK_ADJUSTMENT (ver_adj));
    gtk_object_set_data (GTK_OBJECT (main_window), "ver_viewport",
			 ver_viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (ver_viewport),
				  GTK_SHADOW_NONE);
    gtk_container_add (GTK_CONTAINER (ver_win_ruler), ver_viewport);
    gtk_widget_show (ver_viewport);

    ver_ruler = gtk_vruler_new ();
    gtk_object_set_data (GTK_OBJECT (main_window), "ver_ruler", ver_ruler);
    gtk_ruler_set_metric (GTK_RULER (ver_ruler), GTK_PIXELS);
    gtk_ruler_set_range (GTK_RULER (ver_ruler), 0, 400, 0, 400);

    gtk_signal_connect_object (GTK_OBJECT (drawelem),
			       "motion_notify_event",
			       (GtkSignalFunc)
			       GTK_WIDGET_CLASS (GTK_OBJECT (ver_ruler)->
						 klass)->
			       motion_notify_event,
			       GTK_OBJECT (ver_ruler));

    gtk_container_add (GTK_CONTAINER (ver_viewport), ver_ruler);
    gtk_widget_set_usize (ver_ruler, 10, 10);
    gtk_widget_show (ver_ruler);

    frame_table = gtk_frame_new (NULL);
    gtk_table_attach (GTK_TABLE (table_scrolledwindow), frame_table, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame_table), 1);
    gtk_frame_set_shadow_type (GTK_FRAME (frame_table), GTK_SHADOW_OUT);
    gtk_widget_show (frame_table);

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
    gtk_signal_connect (GTK_OBJECT (about_dialog), "delete_event",
			GTK_SIGNAL_FUNC (on_about_ok), NULL);
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
    gtk_box_pack_start (GTK_BOX (about_hbox), about_frame, FALSE, FALSE,
			0);
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
    gtk_box_pack_start (GTK_BOX (about_hbox), about_vbox, FALSE, FALSE,
			10);
    gtk_widget_show (about_vbox);

    about_pixmap =
	gdk_pixmap_create_from_xpm_d (about_dialog->window, &mask,
				      &style->bg[GTK_STATE_NORMAL],
				      (gchar **) logo_xpm);
    about_pixmapwid = gtk_pixmap_new (about_pixmap, mask);
    gtk_box_pack_start (GTK_BOX (about_vbox), about_pixmapwid, FALSE,
			FALSE, 0);
    gtk_widget_show (about_pixmapwid);

    about_label =
	gtk_label_new (_
		       ("Version 0.3.10\n\nCopyright (c) 2001 by\nRodionov Andrey\nAll rights reserved."));
    gtk_box_pack_start (GTK_BOX (about_vbox), about_label, FALSE, FALSE,
			0);
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


GtkWidget *create_options_dialog (void)
{
    GtkWidget *opt_dialog;
    GtkWidget *opt_vbox;
    GtkWidget *opt_notebook;
    GtkWidget *opt_frame_gui;
    GtkWidget *opt_label_gui;
    GtkWidget *opt_frame_scr;
    GtkWidget *opt_label_scr;
    GtkWidget *opt_hbox;
    GtkWidget *opt_button_ok;
    GtkWidget *opt_button_apply;
    GtkWidget *opt_button_cancel;
    
    GtkWidget *opt_vbox_gui;
  GtkWidget *opt_frame_elem;
  GtkWidget *opt_table_elem;
  GtkWidget *opt_label_elem_color;
  GtkWidget *opt_entry_elem_font;
  GtkWidget *opt_button_elem_font;
  GtkWidget *opt_button_elem_color;
  GtkWidget *opt_label_elem_font;
  GtkWidget *opt_frame_elem_color;
  GtkWidget *opt_drawingarea_elem_color;
  GtkWidget *opt_frame_knot;
  GtkWidget *opt_table_knot;
  GtkWidget *opt_label_knot_color;
  GtkWidget *opt_entry_knot_font;
  GtkWidget *opt_button_knot_font;
  GtkWidget *opt_button_knot_color;
  GtkWidget *opt_label_knot_font;
  GtkWidget *opt_frame_knot_color;
  GtkWidget *opt_drawingarea_knot_color;
  GtkWidget *opt_frame_toolbar;
  GtkWidget *opt_vbox_toolbar_toggle;
  GtkWidget *opt_hbox_toolbar;
  GSList *group_toolbar_check_group = NULL;
  GtkWidget *opt_radiobutton_toolbar_it;
  GtkWidget *opt_radiobutton_toolbar_i;
  GtkWidget *opt_radiobutton_toolbar_t;
  GtkWidget *opt_vbox_toolbar_check;
  GtkWidget *opt_check_show_toolbar;
  GtkWidget *opt_check_show_tooltip;
  GtkWidget *opt_check_raised_button;
  
  GdkPixmap *opt_pixmap_elem;
  GdkPixmap *opt_pixmap_knot;
    
    opt_dialog = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_dialog", opt_dialog);
    gtk_widget_set_usize (opt_dialog, 345, 365);
    gtk_window_set_title (GTK_WINDOW (opt_dialog), _("Настройки Graph2D"));
    gtk_window_set_position (GTK_WINDOW (opt_dialog), GTK_WIN_POS_MOUSE);
    //gtk_window_set_modal (GTK_WINDOW (opt_dialog), TRUE);
    gtk_window_set_policy (GTK_WINDOW (opt_dialog), FALSE, FALSE, FALSE);
    
    opt_vbox = gtk_vbox_new (FALSE, 0);
    gtk_object_set_data(GTK_OBJECT (opt_dialog), "opt_vbox", opt_vbox);
    gtk_container_add (GTK_CONTAINER (opt_dialog), opt_vbox);
    gtk_widget_show (opt_vbox);

    opt_notebook = gtk_notebook_new ();
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_notebook", opt_notebook);
    gtk_box_pack_start (GTK_BOX (opt_vbox), opt_notebook, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (opt_notebook), 5);
    gtk_widget_show (opt_notebook);
    
    opt_vbox_gui = gtk_vbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_vbox_gui", opt_vbox_gui);
  gtk_widget_show (opt_vbox_gui);
  gtk_container_add (GTK_CONTAINER (opt_notebook), opt_vbox_gui);

  opt_frame_elem = gtk_frame_new (_("Элементы:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_frame_elem", opt_frame_elem);
  gtk_widget_show (opt_frame_elem);
  gtk_box_pack_start (GTK_BOX (opt_vbox_gui), opt_frame_elem, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (opt_frame_elem), 5);

  opt_table_elem = gtk_table_new (2, 3, FALSE);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_table_elem", opt_table_elem);
  gtk_widget_show (opt_table_elem);
  gtk_container_add (GTK_CONTAINER (opt_frame_elem), opt_table_elem);
  gtk_container_set_border_width (GTK_CONTAINER (opt_table_elem), 5);
  gtk_table_set_row_spacings (GTK_TABLE (opt_table_elem), 10);

  opt_label_elem_color = gtk_label_new (_("Цвет:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_label_elem_color", opt_label_elem_color);
  gtk_widget_show (opt_label_elem_color);
  gtk_table_attach (GTK_TABLE (opt_table_elem), opt_label_elem_color, 0, 1, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  opt_entry_elem_font = gtk_entry_new ();
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_entry_elem_font", opt_entry_elem_font);
  gtk_widget_show (opt_entry_elem_font);
  gtk_table_attach (GTK_TABLE (opt_table_elem), opt_entry_elem_font, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  opt_button_elem_font = gtk_button_new_with_label (_("..."));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_elem_font", opt_button_elem_font);
  gtk_signal_connect (GTK_OBJECT (opt_button_elem_font), "clicked",
			GTK_SIGNAL_FUNC (on_button_elem_font), NULL);
  gtk_widget_show (opt_button_elem_font);
  gtk_table_attach (GTK_TABLE (opt_table_elem), opt_button_elem_font, 2, 3, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (opt_button_elem_font, 22, 22);

  opt_button_elem_color = gtk_button_new_with_label (_("..."));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_elem_color", opt_button_elem_color);
  gtk_signal_connect (GTK_OBJECT (opt_button_elem_color), "clicked",
			GTK_SIGNAL_FUNC (on_button_elem_color), NULL);
  gtk_widget_show (opt_button_elem_color);
  gtk_table_attach (GTK_TABLE (opt_table_elem), opt_button_elem_color, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (opt_button_elem_color, 22, 22);

  opt_label_elem_font = gtk_label_new (_("Шрифт:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_label_elem_font", opt_label_elem_font);
  gtk_widget_show (opt_label_elem_font);
  gtk_table_attach (GTK_TABLE (opt_table_elem), opt_label_elem_font, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 5, 0);

  opt_frame_elem_color = gtk_frame_new (NULL);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_frame_elem_color", opt_frame_elem_color);
  gtk_frame_set_shadow_type (GTK_FRAME (opt_frame_elem_color), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show (opt_frame_elem_color);
  gtk_table_attach (GTK_TABLE (opt_table_elem), opt_frame_elem_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  opt_drawingarea_elem_color = gtk_drawing_area_new ();
  gtk_widget_set_name (GTK_WIDGET (opt_drawingarea_elem_color), "opt_drawingarea_elem_color");
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_drawingarea_elem_color", opt_drawingarea_elem_color);
  gtk_signal_connect (GTK_OBJECT (opt_drawingarea_elem_color), "expose_event",
			(GtkSignalFunc) on_opt_expose_event_elem, NULL);
  gtk_signal_connect (GTK_OBJECT (opt_drawingarea_elem_color), "configure_event",
			(GtkSignalFunc)on_opt_configure_event_elem, NULL);
  gtk_widget_show (opt_drawingarea_elem_color);
  gtk_container_add (GTK_CONTAINER (opt_frame_elem_color), opt_drawingarea_elem_color);

  opt_frame_knot = gtk_frame_new (_("Узлы:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_frame_knot", opt_frame_knot);
  gtk_widget_show (opt_frame_knot);
  gtk_box_pack_start (GTK_BOX (opt_vbox_gui), opt_frame_knot, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (opt_frame_knot), 5);

  opt_table_knot = gtk_table_new (2, 3, FALSE);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_table_knot", opt_table_knot);
  gtk_widget_show (opt_table_knot);
  gtk_container_add (GTK_CONTAINER (opt_frame_knot), opt_table_knot);
  gtk_container_set_border_width (GTK_CONTAINER (opt_table_knot), 5);
  gtk_table_set_row_spacings (GTK_TABLE (opt_table_knot), 10);

  opt_label_knot_color = gtk_label_new (_("Цвет:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_label_knot_color", opt_label_knot_color);
  gtk_widget_show (opt_label_knot_color);
  gtk_table_attach (GTK_TABLE (opt_table_knot), opt_label_knot_color, 0, 1, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  opt_entry_knot_font = gtk_entry_new ();
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_entry_knot_font", opt_entry_knot_font);
  gtk_widget_show (opt_entry_knot_font);
  gtk_table_attach (GTK_TABLE (opt_table_knot), opt_entry_knot_font, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  opt_button_knot_font = gtk_button_new_with_label (_("..."));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_knot_font", opt_button_knot_font);
  gtk_signal_connect (GTK_OBJECT (opt_button_knot_font), "clicked",
			GTK_SIGNAL_FUNC (on_button_knot_font), NULL);
  gtk_widget_show (opt_button_knot_font);
  gtk_table_attach (GTK_TABLE (opt_table_knot), opt_button_knot_font, 2, 3, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (opt_button_knot_font, 22, 22);

  opt_button_knot_color = gtk_button_new_with_label (_("..."));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_knot_color", opt_button_knot_color);
  gtk_signal_connect (GTK_OBJECT (opt_button_knot_color), "clicked",
			GTK_SIGNAL_FUNC (on_button_knot_color), NULL);
  gtk_widget_show (opt_button_knot_color);
  gtk_table_attach (GTK_TABLE (opt_table_knot), opt_button_knot_color, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (opt_button_knot_color, 22, 22);

  opt_label_knot_font = gtk_label_new (_("Шрифт:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_label_knot_font", opt_label_knot_font);
  gtk_widget_show (opt_label_knot_font);
  gtk_table_attach (GTK_TABLE (opt_table_knot), opt_label_knot_font, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 5, 0);

  opt_frame_knot_color = gtk_frame_new (NULL);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_frame_knot_color", opt_frame_knot_color);
  gtk_frame_set_shadow_type (GTK_FRAME (opt_frame_knot_color), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show (opt_frame_knot_color);
  gtk_table_attach (GTK_TABLE (opt_table_knot), opt_frame_knot_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  opt_drawingarea_knot_color = gtk_drawing_area_new ();
  gtk_widget_set_name (GTK_WIDGET (opt_drawingarea_knot_color), "opt_drawingarea_knot_color");
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_drawingarea_knot_color", opt_drawingarea_knot_color);
  gtk_signal_connect (GTK_OBJECT (opt_drawingarea_knot_color), "expose_event",
			(GtkSignalFunc) on_opt_expose_event_knot, NULL);
  gtk_signal_connect (GTK_OBJECT (opt_drawingarea_knot_color), "configure_event",
			(GtkSignalFunc)on_opt_configure_event_knot, NULL);
  gtk_widget_show (opt_drawingarea_knot_color);
  gtk_container_add (GTK_CONTAINER (opt_frame_knot_color), opt_drawingarea_knot_color);

  opt_frame_toolbar = gtk_frame_new (_("Панель инструментов:"));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_frame_toolbar", opt_frame_toolbar);
  gtk_widget_show (opt_frame_toolbar);
  gtk_box_pack_start (GTK_BOX (opt_vbox_gui), opt_frame_toolbar, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (opt_frame_toolbar), 5);

  opt_hbox_toolbar = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data_full (GTK_OBJECT (opt_dialog), "opt_hbox_toolbar", opt_hbox_toolbar,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (opt_hbox_toolbar);
  gtk_container_add (GTK_CONTAINER (opt_frame_toolbar), opt_hbox_toolbar);
  gtk_container_set_border_width (GTK_CONTAINER (opt_hbox_toolbar), 5);

  opt_vbox_toolbar_toggle = gtk_vbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_vbox_toolbar_toggle", opt_vbox_toolbar_toggle);
  gtk_widget_show (opt_vbox_toolbar_toggle);
  gtk_box_pack_start (GTK_BOX (opt_hbox_toolbar), opt_vbox_toolbar_toggle, TRUE, TRUE, 0);

  opt_radiobutton_toolbar_it = gtk_radio_button_new_with_label (group_toolbar_check_group, _("Иконка и текст"));
  group_toolbar_check_group = gtk_radio_button_group (GTK_RADIO_BUTTON (opt_radiobutton_toolbar_it));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_radiobutton_toolbar_it", opt_radiobutton_toolbar_it);
  gtk_widget_show (opt_radiobutton_toolbar_it);
  gtk_box_pack_start (GTK_BOX (opt_vbox_toolbar_toggle), opt_radiobutton_toolbar_it, FALSE, FALSE, 0);

  opt_radiobutton_toolbar_i = gtk_radio_button_new_with_label (group_toolbar_check_group, _("Только иконка"));
  group_toolbar_check_group = gtk_radio_button_group (GTK_RADIO_BUTTON (opt_radiobutton_toolbar_i));
  //gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(opt_radiobutton_toolbar_i),TRUE);
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_radiobutton_toolbar_i", opt_radiobutton_toolbar_i);
  gtk_widget_show (opt_radiobutton_toolbar_i);
  gtk_box_pack_start (GTK_BOX (opt_vbox_toolbar_toggle), opt_radiobutton_toolbar_i, FALSE, FALSE, 1);

  opt_radiobutton_toolbar_t = gtk_radio_button_new_with_label (group_toolbar_check_group, _("Только текст"));
  group_toolbar_check_group = gtk_radio_button_group (GTK_RADIO_BUTTON (opt_radiobutton_toolbar_t));
  gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_radiobutton_toolbar_t", opt_radiobutton_toolbar_t);
  gtk_widget_show (opt_radiobutton_toolbar_t);
  gtk_box_pack_start (GTK_BOX (opt_vbox_toolbar_toggle), opt_radiobutton_toolbar_t, FALSE, FALSE, 0);

  opt_vbox_toolbar_check = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (opt_vbox_toolbar_check);
  gtk_object_set_data_full (GTK_OBJECT (opt_dialog), "opt_vbox_toolbar_check", opt_vbox_toolbar_check,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (opt_vbox_toolbar_check);
  gtk_box_pack_start (GTK_BOX (opt_hbox_toolbar), opt_vbox_toolbar_check, TRUE, TRUE, 0);

  opt_check_show_toolbar = gtk_check_button_new_with_label (_("Показывать панель"));
  gtk_widget_ref (opt_check_show_toolbar);
  gtk_object_set_data_full (GTK_OBJECT (opt_dialog), "opt_check_show_toolbar", opt_check_show_toolbar,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (opt_check_show_toolbar);
  gtk_box_pack_start (GTK_BOX (opt_vbox_toolbar_check), opt_check_show_toolbar, TRUE, TRUE, 0);

  opt_check_show_tooltip = gtk_check_button_new_with_label (_("Показывать подсказки"));
  gtk_widget_ref (opt_check_show_tooltip);
  gtk_object_set_data_full (GTK_OBJECT (opt_dialog), "opt_check_show_tooltip", opt_check_show_tooltip,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (opt_check_show_tooltip);
  gtk_box_pack_start (GTK_BOX (opt_vbox_toolbar_check), opt_check_show_tooltip, TRUE, TRUE, 0);

  opt_check_raised_button = gtk_check_button_new_with_label (_("Плавающие кнопки"));
  gtk_widget_ref (opt_check_raised_button);
  gtk_object_set_data_full (GTK_OBJECT (opt_dialog), "opt_check_raised_button", opt_check_raised_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (opt_check_raised_button);
  gtk_box_pack_start (GTK_BOX (opt_vbox_toolbar_check), opt_check_raised_button, TRUE, FALSE, 0);

    opt_label_gui = gtk_label_new (_("Интерфейс"));
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_label_gui", opt_label_gui);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (opt_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (opt_notebook), 0), opt_label_gui);
    gtk_widget_show (opt_label_gui);

    opt_frame_scr = gtk_frame_new (NULL);
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_frame_scr", opt_frame_scr);
    gtk_container_add (GTK_CONTAINER (opt_notebook), opt_frame_scr);
    gtk_container_set_border_width (GTK_CONTAINER (opt_frame_scr), 5);
    gtk_widget_show (opt_frame_scr);
  
    opt_label_scr = gtk_label_new (_("Скрипты"));
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_label_scr", opt_label_scr);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (opt_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (opt_notebook), 1), opt_label_scr);
    gtk_widget_show (opt_label_scr);
  
    opt_hbox = gtk_hbox_new (FALSE, 0);
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_hbox", opt_hbox);
    gtk_box_pack_start (GTK_BOX (opt_vbox), opt_hbox, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (opt_hbox), 5);
    gtk_widget_show (opt_hbox);
  
    opt_button_ok = gtk_button_new_with_label (_("Ok"));
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_ok", opt_button_ok);
    gtk_signal_connect (GTK_OBJECT (opt_button_ok), "clicked",
			GTK_SIGNAL_FUNC (on_options_dialog_ok), NULL);
    gtk_box_pack_start (GTK_BOX (opt_hbox), opt_button_ok, TRUE, FALSE, 0);
    gtk_widget_set_usize (opt_button_ok, 70, -2);
    gtk_widget_show (opt_button_ok);
    
    opt_button_apply = gtk_button_new_with_label (_("Apply"));
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_apply", opt_button_apply);
    gtk_signal_connect (GTK_OBJECT (opt_button_apply), "clicked",
			GTK_SIGNAL_FUNC (on_options_dialog_apply), NULL);
    gtk_box_pack_start (GTK_BOX (opt_hbox), opt_button_apply, TRUE, FALSE, 0);
    gtk_widget_set_usize (opt_button_apply, 70, -2);
    gtk_widget_show (opt_button_apply);

    opt_button_cancel = gtk_button_new_with_label (_("Cancel"));
    gtk_object_set_data (GTK_OBJECT (opt_dialog), "opt_button_cancel", opt_button_cancel);
    gtk_signal_connect (GTK_OBJECT (opt_button_cancel), "clicked",
			GTK_SIGNAL_FUNC (on_options_dialog_cancel), NULL);
    gtk_box_pack_start (GTK_BOX (opt_hbox), opt_button_cancel, TRUE, FALSE, 0);
    gtk_widget_set_usize (opt_button_cancel, 70, -2);
    gtk_widget_show (opt_button_cancel);
    
    return(opt_dialog);
}

GtkWidget* create_font_dialog (void)
{
  GtkWidget *font_dialog;
  GtkWidget *font_dialog_ok;
  GtkWidget *font_dialog_cancel;
  GtkWidget *font_dialog_apply;

  font_dialog = gtk_font_selection_dialog_new (_("Select Font"));
  gtk_object_set_data (GTK_OBJECT (font_dialog), "font_dialog", font_dialog);
  gtk_container_set_border_width (GTK_CONTAINER (font_dialog), 4);
  gtk_window_set_modal (GTK_WINDOW (font_dialog), TRUE);
  gtk_window_set_policy (GTK_WINDOW (font_dialog), FALSE, TRUE, TRUE);

  font_dialog_ok = GTK_FONT_SELECTION_DIALOG (font_dialog)->ok_button;
  gtk_object_set_data (GTK_OBJECT (font_dialog), "font_dialog_ok", font_dialog_ok);
  gtk_signal_connect (GTK_OBJECT (font_dialog_ok), "clicked",
			GTK_SIGNAL_FUNC (on_font_dialog_ok), NULL);
  gtk_widget_show (font_dialog_ok);
  GTK_WIDGET_SET_FLAGS (font_dialog_ok, GTK_CAN_DEFAULT);

  font_dialog_cancel = GTK_FONT_SELECTION_DIALOG (font_dialog)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (font_dialog), "font_dialog_cancel", font_dialog_cancel);
  gtk_signal_connect (GTK_OBJECT (font_dialog_cancel), "clicked",
			GTK_SIGNAL_FUNC (on_font_dialog_cancel), NULL);
  gtk_widget_show (font_dialog_cancel);
  GTK_WIDGET_SET_FLAGS (font_dialog_cancel, GTK_CAN_DEFAULT);

  font_dialog_apply = GTK_FONT_SELECTION_DIALOG (font_dialog)->apply_button;
  gtk_object_set_data (GTK_OBJECT (font_dialog), "font_dialog_apply", font_dialog_apply);
  gtk_signal_connect (GTK_OBJECT (font_dialog_apply), "clicked",
			GTK_SIGNAL_FUNC (on_font_dialog_apply), NULL);
  gtk_widget_show (font_dialog_apply);
  GTK_WIDGET_SET_FLAGS (font_dialog_apply, GTK_CAN_DEFAULT);

  return font_dialog;
}

GtkWidget* create_color_dialog (void)
{
  GtkWidget *color_dialog;
  GtkWidget *color_dialog_ok;
  GtkWidget *color_dialog_cancel;
  GtkWidget *color_dialog_help;

  color_dialog = gtk_color_selection_dialog_new (_("Select Color"));
  gtk_object_set_data (GTK_OBJECT (color_dialog), "color_dialog", color_dialog);
  gtk_window_set_modal (GTK_WINDOW (color_dialog), TRUE);
  gtk_container_set_border_width (GTK_CONTAINER (color_dialog), 10);

  color_dialog_ok = GTK_COLOR_SELECTION_DIALOG (color_dialog)->ok_button;
  gtk_object_set_data (GTK_OBJECT (color_dialog), "color_dialog_ok", color_dialog_ok);
  gtk_signal_connect (GTK_OBJECT (color_dialog_ok), "clicked",
			GTK_SIGNAL_FUNC (on_color_dialog_ok), NULL);
  gtk_widget_show (color_dialog_ok);
  GTK_WIDGET_SET_FLAGS (color_dialog_ok, GTK_CAN_DEFAULT);

  color_dialog_cancel = GTK_COLOR_SELECTION_DIALOG (color_dialog)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (color_dialog), "color_dialog_cancel", color_dialog_cancel);
  gtk_signal_connect (GTK_OBJECT (color_dialog_cancel), "clicked",
			GTK_SIGNAL_FUNC (on_color_dialog_cancel), NULL);
  gtk_widget_show (color_dialog_cancel);
  GTK_WIDGET_SET_FLAGS (color_dialog_cancel, GTK_CAN_DEFAULT);

  color_dialog_help = GTK_COLOR_SELECTION_DIALOG (color_dialog)->help_button;
  gtk_object_set_data (GTK_OBJECT (color_dialog), "color_dialog_help", color_dialog_help);
  gtk_signal_connect (GTK_OBJECT (color_dialog_help), "clicked",
			GTK_SIGNAL_FUNC (on_color_dialog_help), NULL);
  gtk_widget_show (color_dialog_help);
  GTK_WIDGET_SET_FLAGS (color_dialog_help, GTK_CAN_DEFAULT);

  return color_dialog;
}