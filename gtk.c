#include <gtk/gtk.h>
#include <pthread.h> 
#include "led-matrix-connector/led-matrix-connector/led-matrix-connector.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_id;

LedMonochromeMessage msg = {
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111
};

void myExit()
{
	pthread_cancel(thread_id);
	LedClear();
	printf("\n");
	return;
}

LedConfig Config = {
	.orientation = 0,
	.spiDevice = 1,
	.primaryColor = 'b',
	.delayTime = 1,
	.exitFunction = myExit
};

void end_program(GtkWidget *wid, gpointer ptr)
{
	gtk_main_quit();
}

void *display_thread(void *vargp)
{
	pthread_mutex_lock(&mutex);
	if ((int)vargp == 0)
		LedRenderText("TEST MSG", 300);
	if ((int)vargp == 1)
	{
		Config.primaryColor='r';
		LedSetConfig(Config);
		LedRenderMonochrome(msg, 1000);
	}
	if ((int)vargp == 2)
	{
		Config.primaryColor='g';
		LedSetConfig(Config);
		LedRenderMonochrome(msg, 1000);
	}
	if ((int)vargp == 3)
	{
		Config.primaryColor='b';
		LedSetConfig(Config);
		LedRenderMonochrome(msg, 1000);
	}
	pthread_mutex_unlock(&mutex);
}

void display(GtkWidget *wid, gpointer ptr)
{
	pthread_create(&thread_id, NULL, display_thread, 0);
	pthread_detach(thread_id);
}

void displayRed(GtkWidget *wid, gpointer ptr)
{
	pthread_create(&thread_id, NULL, display_thread, 1);
	pthread_detach(thread_id);
}

void displayGreen(GtkWidget *wid, gpointer ptr)
{
	pthread_create(&thread_id, NULL, display_thread, 2);
	pthread_detach(thread_id);
}

void displayBlue(GtkWidget *wid, gpointer ptr)
{
	pthread_create(&thread_id, NULL, display_thread, 3);
	pthread_detach(thread_id);
}

void main(int argc, char *argv[])
{
	LedInit(Config);
	
	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);
	
	GtkWidget *btn = gtk_button_new_with_label("Close window");
	g_signal_connect(btn, "clicked", G_CALLBACK(end_program), NULL);
	
	GtkWidget *lbl = gtk_label_new("test");
	
	GtkWidget *btn2 = gtk_button_new_with_label("SURPRISE"); 
	g_signal_connect(btn2, "clicked", G_CALLBACK(display), NULL);
	
	GtkWidget *btn3 = gtk_button_new_with_label("Test RED"); 
	g_signal_connect(btn3, "clicked", G_CALLBACK(displayRed), NULL);
	
	GtkWidget *btn4 = gtk_button_new_with_label("Test GREEN"); 
	g_signal_connect(btn4, "clicked", G_CALLBACK(displayGreen), NULL);
	
	GtkWidget *btn5 = gtk_button_new_with_label("Test BLUE"); 
	g_signal_connect(btn5, "clicked", G_CALLBACK(displayBlue), NULL);
	
	GtkWidget *tbl = gtk_table_new(5, 2, TRUE);
	gtk_table_attach_defaults(GTK_TABLE (tbl), lbl, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE (tbl), btn2, 1, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE (tbl), btn3, 1, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE (tbl), btn4, 1, 2, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE (tbl), btn5, 1, 2, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE (tbl), btn, 0, 2, 4, 5);
	gtk_container_add(GTK_CONTAINER (win), tbl);
	
	gtk_widget_show_all(win);
	gtk_main();
	
	LedFinalise();
}
