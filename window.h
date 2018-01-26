#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <webkit2/webkit2.h>

// window.h
int create_window(){

  // Create an 800x600 window that will contain the browser instance
  GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

  // create the tab manager
  GtkWidget *notebook = gtk_notebook_new();

  GtkWidget *label = gtk_label_new ("test");

  // create the gtk box that'll set the layout and put box in window
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);


  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), box, label);


  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(notebook));

  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(grid));

  // create url_bar and add to grid

  GtkWidget *back_button = gtk_button_new_with_label("Back");
  GtkWidget *forward_button = gtk_button_new_with_label("Forward");
  GtkWidget *stop_connection_button = gtk_button_new_with_label("Stop");

  GtkEntryBuffer *buf = gtk_entry_buffer_new("about:blank", 12);
  GtkWidget *url_bar = gtk_entry_new_with_buffer(buf);

  gtk_grid_attach(GTK_GRID(grid), back_button, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), forward_button, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), stop_connection_button, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), url_bar, 0, 1, 3, 1);

  // Create a browser instance and put in main window
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());


  //[]gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(webView));
  gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webView), TRUE, TRUE, 20);

   g_signal_connect(url_bar, "activate", G_CALLBACK(uriChangeCb), webView);
   g_signal_connect(back_button, "clicked", G_CALLBACK(backButtonCb), webView);
   g_signal_connect(forward_button, "clicked", G_CALLBACK(forwardButtonCb), webView);
   g_signal_connect(stop_connection_button, "clicked", G_CALLBACK(stopConnectionButtonCb), webView);

   g_signal_connect(webView, "navigation-policy-decision-requested", G_CALLBACK(uriUpdateCb), url_bar);

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), webView);

  // Load a web page into the browser instance
  webkit_web_view_load_uri(webView, "https://hackthe.tech/siletto");

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // Run the main GTK+ event loop
  gtk_main();

  return 0;
}
