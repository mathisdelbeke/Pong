#ifndef PONG_H
#define PONG_H

#include <gtk/gtk.h>

typedef struct {
    int x; 
    int y;
} Position;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    int number;
    Position pos;
    Size size;    
} Player;

typedef struct {
    Position pos;
    int dx, dy;
    int speed;
    int radius;
} Ball;

static void make_players();
static void init_player_struct(Player *p, int num, int x, int y, int width, int height);
static void make_ball();
static void first_render();
static void init_window();
static void init_drawing_area();
static void init_key_presses();
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static gboolean update_game(gpointer user_data);
static void update_ball_pos();
static void check_wall_collision();
static void check_goal();
static void check_player_collision(Player *player);
static void update_enemy();
static int predict_ball_y();

#endif