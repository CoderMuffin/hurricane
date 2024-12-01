#ifndef HC_ANIM
#define HC_ANIM
#include "include/util/quat.h"
#include "include/util/vec.h"
#include "include/anim.h"
#include <stdbool.h>

void hc_animator_vec3(void *from, void *to, double t, int frame, void *out) {
    hc_vec3_lerp(from, to, t, out);
}
void hc_animator_quaternion(void *from, void *to, double t, int frame, void *out) {
    hc_quaternion_slerp(from, to, t, out);
}

// void hc_animator_quaternion(hc_keyframe *from, hc_keyframe *to, void *out,
//                       double time) {
//   double end = to->time - from->time;
//   time -= from->time;
//   hc_quaternion_lerp(from->state, to->state, time / end, out);
// }

void hc_anim_step(hc_anim *anim, double delta, void *out) {
    if (!anim->playing)
        return;
    anim->time += delta;
    if (anim->frames[(anim->curr_frame + 1) % anim->frame_count].time <= anim->time) {
        anim->curr_frame++;
        if (anim->curr_frame >= anim->frame_count - 1) {
            if (anim->looping) {
                // printf("reset %d\n",anim->frame_count);
                anim->time = 0;
                anim->curr_frame = 0;
            } else {
                anim->playing = false;
                // printf("finish %d\n",anim->frame_count);
                anim->animator(anim->frames[anim->frame_count - 2].state,
                               anim->frames[anim->frame_count - 1].state, 1,
                               anim->frame_count - 1, out);
                return;
            }
        }
    }
    hc_keyframe *from = anim->frames + anim->curr_frame;
    // printf("frame %d time %f\n", anim->curr_frame, anim->time);
    anim->animator(from->state, (from + 1)->state,
                   (anim->time - from->time) / ((from + 1)->time - from->time),
                   anim->curr_frame, out);
}

void hc_anim_play(hc_anim *anim) {
    anim->time = 0;
    anim->curr_frame = 0;
    anim->playing = true;
}

void hc_anim_new(hc_anim *output, hc_keyframe frames[], int frame_count, void (*animator)(void *, void *, double, int, void *)) {
    output->animator = animator;
    output->frame_count = frame_count;
    output->frames = frames;
    output->time = 0;
    output->looping = false;
    output->playing = true;
}

#endif
