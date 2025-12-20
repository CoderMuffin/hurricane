#include <hurricane/engine.h>
#include <hurricane/renderer/SDL.h>
#include <hurricane/shared.h>
#include <hurricane/util/list.h>
#include <hurricane/util/vec.h>
#include <hurricane/util/quat.h>
#include <hurricane/renderer/console.h>
#include <hurricane/renderer/video.h>
// #include <hurricane/renderer/xlib.h>
#include <hurricane/clock.h>
#include <hurricane/loader/obj.h>
#include <hurricane/input.h>

#include <hurricane/anim.h>
#include <hurricane/util/log.h>
#include <hurricane/clock.h>
#include "maze.c"
#include <stdbool.h>

#define M_PI 3.1415926535897535

#if 1
#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100
#define KEYSPC 32
#else
#define KEYW 25
#define KEYA 38
#define KEYS 39
#define KEYD 40
#endif

void noop(void) {}

hc_object camera;
hc_renderer renderer;

hc_clock gclock;

fixed tmpvec[3] = {0.02, 0.02, 0};
fixed tmpvecupdate[3];
char action = '\0';
hc_list blocks;

struct {
    int angle; // 0..=3
    int x;
    int y;
    struct {
        int real_angle; // free
        bool animating;
        fixed progress;
        hc_quaternion rotation_start;
        hc_quaternion rotation_end;
        fixed position_start[3];
        fixed position_end[3];
    } camera;
} player;

const fixed CAMERA_ANIM_DURATION = 0.3;

char format_tmp[100];
hc_quaternion tick;
bool *maze = 0;
int maze_w = 0;
int maze_h = 0;

void on_key_down(void *e) {
    hc_input_key_event *evt = e;
    if (evt->code == KEYSPC) {
        hc_log("\nangle=%d x=%d y=%d\npx=%f py=%f pz=%f\nanim=%f action=%d animating=%d", player.angle, player.x, player.y, camera.position[0], camera.position[1], camera.position[2], player.camera.progress, action, player.camera.animating);
    }
            /*
        if (action == ' ') {
            dump_maze(maze, 19, 19, player.x, player.y);
        }
            */
    if (action == '\0') {
        switch (evt->code) {
            case KEYW:
                action = 'w';
                break;
            case KEYA:
                action = 'a';
                break;
            case KEYS:
                action = 's';
                break;
            case KEYD:
                action = 'd';
                break;
            default:
                hc_warn("unknown key code %d", evt->code);
                return;
        }
    }
}

void update(void) {
    fixed delta = hc_clock_step(&gclock);

    for (int i = 0; i < blocks.length; i++) {
        hc_object *obj = hc_list_get(&blocks, i);
        hc_render_object(&camera, obj);
    }

    if (!player.camera.animating) {
        if (action == 'w' || action == 's') {
            hc_vec3_set(player.x * 2, 0, player.y * 2, player.camera.position_start);

            int old_x = player.x;
            int old_y = player.y;

            player.x += (action == 'w' ? 1 : -1) * ((player.angle == 1 ? 1 : 0) - (player.angle == 3 ? 1 : 0));
            player.y += (action == 'w' ? 1 : -1) * ((player.angle == 0 ? 1 : 0) - (player.angle == 2 ? 1 : 0));
            if (maze[player.y * maze_w + player.x]) {
                player.x = old_x;
                player.y = old_y;
                action = '\0';
                return;
            }
            
            hc_vec3_set(player.x * 2, 0, player.y * 2, player.camera.position_end);

            player.camera.progress = 0;
            player.camera.animating = true;
        }
        if (action == 'a' || action == 'd') {
            hc_quaternion_from_y_rotation(player.camera.real_angle * M_PI/2, &player.camera.rotation_start);
            player.camera.real_angle += (action == 'a' ? -1 : 1);
            player.angle = (player.camera.real_angle % 4 + 4) % 4;
            hc_quaternion_from_y_rotation(player.camera.real_angle * M_PI/2, &player.camera.rotation_end);
            player.camera.progress = 0;
            player.camera.animating = true;
        }
    } else {
        player.camera.progress += delta;
        if (player.camera.progress > CAMERA_ANIM_DURATION) {
            player.camera.progress = CAMERA_ANIM_DURATION;
            player.camera.animating = false;
        }

        if (action == 'a' || action == 'd') {
            hc_quaternion_slerp(&player.camera.rotation_start, &player.camera.rotation_end, player.camera.progress / CAMERA_ANIM_DURATION, &camera.rotation);
        }
        if (action == 'w' || action == 's') {
            hc_vec3_lerp(player.camera.position_start, player.camera.position_end, player.camera.progress / CAMERA_ANIM_DURATION, camera.position);
        }

        if (!player.camera.animating) {
            action = '\0';
        }
    }
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
                hc_new_object(cube, &hc_geometry_cube, VEC3(x*2, 0, y*2), hc_quaternion_identity, hc_vec3_one);
                hc_list_add(&blocks, cube);
            }
        }
    }
}

int main(void) {
    hc_input_subscribe(on_key_down, HC_INPUT_KEYDOWN);
    hc_clock_new(&gclock);

    hc_list_new(&blocks);
    create_maze(8, 8);
    renderer = hc_renderer_sdl;
    renderer.init();

    hc_set_fov(90, false);

    hc_new_object(&camera, &hc_geometry_none, VEC3(2, 0, 2), hc_quaternion_identity, hc_vec3_one);

    hc_quaternion_copy(&camera.rotation, &player.camera.rotation_start);
    // hc_quaternion_from_euler_zyx(VEC3(M_PI/2, 0, 0), &camera.rotation);
    hc_vec3_copy(camera.position, player.camera.position_start);
    // hc_vec3_copy(VEC3(0, 100, 0), camera.position);
    hc_quaternion_from_y_rotation(M_PI / 2, &camera.rotation);
    player.angle = 1;
    player.x = 1;
    player.y = 1;
    player.camera.animating = false;
    player.camera.real_angle = 1;
    
    hc_init(false, -1, renderer, update);

    renderer.finish();
    // hc_sdl_finish();
    // hc_video_finish();
    return 0;
}
