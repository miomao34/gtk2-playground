all:
	gcc `pkg-config --cflags --libs gtk+-2.0` led-matrix-connector/led-matrix-connector/led-matrix-connector.c gtk.c -o gtk.out -lwiringPi -lpthread
