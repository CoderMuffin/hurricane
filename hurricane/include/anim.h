#ifndef HC_ANIM_H
#define HC_ANIM_H

#include "util/quat.h"
#include "util/vec.h"
#include <stdbool.h>

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
    void (*animator)(void *, void *, double, int, void *);
} hc_anim;

void hc_animator_vec3(void *from, void *to, double t, int frame, void *out);
void hc_anim_step(hc_anim *anim, double delta, void *out);
void hc_anim_play(hc_anim *anim);

#endif
