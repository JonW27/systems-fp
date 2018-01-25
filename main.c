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
#include <string.h>
#include <stdio.h>
#include <webkit2/webkit2.h>


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);
static void uriChangeCb(GtkEntry* entry, gpointer user_data);
static void backButtonCb(GtkButton* button, gpointer user_data);
static void forwardButtonCb(GtkButton* button, gpointer user_data);

int main(int argc, char* argv[])
{
    // Initialize GTK+
    gtk_init(&argc, &argv);

    // Create an 800x600 window that will contain the browser instance
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);


    GtkWidget *notebook = gtk_notebook_new();

    GtkWidget *label = gtk_label_new ("test");
    
    // create the gtk box that'll set the layout and put box in window
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);


    gtk_notebook_append_page(GTK_NOTEBOOK (notebook), box, label);

    //[]gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
    
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
	gtk_grid_attach(GTK_GRID(grid), url_bar, 0, 1, 4, 1);

    /*gtk_box_pack_start(GTK_BOX(box), back_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), forward_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), stop_connection_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), url_bar, FALSE, FALSE, 0);*/

	//GtkWidget *web_view = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    //gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(web_view));

    // Create a browser instance and put in main window
    WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    
    //[]gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(webView));
    gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webView), TRUE, TRUE, 20);
    
	g_signal_connect(url_bar, "activate", G_CALLBACK(uriChangeCb), webView);
	g_signal_connect(back_button, "clicked", G_CALLBACK(backButtonCb), webView);
	g_signal_connect(forward_button, "clicked", G_CALLBACK(forwardButtonCb), webView);

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

static void uriChangeCb(GtkEntry* entry, gpointer user_data){
	const gchar *entry_text;
	entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	if(strcmp(entry_text, "about:blank") == 0){
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(user_data), "https://hackthe.tech/siletto");
	}
	else if(strncmp(entry_text, "file://", 7) == 0){
		// do the file shit here
	}
	else if(strncmp(entry_text, "localhost:", 10) == 0){
		char txt[20];
		sprintf(txt, "127.0.0.1:%s", (entry_text + 10)); 
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(user_data), entry_text);
	}
	else{
		printf("this ran");
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(user_data), entry_text);
	}
}

static void backButtonCb(GtkButton* button, gpointer user_data){
	if(webkit_web_view_can_go_back(user_data)){
		webkit_web_view_go_back(user_data);
	}
}

static void forwardButtonCb(GtkButton* button, gpointer user_data){
	if(webkit_web_view_can_go_forward(user_data)){
		webkit_web_view_go_forward(user_data);
	}
}

		
	
