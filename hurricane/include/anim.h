#ifndef HC_ANIM_H
#define HC_ANIM_H

// macros
#include "util/quat.h"
#include "util/vec.h"
#include <stdbool.h>

// types
typedef void (*hc_animator)(void *, void *, double, int, void *);
typedef struct hc_keyframe {
    void *state;
    double time;
} hc_keyframe;
typedef struct hc_anim {
    hc_keyframe *frames;
    int frame_count;
    bool looping;
    double time;
    int curr_frame;
    bool playing;
    hc_animator animator;
} hc_anim;

// decls

// functions
void hc_animator_vec3(void *from, void *to, double t, int frame, void *out);
void hc_animator_quaternion(void *from, void *to, double t, int frame, void *out);
void hc_anim_step(hc_anim *anim, double delta, void *out);
void hc_anim_play(hc_anim *anim);
void hc_anim_new(hc_anim *output, hc_keyframe frames[], int frame_count, hc_animator animator);

#endif
