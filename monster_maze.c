#include "hurricane/include/engine.h"
#include "hurricane/include/shared.h"
#include "hurricane/include/util/list.h"
#include "hurricane/include/renderer/console.h"
#include "hurricane/include/renderer/video.h"
// #include "hurricane/include/renderer/xlib.h"
// #include "hurricane/include/renderer/sdl.h"
#include "hurricane/include/loader/obj.h"
#include "hurricane/include/input.h"

#include "hurricane/include/anim.h"
#include "hurricane/include/util/log.h"
#include "hurricane/include/clock.h"
#include "maze.c"
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#if 1
#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100
#else
#define KEYW 25
#define KEYA 38
#define KEYS 39
#define KEYD 40
#endif

void noop() {}

hc_object camera;
hc_renderer renderer;

double tmpvec[3] = {0.02, 0.02, 0};
double tmpvecupdate[3];
char action;
hc_list blocks;

struct {
    int angle; // 0..=3
    int x;
    int y;
} player;

#define CAMERA_ANIMATION_FRAMES (10)
double camera_current_frame = CAMERA_ANIMATION_FRAMES - 1;
hc_quaternion camera_rotation_start;
hc_quaternion camera_rotation_end;
double camera_position_start[3];
double camera_position_end[3];

char format_tmp[100];
hc_quaternion tick;
bool *maze = 0;
int maze_w = 0;
int maze_h = 0;

void update() {
    camera_current_frame++;
    // double tmp[3];
    // hc_quaternion_to_euler_zyx(&camera.rotation, tmp);
    // hc_quaternion_fprint(stdout, &camera.rotation);
    // hc_log("%f %f %f", tmp[0], tmp[1], tmp[2]);
    if (camera_current_frame > CAMERA_ANIMATION_FRAMES) {
        action = getch();
        while (action == ' ') {
            dump_maze(maze, 19, 19, player.x, player.y);
            action = getch();
        }
        if (action == 'w' || action == 's') {
            int old_x = player.x;
            int old_y = player.y;
            player.x += (action == 'w' ? 1 : -1) * ((player.angle == 1 ? 1 : 0) - (player.angle == 3 ? 1 : 0));
            player.y += (action == 'w' ? 1 : -1) * ((player.angle == 0 ? 1 : 0) - (player.angle == 2 ? 1 : 0));
            if (maze[player.y * maze_w + player.x]) {
                player.x = old_x;
                player.y = old_y;
                return;
            }
            
            camera_current_frame = 0; // start animation if it was valid
            hc_vec3_copy(camera_position_end, camera_position_start);
            hc_vec3_set(player.x * 2, 0, player.y * 2, camera_position_end);
        }
        if (action == 'a' || action == 'd') {
            player.angle += 4 + (action == 'a' ? -1 : 1);
            player.angle %= 4;
            // hc_quaternion_copy(&camera_rotation_end, &camera_rotation_start);
            hc_quaternion_from_y_rotation(player.angle * M_PI / 2, &camera_rotation_end);
            hc_quaternion_copy(&camera_rotation_end, &camera.rotation);
        }
    } else {
        if (action == 'a' || action == 'd') {
            hc_quaternion_slerp(&camera_rotation_start, &camera_rotation_end, camera_current_frame / CAMERA_ANIMATION_FRAMES, &camera.rotation);
            Sleep(100);
        }
        if (action == 'w' || action == 's') {
            hc_vec3_lerp(camera_position_start, camera_position_end, camera_current_frame / CAMERA_ANIMATION_FRAMES, camera.position);
            Sleep(100);
        }
    }
    for (int i = 0; i < blocks.length; i++) {
        hc_object *obj = hc_list_get(&blocks, i);
        // hc_render_object(&camera, obj);
        if (
            (player.angle == 0 && obj->position[2] > camera.position[2] + 0.85) ||
            (player.angle == 1 && obj->position[0] > camera.position[0] + 0.85) ||
            (player.angle == 2 && obj->position[2] < camera.position[2] - 0.85) ||
            (player.angle == 3 && obj->position[0] < camera.position[0] - 0.85)) {
            hc_render_object(&camera, obj);
        }
    }
    sprintf(format_tmp, "\033[0mangle=%d x=%d y=%d px=%f py=%f pz=%f", player.angle, player.x, player.y, camera.position[0], camera.position[1], camera.position[2]);
    hc_console_blit(format_tmp, 20 - strlen(format_tmp) / 2, 30);
}

void create_maze(int width, int height) {
    maze_w = width * 2 + 3;
    maze_h = height * 2 + 3;
    maze = malloc(maze_w * maze_h * sizeof(bool));
    GenerateMaze(maze, maze_w, maze_h);
    for (int y = 0; y < maze_h; y++) {
        for (int x = 0; x < maze_w; x++) {
            if (maze[y * maze_w + x]) { // intentional y*w instead of y*h
                hc_object *cube = malloc(sizeof(hc_object));
                hc_object_new(cube, &hc_geometry_cube, VEC3(x*2, 0, y*2), hc_quaternion_identity, hc_vec3_one);
                hc_list_add(&blocks, cube);
            }
        }
    }
}

int main(int argc, char **argv) {
    hc_list_new(&blocks);
    create_maze(8, 8);
    renderer = hc_renderer_console;
    renderer.init();


    hc_set_fov(60, false);

    hc_object_new(&camera, &hc_geometry_none, VEC3(2, 0, 2), hc_quaternion_identity, hc_vec3_one);

    hc_quaternion_copy(&camera.rotation, &camera_rotation_start);
    hc_vec3_copy(camera.position, camera_position_start);
    hc_quaternion_from_y_rotation(M_PI / 2, &camera.rotation);
    player.angle = 1;
    player.x = 1;
    player.y = 1;
    
    hc_init(false, -1, renderer, update);

    renderer.finish();
    // hc_sdl_finish();
    // hc_video_finish();
    return 0;
}
