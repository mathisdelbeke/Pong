#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <math.h>
#include "pong.h"

static const int FIELD_WIDTH = 800;
static const int FIELD_HEIGHT = 500;
static const float MAX_BOUNCE_ANGLE = (75 * (G_PI / 180));
static const int GOAL_WIDTH = 10;

static int width_user = 10;
static int width_enemy = 10;
static int height_user = 60;
static int height_enemy = 60;

static int ball_radius = 10;
static int enemy_speed = 20;

static bool enemy_catch_up = true;

static Player user;
static Player enemy;
static Ball ball;

static GtkWidget *window = NULL;
static GtkWidget *drawing_area = NULL;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    make_players();
    make_ball();
    first_render();
    gtk_main();
    return 0;
}

static void make_players() {
    int x_user = 0 + GOAL_WIDTH;
    int x_enemy = FIELD_WIDTH - width_enemy - GOAL_WIDTH;
    int y_user = (FIELD_HEIGHT / 2) - (height_user / 2);
    int y_enemy = (FIELD_HEIGHT / 2) - (height_enemy / 2);

    init_player_struct(&user, 0, x_user, y_user, width_user, height_user);
    init_player_struct(&enemy, 1, x_enemy, y_enemy, width_enemy, height_enemy);
}

static void init_player_struct(Player *p, int num, int x, int y, int width, int height) {  
    p->number = num;
    p->pos.x = x;
    p->pos.y = y;
    p->size.width = width;
    p->size.height = height;
}

static void make_ball() {
    ball.pos.x = (FIELD_WIDTH / 2) - (ball_radius / 2);
    ball.pos.y = (FIELD_HEIGHT / 2) - (ball_radius / 2);
    ball.radius = ball_radius;
    ball.dx = -10;      
    ball.dy = 0;
    ball.speed = 10;     
}

static void first_render() {
    init_window();
    init_drawing_area();
    init_mouse_input();
    gtk_widget_show_all(GTK_WIDGET(window));
    g_timeout_add_full(G_PRIORITY_HIGH, 16, update_game, NULL, NULL);
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
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), &user);
}

static void init_mouse_input() {
    g_signal_connect(window, "motion-notify-event", G_CALLBACK(on_mouse_move_event), NULL);
    gtk_widget_add_events(window, GDK_POINTER_MOTION_MASK);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); 
    cairo_rectangle(cr, 0, 0, FIELD_WIDTH, FIELD_HEIGHT);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_arc(cr, ball.pos.x, ball.pos.y, ball.radius, 0, 2 * G_PI);
    cairo_fill(cr);
    
    Player *player = (Player *)user_data;
    cairo_rectangle(cr, player->pos.x, player->pos.y, player->size.width, player->size.height);
    cairo_fill(cr);
    cairo_rectangle(cr, enemy.pos.x, enemy.pos.y, enemy.size.width, enemy.size.height);
    cairo_fill(cr);

    return FALSE;
}

static gboolean on_mouse_move_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data) {
    if (event->y >= 0 && event->y <= FIELD_HEIGHT - user.size.height) {
        user.pos.y = (int)event->y;
    }
    return FALSE;
}

static gboolean update_game(gpointer user_data) {
    update_ball_pos();
    check_wall_collision();
    check_goal();
    check_player_collision(&user);
    check_player_collision(&enemy);
    update_enemy();
    
    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

static void update_ball_pos() {
    ball.pos.x += ball.dx;
    ball.pos.y += ball.dy;
}

static void check_wall_collision() {
    if (ball.pos.y - ball.radius <= 0 || ball.pos.y + ball.radius >= FIELD_HEIGHT) {
        ball.dy = -ball.dy;
    }
}

static void check_goal() {
    if (ball.pos.x - (ball.radius / 2) <= 0 || ball.pos.x + (ball.radius / 2) >= FIELD_WIDTH) {
        ball.pos.x = (FIELD_WIDTH / 2) - (ball_radius / 2);
        ball.pos.y = (FIELD_HEIGHT / 2) - (ball_radius / 2);
        ball.radius = ball_radius;
        ball.dx = -10;
        ball.dy = 0;
        ball.speed = 10;  
    }
}

static void check_player_collision(Player *player) {
    if (ball.pos.x - ball.radius <= player->pos.x + player->size.width && ball.pos.x + ball.radius >= player->pos.x) {
        if (ball.pos.y >= player->pos.y && ball.pos.y <= player->pos.y + player->size.height) {
            float hit_position = (ball.pos.y - player->pos.y) / (float)player->size.height;
            float bounce_angle = (hit_position - 0.5) * MAX_BOUNCE_ANGLE;

            ball.dx = ball.speed * cos(bounce_angle) * (ball.dx < 0 ? 1 : -1);  
            ball.dy = ball.speed * sin(bounce_angle);

            if (player->number == 1) {
                enemy_catch_up = !enemy_catch_up;
            }
        }
    }
}

static void update_enemy() {
    int catch_offset = enemy_speed;
    if (enemy_catch_up) catch_offset = -enemy_speed; 
    int target_y = predict_ball_y() - (enemy.size.height / 2) - catch_offset;

    if (enemy.pos.y < target_y && enemy.pos.y <= (FIELD_HEIGHT - enemy.size.height - enemy_speed) ) {
        enemy.pos.y += enemy_speed;
        if (enemy.pos.y > target_y) enemy.pos.y = target_y;
    } else if (enemy.pos.y > target_y && enemy.pos.y + enemy_speed > 0) {
        enemy.pos.y -= enemy_speed;
        if (enemy.pos.y < target_y) enemy.pos.y = target_y;
    }
}

static int predict_ball_y() {
    float time_to_enemy = (enemy.pos.x - ball.pos.x) / ball.dx;
    float predicted_y = ball.pos.y + time_to_enemy * ball.dy;

    while (predicted_y < 0 || predicted_y > FIELD_HEIGHT) {
        if (predicted_y < 0) predicted_y = -predicted_y;
        if (predicted_y > FIELD_HEIGHT) predicted_y = 2 * FIELD_HEIGHT - predicted_y;
    }

    return (int)predicted_y;
}