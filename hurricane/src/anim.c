#include <hurricane/util/quat.h>
#include <hurricane/util/vec.h>
#include <hurricane/anim.h>
#include <hurricane/fixed.h>
#include <stdbool.h>

void hc_animator_vec3(void *from, void *to, fixed t, int frame, void *out) {
    hc_vec3_lerp(from, to, t, out);
}
void hc_animator_quaternion(void *from, void *to, fixed t, int frame,
                            void *out) {
    hc_quaternion_slerp(from, to, t, out);
}

void hc_anim_step(hc_anim *anim, fixed delta, void *out) {
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
                   fd(anim->time - from->time, (from + 1)->time - from->time),
                   anim->curr_frame, out);
}

void hc_anim_play(hc_anim *anim) {
    anim->time = 0;
    anim->curr_frame = 0;
    anim->playing = true;
}

void hc_anim_new(hc_anim *output, hc_keyframe frames[], int frame_count,
                 hc_animator *animator) {
    output->animator = animator;
    output->frame_count = frame_count;
    output->frames = frames;
    output->time = 0;
    output->looping = false;
    output->playing = true;
}
