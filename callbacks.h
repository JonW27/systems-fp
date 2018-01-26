static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);
static void uriChangeCb(GtkEntry* entry, gpointer user_data);
static void backButtonCb(GtkButton* button, gpointer user_data);
static void forwardButtonCb(GtkButton* button, gpointer user_data);
static void tabRemoveCb(GtkButton* button, gpointer user_data);
static void tabAddCb(GtkButton* button, gpointer user_data);
int create_window(GtkWidget *notebook, sem_t s);
