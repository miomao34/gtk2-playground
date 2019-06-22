#include <gtk/gtk.h>
#include <pthread.h> 
#include "led-matrix-connector/led-matrix-connector/led-matrix-connector.h"

pthread_t thread_id;

void myExit()
{
	pthread_cancel(thread_id);
	LedClear();
	printf("\n");
	return;
}

void end_program (GtkWidget *wid, gpointer ptr)
{
	gtk_main_quit ();
}

void *display_thread(void *vargp)
{
	LedRenderText("TEST MSG", 300);
}

void display (GtkWidget *wid, gpointer ptr)
{
	pthread_create(&thread_id, NULL, display_thread, NULL);
	pthread_detach(thread_id);
}

void main (int argc, char *argv[])
{
	LedConfig Config = {
		.orientation = 0,
		.spiDevice = 1,
		.primaryColor = 'b',
		.delayTime = 1,
		.exitFunction = myExit
	};
	LedInit(Config);
	
	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(win, "delete_event", G_CALLBACK (end_program), NULL);
	
	GtkWidget *box = gtk_vbox_new(FALSE, 0);
	
	GtkWidget *btn = gtk_button_new_with_label ("Close window");
	g_signal_connect (btn, "clicked", G_CALLBACK (end_program), NULL);
	gtk_box_pack_start (GTK_BOX (box), btn, TRUE, TRUE, 0);
	
	GtkWidget *lbl = gtk_label_new("test");
	gtk_box_pack_start (GTK_BOX (box), lbl, FALSE, TRUE, 0);
	
	GtkWidget *btn2 = gtk_button_new_with_label ("SURPRISE"); 
	g_signal_connect (btn2, "clicked", G_CALLBACK (display), NULL); 
	gtk_box_pack_start (GTK_BOX (box), btn2, TRUE, TRUE, 0);
	
	gtk_container_add (GTK_CONTAINER (win), box); 
	gtk_widget_show_all(win);
	gtk_main();
	
	LedFinalise();
}
