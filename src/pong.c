#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "pong.h"

static int FIELD_WIDTH = 800;
static int FIELD_HEIGHT = 500;

static int WIDTH_USER = 10;
static int WIDTH_ENEMY = 10;
static int HEIGHT_USER = 60;
static int HEIGHT_ENEMY = 60;

static bool running = true;
static Player user;
static Player enemy;

static GtkWidget *window = NULL;
static GtkWidget *drawing_area = NULL;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    cairo_set_source_rgb(cr, 0.0, 0.0, 1.0); 
    cairo_rectangle(cr, user.pos.x, user.pos.y, user.size.width, user.size.height);
    cairo_fill(cr);
    return FALSE;
}

static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Up) {
        user.pos.y -= 10;  
    } else if (event->keyval == GDK_KEY_Down) {
        user.pos.y += 10;  
    } else if (event->keyval == GDK_KEY_Left) {
        user.pos.x -= 10;  
    } else if (event->keyval == GDK_KEY_Right) {
        user.pos.x += 10;  
    }
    
    gtk_widget_queue_draw(widget);
    return FALSE;
}

static void init_window() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Pong");
    gtk_window_set_default_size(GTK_WINDOW(window), FIELD_WIDTH, FIELD_HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

static void init_drawing_area() {
    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);
}

static void init_key_presses() {
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press_event), NULL);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
}

static void first_render() {
    init_window();
    init_drawing_area();
    init_key_presses();
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}

static void make_player(Player *p, int num, int x, int y, int width, int height) {  
    p->number = num;
    p->pos.x = x;
    p->pos.y = y;
    p->size.width = width;
    p->size.height = height;
}

static void calculate_pos_players() {
    int x_user = 0;
    int x_enemy = FIELD_WIDTH - WIDTH_ENEMY;
    int y_user = (FIELD_HEIGHT / 2) - (HEIGHT_USER / 2);
    int y_enemy = (FIELD_HEIGHT / 2) - (HEIGHT_ENEMY / 2);

    make_player(&user, 0, x_user, y_user, WIDTH_USER, HEIGHT_USER);
    make_player(&enemy, 1, x_enemy, y_enemy, WIDTH_ENEMY, HEIGHT_ENEMY);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    calculate_pos_players();
    first_render();

    //while (running) {

    //}

    return 0;
}