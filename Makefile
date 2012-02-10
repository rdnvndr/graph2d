GTK_LIBS = -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule \
           -lglib -ldl -lXi -lXext -lX11 -lm -lgdk_imlib 
OBJECTS  = graph2d.o interface.o callback.o drawelem.o
CFLAGS   =-g -O -gstabs -I/usr/lib/glib/include -I/usr/X11R6/include
CC=g++ 
All: $(OBJECTS)	
	$(CC) -o graph2d  $(GTK_LIBS) $(OBJECTS)
clean:
	rm -f *.o graph2d