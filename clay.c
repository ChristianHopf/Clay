#include <gtk/gtk.h>
#include <glib/gstdio.h>

#include "client.h"

int sockfd = -1;

static void on_connect(GtkButton *button, gpointer user_data){
  GtkEntry *ip_entry = GTK_ENTRY(user_data);
  const char *ip = gtk_editable_get_text(GTK_EDITABLE(ip_entry));
  g_print("Connecting\n");
  sockfd = connect_to_server(ip);
  printf("Connected to %s\n", ip);
}

static void on_send(GtkButton *button, gpointer user_data){
  GtkEntry *command_entry = GTK_ENTRY(user_data);
  const char *command = gtk_editable_get_text(GTK_EDITABLE(command_entry));
  g_print("Sending command: %s\n", command);
}

static void activate(GtkApplication *app, gpointer user_data){
  GtkBuilder *builder = gtk_builder_new();
  GError *error = NULL;
  if (gtk_builder_add_from_file(builder, "layout.ui", &error) == 0){
    g_printerr("Error loading layout file: %s\n", error->message);
    g_clear_error(&error);
    g_object_unref(builder);
    return;
  }
 
  // Get main window
  GObject *window = gtk_builder_get_object(builder, "main_window");
  if (!window){
    g_printerr("Error: failed to find main window\n");
    g_object_unref(builder);
    return;
  }
  gtk_application_add_window(app, GTK_WINDOW(window));

  // Get widgets
  GtkWidget *ip_entry = GTK_WIDGET(gtk_builder_get_object(builder, "ip_entry"));
  GtkWidget *connect_button = GTK_WIDGET(gtk_builder_get_object(builder, "connect_button"));
  GtkWidget *messages_view = GTK_WIDGET(gtk_builder_get_object(builder, "messages_view"));
  GtkWidget *command_entry = GTK_WIDGET(gtk_builder_get_object(builder, "command_entry"));
  GtkWidget *send_button = GTK_WIDGET(gtk_builder_get_object(builder, "send_button"));

  // Connect callbacks to button widgets
  g_signal_connect(connect_button, "clicked", G_CALLBACK(on_connect), ip_entry);
  g_signal_connect(send_button, "clicked", G_CALLBACK(on_send), command_entry);

  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
  g_object_unref(builder);
}

int main(int argc, char *argv[]){
  #ifdef GTK_SRCDIR
    g_chdir(GTK_SRCDIR);
  #endif

  GtkApplication *app = gtk_application_new("com.github.ChristianHopf.Clay", G_APPLICATION_DEFAULT_FLAGS);
  int status;

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
