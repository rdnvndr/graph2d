GTK_LIBS = -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule \
           -lglib -ldl -lXi -lXext -lX11 -lm -lgdk_imlib 
OBJECTS  = graph2d.o interface.o callback.o drawelem.o
CFLAGS   =-g -I/usr/lib/glib/include -I/usr/X11R6/include
CC=g++ -g
All: 	
	g++ -g   -c -o graph2d.o graph2d.C
	g++ -g   -c -o interface.o interface.C
	g++ -g   -c -o callback.o callback.C
	g++ -g   -c -o drawelem.o drawelem.C
	g++ -g -o graph2d  -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm -lgdk_imlib  graph2d.o interface.o callback.o drawelem.o
	
clean:
	rm -f *.o graph2d

install:
	cp graph2d /usr/local/bin
