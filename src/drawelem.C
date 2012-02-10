#include "drawelem.h"
#include "colormap.h"
GdkColor *color_pal = NULL;
int ncolor_pal = 0;
int sdn = 0;
int selem = 0;
int sknot = 0;
static void drawelem_class_init (DrawElemClass * klass);
static void drawelem_init (DrawElem * det);

// Нужно для интерполяции
float *nds_knot;
int *nds_knotn;
int sinter = 0;

void drawelem_show_nelem (void)
{
    if (selem == 0)
	selem = 1;
    else
	selem = 0;
}

void drawelem_inter (void)
{
    if (sinter == 0)
	sinter = 1;
    else
	sinter = 0;
}

void drawelem_show_nknot (void)
{
    if (sknot == 0)
	sknot = 1;
    else
	sknot = 0;
}

void drawelem_set (int s)
{
    sdn = s;
}

char *float_to_char (float n)
{
    int x, nn;
    int i, j;
    char *st = NULL;
    int flag = 0;

    nn = int (n);
    j = 0;
    if (int (1000 * n) > int (n) * 1000) {
	nn = int (n * 1000);
	if (n < 1)
	    j++;
	flag++;
    }
    x = nn;
    while (x > 0) {
	x = int (x / 10);
	j++;
    }

    x = nn;

    if (flag == 1)
	j++;

    if ((j < 5) && (flag == 1))
	j = 5;

    st = new char[j];
    st[j] = *"\0";

    if (n < 1) {
	st[0] = *"0";
	j--;
    }

    for (i = 1; i <= j; i++) {
	if ((flag == 1) && (i == 4)) {
	    st[strlen (st) - i] = *".";
	} else {
	    nn = int (nn / 10);
	    x = x - nn * 10;
	    st[strlen (st) - i] = char (int (*"0") + x);
	    x = nn;
	}
    }
    return (st);
}

float area_triangle (float x1, float y1, float x2, float y2, float x3,
		     float y3)
{
    float s = (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2;
    if (s < 0)
	s = s * (-1);
    return (s);
}

float len (float x1, float y1, float x2, float y2)
{
    return (sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

float koef (float x1, float y1, float x2, float y2, float x3, float y3,
	    float x4, float y4)
{
    float b1, c1, b2, c2, x, y;

    if ((x1 - x2) != 0)
	b1 = (y1 - y2) / (x1 - x2);
    else
	b1 = (y1 - y2) / (0.0000001);
    c1 = y1 - b1 * x1;

    if ((x3 - x4) != 0)
	b2 = (y3 - y4) / (x3 - x4);
    else
	b2 = (y3 - y4) / (0.0000001);
    c2 = y3 - b2 * x3;

    if ((b1 - b2) != 0)
	x = (c2 - c1) / (b1 - b2);
    else
	x = (c2 - c1) / (0.0000001);
    y = b1 * x + c1;

    if (len (x2, y2, x, y) != 0)
	return (len (x1, y1, x, y) / len (x2, y2, x, y));
    else
	return (len (x1, y1, x, y) / 0.0000001);
}

void drawelem_size (DrawElem * de, gint width, gint height)
{
    gtk_drawing_area_size (GTK_DRAWING_AREA (de), width, height);
}

static gint configure_event (GtkWidget * widget, GdkEventConfigure * event)
{
    if (!DRAWELEM (widget)->pixmap) {

	if (DRAWELEM (widget)->pixmap != NULL)
	    gdk_pixmap_unref (DRAWELEM (widget)->pixmap);
	DRAWELEM (widget)->pixmap = gdk_pixmap_new (widget->window,
						    widget->
						    allocation.width,
						    widget->
						    allocation.height, -1);

	gdk_draw_rectangle (DRAWELEM (widget)->pixmap,
			    widget->style->white_gc,
			    TRUE,
			    0, 0,
			    widget->allocation.width,
			    widget->allocation.height);
    }

    return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gint expose_event (GtkWidget * widget, GdkEventExpose * event)
{
    gdk_draw_pixmap (widget->window,
		     widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		     DRAWELEM (widget)->pixmap,
		     event->area.x, event->area.y,
		     event->area.x, event->area.y,
		     widget->allocation.width, widget->allocation.height);
    return FALSE;
}

void draw_triangle (GdkDrawable * pixmap, GdkGC * color, gint filled,
		    int x1, int y1, int x2, int y2, int x3, int y3)
{
    GdkPoint points[3];

    points[0].x = x1;
    points[0].y = y1;

    points[1].x = x2;
    points[1].y = y2;

    points[2].x = x3;
    points[2].y = y3;

    gdk_draw_polygon (pixmap, color, filled, points, 3);
}

GdkGC *color_to_gc (GtkWidget * widget, GdkColor * fcolor,
		    GdkColormap * colormap)
{
    GdkColor color;
    GdkGC *colorgc;
    GdkGCValues gc_values;
    color.red = fcolor->red * 256;
    color.green = fcolor->green * 256;
    color.blue = fcolor->blue * 256;
    if (!gdk_color_alloc (colormap, &color))
	g_print ("Ошибка: Цвет не найден.\n ");
    gc_values.foreground = color;
    colorgc = gdk_gc_new_with_values (GTK_WIDGET (widget)->window,
				      &gc_values, GDK_GC_FOREGROUND);
    return (colorgc);
};

void draw_nknot (DrawElem * det)
{
    GdkFont *font =
	gdk_font_load
	("-adobe-helvetica-medium-r-normal--*-80-*-*-*-*-*-*");
    GdkColormap *colormap;
    GdkGC *colorgc;
    GdkColor color = { 0, 0, 0, 65535 };
    char *text = NULL;
    int i;

    colormap = gdk_colormap_get_system ();
    colorgc = color_to_gc (GTK_WIDGET (det), &color, colormap);

    for (i = 0; i < det->nknot; i++) {
	text = float_to_char (float (i + 1));
	gdk_draw_text (DRAWELEM (det)->pixmap,
		       font,
		       colorgc,
		       int (det->mash * det->knot_x[i]) + det->zero_x + 2,
		       -int (det->mash * det->knot_y[i]) + det->zero_y - 2,
		       text, strlen (text));
	delete (text);
    }

    gdk_gc_unref (colorgc);
    gdk_colormap_unref (colormap);
    gdk_font_unref (font);
}

void draw_nelem (DrawElem * det)
{
    GdkFont *font =
	gdk_font_load
	("-adobe-helvetica-medium-r-normal--*-80-*-*-*-*-*-*");
    GdkColormap *colormap;
    GdkGC *colorgc;
    GdkColor color = { 0, 65535, 0, 0 };
    char *text = NULL;
    int i;
    float x, y;

    colormap = gdk_colormap_get_system ();
    colorgc = color_to_gc (GTK_WIDGET (det), &color, colormap);

    for (i = 0; i < det->nelem; i++) {
	x =
	    (det->knot_x[det->elem[0][i] - 1] +
	     det->knot_x[det->elem[1][i] - 1] +
	     det->knot_x[det->elem[2][i] - 1]) / 3;
	y =
	    (det->knot_y[det->elem[0][i] - 1] +
	     det->knot_y[det->elem[1][i] - 1] +
	     det->knot_y[det->elem[2][i] - 1]) / 3;

	text = float_to_char (float (i + 1));
	gdk_draw_text (DRAWELEM (det)->pixmap,
		       font,
		       colorgc,
		       int (det->mash * x) + det->zero_x -
		       int (gdk_string_width (font, text) / 2),
		       -int (det->mash * y) + det->zero_y +
		       int (gdk_string_height (font, "0") / 2), text,
		       strlen (text));
	delete (text);
    }

    gdk_gc_unref (colorgc);
    gdk_colormap_unref (colormap);
    gdk_font_unref (font);
}

void drawelem_show_elem (DrawElem * det)
{
    GdkGC *colorgc;
    GdkColormap *colormap;
    GdkFont *font;

    int num, i, th;
    gint csize;
    float maxn, minn, inter;
    int colline;
    int width, height;
    char *text = NULL;
    // Переменные для интерполяции
    int max_Y, min_Y, k, max_X, min_X, l, m, n, j;
    float S1, S2, nd;
    float kx[3];
    
    colormap = gdk_colormap_get_system ();

    if (color_pal == NULL)
	csize = 256;
    else
	csize = ncolor_pal;

    if (sdn != 0) {
	maxn = det->nds[0];
	minn = maxn;

	for (int i = 0; i < det->nelem; i++) {
	    if (maxn < det->nds[i])
		maxn = det->nds[i];
	    if (minn > det->nds[i])
		minn = det->nds[i];
	}
	inter = (maxn - minn) / csize;
    }

    /* Вывод сетки или заливка заготовки */
    for (i = 0; i < det->nelem; i++) {
	if (sdn != 0) {
	    if (sinter == 0) {
		num = int ((det->nds[i] - minn) / inter);
		if ((inter * num) > (det->nds[i] - minn))
		    num--;
		if (color_pal == NULL)
		    colorgc =
			color_to_gc (GTK_WIDGET (det), &default_color[num],
				     colormap);
		else
		    colorgc =
			color_to_gc (GTK_WIDGET (det), &color_pal[num],
				     colormap);

		draw_triangle (det->pixmap, colorgc, TRUE,
			       int (det->mash *
				    det->knot_x[det->elem[0][i] - 1]) +
			       det->zero_x,
			       -int (det->mash *
				     det->knot_y[det->elem[0][i] - 1]) +
			       det->zero_y,
			       int (det->mash *
				    det->knot_x[det->elem[1][i] - 1]) +
			       det->zero_x,
			       -int (det->mash *
				     det->knot_y[det->elem[1][i] - 1]) +
			       det->zero_y,
			       int (det->mash *
				    det->knot_x[det->elem[2][i] - 1]) +
			       det->zero_x,
			       -int (det->mash *
				     det->knot_y[det->elem[2][i] - 1]) +
			       det->zero_y);
		gdk_gc_unref (colorgc);
	    } else {

		max_Y = int (det->knot_y[det->elem[0][i] - 1] * det->mash);
		min_Y = int (det->knot_y[det->elem[0][i] - 1] * det->mash);
		max_X = int (det->knot_x[det->elem[0][i] - 1] * det->mash);
		min_X = int (det->knot_x[det->elem[0][i] - 1] * det->mash);

		for (j = 1; j < 3; j++) {
		    if (int (det->knot_y[det->elem[j][i] - 1] * det->mash)
			> max_Y)
			max_Y =
			    int (det->knot_y[det->elem[j][i] - 1] *
				 det->mash);
		    if (int (det->knot_y[det->elem[j][i] - 1] * det->mash)
			< min_Y)
			min_Y =
			    int (det->knot_y[det->elem[j][i] - 1] *
				 det->mash);

		    if (int (det->knot_x[det->elem[j][i] - 1] * det->mash)
			> max_X)
			max_X =
			    int (det->knot_x[det->elem[j][i] - 1] *
				 det->mash);
		    if (int (det->knot_x[det->elem[j][i] - 1] * det->mash)
			< min_X)
			min_X =
			    int (det->knot_x[det->elem[j][i] - 1] *
				 det->mash);
		}

		for (j = min_Y; j <= max_Y; j++)
		    for (k = min_X; k <= max_X; k++) {
			S1 =
			    area_triangle (
					   (det->
					    knot_x[det->elem[0][i] -
						   1] * det->mash),
					   (det->
					    knot_y[det->elem[0][i] -
						   1] * det->mash),
					   (det->
					    knot_x[det->elem[1][i] -
						   1] * det->mash),
					   (det->
					    knot_y[det->elem[1][i] -
						   1] * det->mash),
					   (det->
					    knot_x[det->elem[2][i] -
						   1] * det->mash),
					   (det->
					    knot_y[det->elem[2][i] -
						   1] * det->mash));
			S2 = 0;
			for (l = 0; l < 3; l++) {
			    m = l + 1;
			    if (m > 2)
				m = 0;
			    S2 =
				S2 +
				area_triangle (
					       (det->
						knot_x[det->elem[l][i] -
						       1] * det->mash),
					       (det->
						knot_y[det->elem[l][i] -
						       1] * det->mash),
					       (det->
						knot_x[det->elem[m][i] -
						       1] * det->mash),
					       (det->
						knot_y[det->elem[m][i] -
						       1] * det->mash), k,
					       j);

			}

			nd = S2 - S1;
			if (nd < 0)
			    nd = nd * (-1);

			if (nd < 0.3) {

			    for (l = 0; l < 3; l++) {
				m = l + 1;
				n = m + 1;

				if (m > 2)
				    m = m - 3;
				if (n > 2)
				    n = n - 3;

				kx[l] = koef (k, j,
					      (det->
					       knot_x[det->elem[l][i] -
						      1] * det->mash),
					      (det->
					       knot_y[det->elem[l][i] -
						      1] * det->mash),
					      (det->
					       knot_x[det->elem[m][i] -
						      1] * det->mash),
					      (det->
					       knot_y[det->elem[m][i] -
						      1] * det->mash),
					      (det->
					       knot_x[det->elem[n][i] -
						      1] * det->mash),
					      (det->
					       knot_y[det->elem[n][i] -
						      1] * det->mash));
			    }

			    nd = 0;

			    for (l = 0; l < 3; l++)
				nd =
				    nd + kx[l] * nds_knot[det->elem[l][i] -
							  1];

			    num = int ((nd - minn) / inter);

			    if ((inter * num) > (nd - minn))
				num--;
			    if (color_pal == NULL)
				colorgc =
				    color_to_gc (GTK_WIDGET (det),
						 &default_color[num],
						 colormap);
			    else
				colorgc =
				    color_to_gc (GTK_WIDGET (det),
						 &color_pal[num],
						 colormap);

			    gdk_draw_point (det->pixmap, colorgc,
					    k + det->zero_x,
					    -j + det->zero_y);

			    gdk_gc_unref (colorgc);
			}
		    }
	    }
	} else
	    draw_triangle (det->pixmap, GTK_WIDGET (det)->style->black_gc,
			   FALSE,
			   int (det->mash *
				det->knot_x[det->elem[0][i] - 1]) +
			   det->zero_x,
			   -int (det->mash *
				 det->knot_y[det->elem[0][i] - 1]) +
			   det->zero_y,
			   int (det->mash *
				det->knot_x[det->elem[1][i] - 1]) +
			   det->zero_x,
			   -int (det->mash *
				 det->knot_y[det->elem[1][i] - 1]) +
			   det->zero_y,
			   int (det->mash *
				det->knot_x[det->elem[2][i] - 1]) +
			   det->zero_x,
			   -int (det->mash *
				 det->knot_y[det->elem[2][i] - 1]) +
			   det->zero_y);
    }

    if (selem != 0)
	draw_nelem (det);
    if (sknot != 0)
	draw_nknot (det);

    if (sdn == 0)
	return;

    /* Вывод градиента  */
    if (csize < 128)
	colline = int (256 / csize);
    else
	colline = 1;

    gdk_window_get_size (det->pixmap, &width, &height);

    for (i = 0; i < csize; i++) {
	if (color_pal == NULL)
	    colorgc =
		color_to_gc (GTK_WIDGET (det), &default_color[i],
			     colormap);
	else
	    colorgc =
		color_to_gc (GTK_WIDGET (det), &color_pal[i], colormap);

	gdk_draw_rectangle (DRAWELEM (det)->pixmap,
			    colorgc,
			    TRUE,
			    width - 100, 10 + colline * i, 20, colline);
	gdk_gc_unref (colorgc);
    }

    /* Вывод значений градиента */
    font =
	gdk_font_load
	("-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*");
    th = gdk_string_height (font, "H") + 3;

    inter = (maxn - minn) / int (csize * colline / th);
    for (i = 1; i <= int (csize * colline / th); i++) {
	text = float_to_char (minn + (i) * inter + inter / 2);
	gdk_draw_text (DRAWELEM (det)->pixmap,
		       font,
		       GTK_WIDGET (det)->style->black_gc,
		       width - 70, 10 + i * th, text, strlen (text));
	delete (text);
    }
    gdk_font_unref (font);

    gdk_colormap_unref (colormap);

    return;
}

void drawelem_center (DrawElem * det)
{
    float maxx, minx, maxy, miny;
    gint width, height;

    maxx = minx = det->knot_x[0];
    maxy = miny = det->knot_y[0];

    for (int i = 0; i < det->nknot; i++) {
	if (maxx < (det->knot_x[i]))
	    maxx = det->knot_x[i];
	if (maxy < (det->knot_y[i]))
	    maxy = det->knot_y[i];
	if (minx > (det->knot_x[i]))
	    minx = det->knot_x[i];
	if (miny > (det->knot_y[i]))
	    miny = det->knot_y[i];
    }

    gdk_window_get_size (det->pixmap, &width, &height);

    det->zero_x = int ((width - 125 - (maxx + minx) * det->mash) / 2);
    det->zero_y = int ((height + (maxy + miny) * det->mash) / 2);
}

void drawelem_fit (DrawElem * det)
{
    float maxx, minx, maxy, miny;
    gint width, height;

    maxx = minx = det->knot_x[0];
    maxy = miny = det->knot_y[0];

    for (int i = 0; i < det->nknot; i++) {
	if (maxx < det->knot_x[i])
	    maxx = det->knot_x[i];
	if (maxy < det->knot_y[i])
	    maxy = det->knot_y[i];
	if (minx > det->knot_x[i])
	    minx = det->knot_x[i];
	if (miny > det->knot_y[i])
	    miny = det->knot_y[i];
    }

    width = int ((maxx - minx) * det->mash) + 145;
    height = int ((maxy - miny) * det->mash) + 20;

    if (height < 300)
	height = 300;

    det->zero_x = int ((width - 125 - (maxx + minx) * det->mash) / 2);
    det->zero_y = int ((height + (maxy + miny) * det->mash) / 2);

    drawelem_size (det, width, height);
    gdk_pixmap_unref (det->pixmap);
    det->pixmap = gdk_pixmap_new (GTK_WIDGET (det)->window,
				  width, height, -1);

    gdk_draw_rectangle (det->pixmap,
			GTK_WIDGET (det)->style->white_gc,
			TRUE, 0, 0, width, height);

    drawelem_show_elem (det);
}

void load_knot (DrawElem * det, char *path)
{
    FILE *fd;
    float tmp;
    int nuz, n, i;
    char s[80];

    nuz = 0;
    fd = fopen (path, "r");
    while (!feof (fd)) {
	i = fscanf (fd, "%d %f %f %f", &n, &tmp, &tmp, &tmp);
	if (i == 4)
	    nuz++;
	else
	    fgets (s, 80, fd);
    }
    fclose (fd);

    if (det->knot_x != NULL)
	delete (det->knot_x);
    if (det->knot_y != NULL)
	delete (det->knot_y);

    det->knot_x = new float[nuz];
    det->knot_y = new float[nuz];

    nuz = 0;
    fd = fopen (path, "r");

    while (!feof (fd)) {
	i =
	    fscanf (fd, "%d %f %f %f", &n, &det->knot_x[nuz],
		    &det->knot_y[nuz], &tmp);
	if (i == 4)
	    nuz++;
	else
	    fgets (s, 80, fd);
    }
    fclose (fd);

    det->nknot = nuz;
    return;
}

void drawelem_loadpal (char *path)
{
    FILE *fd;
    int r, g, b;
    int i;

    fd = fopen (path, "r");

    ncolor_pal = 0;

    if (color_pal != NULL)
	delete (color_pal);

    while (!feof (fd)) {
	fscanf (fd, "%x %x %x", &r, &g, &b);
	ncolor_pal++;
    }
    fclose (fd);
    color_pal = new GdkColor[ncolor_pal];

    fd = fopen (path, "r");
    for (i = 0; i < ncolor_pal; i++) {
	fscanf (fd, "%x %x %x", &r, &g, &b);
	color_pal[i].red = r;
	color_pal[i].green = g;
	color_pal[i].blue = b;
    }
    fclose (fd);
    return;
}

void load_elem (DrawElem * det, char *path)
{
    FILE *fd;
    int n, i;
    int nelem;
    char s[80];
    fd = fopen (path, "r");
    //fscanf (fd, "%d", &nelem);

    nelem = 0;
    while (!feof (fd)) {
	if (fscanf (fd, "%d %d %d %d", &n, &n, &n, &n) == 4)
	    nelem++;
	else
	    fgets (s, 80, fd);
    }
    fclose (fd);

    for (i = 0; i < 3; i++)
	if (det->elem[i] != NULL)
	    delete (det->elem[i]);

    for (i = 0; i < 3; i++)
	det->elem[i] = new int[nelem];

    nelem = 0;
    fd = fopen (path, "r");
    while (!feof (fd)) {
	if (fscanf
	    (fd, "%d %d %d %d", &n, &det->elem[0][nelem],
	     &det->elem[1][nelem], &det->elem[2][nelem]) == 4)
	    nelem++;
	else
	    fgets (s, 80, fd);
    }
    fclose (fd);

    det->nelem = nelem;
    return;
}

void drawelem_load_nds (DrawElem * det, char *path)
{
    FILE *fd;
    int n, i;
    int nelem;
    float tmp;
    char s[80];

    fd = fopen (path, "r");
    nelem = 0;
    while (!feof (fd)) {
	if (sdn == 3)
	    if (fscanf (fd, "%f", &tmp) == 1)
		nelem++;
	    else
		fgets (s, 80, fd);
	else if (fscanf (fd, "%d %f", &n, &tmp) == 2)
	    nelem++;
	else
	    fgets (s, 80, fd);
    }
    fclose (fd);

    if (det->nds != NULL)
	delete (det->nds);

    det->nds = new float[nelem];

    fd = fopen (path, "r");
    nelem = 0;
    while (!feof (fd)) {
	if (sdn == 3)
	    if (fscanf (fd, "%f", &det->nds[nelem]) == 1)
		nelem++;
	    else
		fgets (s, 80, fd);
	else if (fscanf (fd, "%d %f", &n, &det->nds[nelem]) == 2)
	    nelem++;
	else
	    fgets (s, 80, fd);
    }
    fclose (fd);

    // Нужно для интерполяции
    if (nds_knot != NULL)
	delete (nds_knot);
    nds_knot = new float[det->nknot];

    if (nds_knotn != NULL)
	delete (nds_knotn);
    nds_knotn = new int[det->nknot];
    for (i = 0; i < (det->nknot); i++) {
	nds_knotn[i] = 0;
	nds_knot[i] = 0;
    }

    for (i = 0; i < (det->nelem); i++)
	for (n = 0; n < 3; n++) {
	    nds_knot[det->elem[n][i]-1] =
		det->nds[i] + nds_knot[det->elem[n][i]-1];
	    nds_knotn[det->elem[n][i]-1]++;
	}

    for (i = 0; i < (det->nknot); i++)
	nds_knot[i] = nds_knot[i] / nds_knotn[i];

    return;
}

void drawelem_load_all (DrawElem * det, char *path)
{
    struct stat st;


    chdir (path);

    if (stat ("nod_os1.dat", &st) < 0) {
	g_print ("Файл nod_os1.dat не существует.\n");
	return;
    }
    if (stat ("koord.poc", &st) < 0) {
	g_print ("Файл koord.poc не существует.\n");
	return;
    }

    if (stat ("ntg.poc", &st) < 0) {
	g_print ("Файл ntg.poc не существует.\n");
	return;
    }

    load_elem (det, "nod_os1.dat");
    load_knot (det, "koord.poc");
    if (sdn == 1)
	drawelem_load_nds (det, "sg_na.poc");
    if (sdn == 2)
	drawelem_load_nds (det, "na_df.poc");
    if (sdn == 3)
	drawelem_load_nds (det, "ntg.poc");
    drawelem_fit (det);
}

void drawelem_save (DrawElem * det, char *path)
{
    GdkImlibImage *image;
    GdkBitmap mask;
    gint width, height;
    GdkImlibSaveInfo *info;

    gdk_window_get_size (DRAWELEM (det)->pixmap, &width, &height);
    image = gdk_imlib_create_image_from_drawable (DRAWELEM (det)->pixmap,
						  &mask, 0, 0, width,
						  height);
    gdk_imlib_save_image (image, path, info);

}

GtkType drawelem_get_type (void)
{
    static guint det_type = 0;
    if (!det_type) {
	GtkTypeInfo det_info = {
	    "DrawElem",
	    sizeof (DrawElem),
	    sizeof (DrawElemClass),
	    (GtkClassInitFunc) drawelem_class_init,
	    (GtkObjectInitFunc) drawelem_init,
	    (GtkArgSetFunc) NULL,
	    (GtkArgGetFunc) NULL
	};
	det_type =
	    gtk_type_unique (gtk_drawing_area_get_type (), &det_info);
    }

    return det_type;
}

static void drawelem_class_init (DrawElemClass * klass)
{
}

static void drawelem_init (DrawElem * det)
{
    int i;
    gtk_signal_connect (GTK_OBJECT (det), "expose_event",
			(GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT (det), "configure_event",
			(GtkSignalFunc) configure_event, NULL);
    for (i = 0; i < 3; i++)
	det->elem[i] = NULL;
    det->knot_x = NULL;
    det->knot_y = NULL;
    det->nds = NULL;
    det->mash = 5;
}

GtkWidget *drawelem_new (void)
{
    return GTK_WIDGET (gtk_type_new (drawelem_get_type ()));
}
