#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "pong.h"

static int window_width = 800;
static int window_height = 500;

static bool running = true;

static GError *error = NULL;
static GtkBuilder *builder = NULL;
static GObject *window = NULL;
static GtkWidget *drawing_area = NULL;

static void init_builder() {
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "src/builder.xml", &error)) {
        printf("Error loading builder file: %s\n", error->message);
        g_clear_error(&error);
    }
}

static void init_styling() {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path("src/style.css");
    if (gtk_css_provider_load_from_file(css_provider, css_file, &error)) {
        GtkStyleContext *style_context = gtk_widget_get_style_context(GTK_WIDGET(window));
        gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    } else {
        printf("Error loading CSS file: %s\n", error->message);
        g_clear_error(&error);
    }
}

static void init_render() {
    init_builder();
    window = gtk_builder_get_object(builder, "main_window");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    init_styling();
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    init_render();

    //while (running) {

    //}

    return 0;
}