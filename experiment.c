/*
 * Copyright (C) 2006, 2007 Apple Inc.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2011 Lukasz Slachciak
 * Copyright (C) 2011 Bob Murphy
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

int main(int argc, char* argv[])
{
    // Initialize GTK+
    gtk_init(&argc, &argv);

    // Create an 800x600 window that will contain the browser instance
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // create the gtk box that'll set the layout and put box in window
    GtkWidget *box = gtk_grid_new();
    
    gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(box));

    GtkWidget *web_view = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    //gtk_box_set_homogeneous(GTK_BOX(web_view), TRUE);

    // create url_bar and add to grid

    GtkWidget *back_button = gtk_button_new_with_label("Back");
    GtkWidget *forward_button = gtk_button_new_with_label("Forward");
    GtkWidget *stop_connection_button = gtk_button_new_with_label("Stop");
    GtkWidget *url_bar = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(box), back_button, 0, 0, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(box), forward_button, back_button, GTK_POS_RIGHT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(box), stop_connection_button, forward_button, GTK_POS_RIGHT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(box), url_bar, stop_connection_button, GTK_POS_RIGHT, 4, 1);
    //gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(web_view));

	//gtk_grid_set_row_homogeneous(GTK_GRID(box), TRUE);
    //gtk_grid_set_column_homogeneous(GTK_GRID(box), TRUE);

    // Create a browser instance and put in main window
    WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
	//gtk_grid_attach(GTK_GRID(box), GTK_WIDGET(webView), 1, 1, 1, 1);
    //gtk_box_set_homogeneous(GTK_BOX(web_view), TRUE);
    gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(webView));

    // Set up callbacks so that if either the main window or the browser instance is
    // closed, the program will exit
    g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

    // Load a web page into the browser instance
    webkit_web_view_load_uri(webView, "http://webkitgtk.org");

    // Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(webView));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(main_window);

    // Run the main GTK+ event loop
    gtk_main();

    return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
    gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
    // change this because program exits when tab is closed for now
    gtk_widget_destroy(window);
    return TRUE;
}
