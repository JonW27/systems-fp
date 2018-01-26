#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <webkit2/webkit2.h>
#include <semaphore.h>
#include <sys/sem.h>
#define KEY 12345
#include "callbacks.h"

// window.h

sem_t sem;

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
  gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
  // change this because program exits when tab is closed for now
  //gtk_widget_destroy(window);
  return TRUE;
}

static void uriChangeCb(GtkEntry* entry, gpointer user_data){
	const gchar *entry_text;
	entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	if(strcmp(entry_text, "about:blank") == 0){
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(user_data), "https://hackthe.tech/siletto");
	}
	/*else if(strncmp(entry_text, "file://", 7) == 0){
		// do the file shit here
	}*/
	else if(strncmp(entry_text, "localhost:", 10) == 0){
		char txt[20];
		sprintf(txt, "127.0.0.1:%s", (entry_text + 10));
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(user_data), entry_text);
	}
	else{
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(user_data), entry_text);
	}
}

static void backButtonCb(GtkButton* button, gpointer user_data){
  if(webkit_web_view_can_go_back(user_data)){
    webkit_web_view_go_back(user_data);
  }
}


static void tabRemoveCb(GtkButton* button, gpointer user_data){
  if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(user_data)) == 1){
    gtk_main_quit();
  }
  else{
    gtk_notebook_detach_tab (GTK_NOTEBOOK(user_data), gtk_widget_get_ancestor (button, GtkBox));
    sem_post(&sem);
  }
}


static void tabAddCb(GtkButton* button, gpointer user_data){
  sem_wait(&sem);
  create_window(user_data);
}

static void forwardButtonCb(GtkButton* button, gpointer user_data){
	if(webkit_web_view_can_go_forward(user_data)){
		webkit_web_view_go_forward(user_data);
	}
}



int create_window(GtkWidget *notebook, sem_t s){
  sem = s;
  GtkWidget *label = gtk_label_new ("test");

  // create the gtk box that'll set the layout and put box in window
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);


  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), box, label);
  gtk_notebook_next_page (GTK_NOTEBOOK(notebook));
 

  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(grid));

  // create url_bar and add to grid

  GtkWidget *back_button = gtk_button_new_with_label("Back");
  GtkWidget *forward_button = gtk_button_new_with_label("Forward");
  GtkWidget *stop_connection_button = gtk_button_new_with_label("Stop");
  GtkWidget *remove_tab_button = gtk_button_new_with_label("Close tab");
  GtkWidget *add_tab_button = gtk_button_new_with_label("New tab");

  GtkEntryBuffer *buf = gtk_entry_buffer_new("about:blank", 12);
  GtkWidget *url_bar = gtk_entry_new_with_buffer(buf);

  gtk_grid_attach(GTK_GRID(grid), back_button, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), forward_button, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), stop_connection_button, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), url_bar, 0, 1, 3, 1);
  gtk_grid_attach(GTK_GRID(grid), remove_tab_button, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), add_tab_button, 1, 2, 1, 1);

  // Create a browser instance and put in main window
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

  //[]gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(webView));
  gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webView), TRUE, TRUE, 20);

   g_signal_connect(url_bar, "activate", G_CALLBACK(uriChangeCb), webView);
   g_signal_connect(back_button, "clicked", G_CALLBACK(backButtonCb), webView);
   g_signal_connect(forward_button, "clicked", G_CALLBACK(forwardButtonCb), webView);
   g_signal_connect(remove_tab_button, "clicked", G_CALLBACK(tabRemoveCb), notebook);
   g_signal_connect(add_tab_button, "clicked", G_CALLBACK(tabAddCb), notebook);
   
   
  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit

  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), webView);

  // Load a web page into the browser instance
  webkit_web_view_load_uri(webView, "https://hackthe.tech/siletto");

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

 
  return 0;
}
