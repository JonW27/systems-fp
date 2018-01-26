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
#include <unistd.h>

#include "callbacks.h"
#include "window.h"

int main(int argc, char* argv[])
{
  // Initialize GTK+
  gtk_init(&argc, &argv);

  int child = fork();
  if(child == 0){
      create_window(); // spawn off separate process for each window
  }
  //create_menu();
}


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
  else{}
}

static void forwardButtonCb(GtkButton* button, gpointer user_data){
	if(webkit_web_view_can_go_forward(user_data)){
		webkit_web_view_go_forward(user_data);
	}
}

static void stopConnectionButtonCb(GtkButton* button, gpointer user_data){
	if(webkit_web_view_is_loading(user_data)){
		webkit_web_view_stop_loading(user_data);
	}
}

// this function not only handles uri updating of the uri bar, but handles downloading
// files it obviously is not good at reading
static void uriUpdateCb(WebKitWebView* web_view, WebKitWebFrame* frame, WebkitNetworkRequest* request, WebkitWebNavigationAction* navigation_action, WebkitWebPolicyDecision* policy_decision, gpointer user_data){
  const gchar* uri = webkit_network_request_get_uri(request);
  char* file_type = strchr(uri, '.');
  if(file_type && (!strcmp(file_type, ".pdf") || !strcmp(file_type, ".db") || !strcmp(file_type, ".exe") || !strcmp(file_type, ".deb") || !strcmp(file_type, ".rpm") || !strcmp(file_type, ".dmg"))){
      // note to self: replace above if statement with a loop and a file that contains these extensions
      webkit_web_policy_decision_download(policy_decision);
  }
  else{
    webkit_web_policy_decision_use(policy_decision);
    gtk_entry_set_text(user_data, uri);
  }
}
