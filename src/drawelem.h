#ifndef G2D_DRAWELEM
#define G2D_DRAWELEM
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <gdk_imlib.h>
#include <string.h>
#define DRAWELEM(obj)          GTK_CHECK_CAST (obj, drawelem_get_type (), DrawElem)
#define DRAWELEM_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, drawelem_get_type (), DrawElemClass)
# define IS_DRAWELEM(obj)       GTK_CHECK_TYPE (obj, drawelem_get_type ())


typedef GdkGC* MyGdkGC;
typedef struct _DrawElem DrawElem;
typedef struct _DrawElemClass DrawElemClass;

struct _DrawElem {
    GtkDrawingArea drawingarea;
    GdkPixmap *pixmap;
    float *knot_x;
    float *knot_y;
    int *elem[3];
    float *nds;
    int nelem;
    int nknot;
    int zero_x, zero_y;
    int mash;
    char *knot_font;
    char *elem_font;
    GdkGC *knot_color;
    GdkGC *elem_color;
};

struct _DrawElemClass {
    GtkDrawingAreaClass parent_class;
};

GtkType drawelem_get_type (void);
GtkWidget *drawelem_new (void);

void drawelem_size (DrawElem * de, gint width, gint height);
void drawelem_show_elem (DrawElem * det);
void drawelem_center (DrawElem * det);
void drawelem_set_default_colormap(DrawElem * det);
void drawelem_fit (DrawElem * det);
void drawelem_load_all (DrawElem * det, char *path);
void drawelem_save (DrawElem * det, char *path);
void drawelem_loadpal (DrawElem * det, char *path);
void drawelem_set (int s);
int  drawelem_get ();
void drawelem_load_nds (DrawElem * det, char *path);
void drawelem_show_nelem (void);
void drawelem_show_nknot (void);
void draw_nknot (DrawElem * det);
void draw_nelem (DrawElem * det);
void drawelem_inter (void);
float drawelem_get_nds(DrawElem * det,int x,int y);

void drawelem_set_font_elem (DrawElem * det,char *fontname);
char* drawelem_get_font_elem (DrawElem * det);
void drawelem_set_font_knot (DrawElem * det,char *fontname);
char* drawelem_get_font_knot (DrawElem * det);

void drawelem_set_color_elem (DrawElem * det,GdkGC *color);
GdkGC* drawelem_get_color_elem (DrawElem * det);
void drawelem_set_color_knot (DrawElem * det,GdkGC *color);
GdkGC* drawelem_get_color_knot (DrawElem * det);

#endif
