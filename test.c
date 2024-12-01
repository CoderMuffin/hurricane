#include "hurricane/include/engine.h"
#include "hurricane/include/shared.h"
#include "hurricane/include/util/list.h"
#include "hurricane/include/renderer/console.h"
#include "hurricane/include/renderer/video.h"
// #include "hurricane/include/renderer/xlib.h"
#include "hurricane/include/loader/obj.h"
#include "hurricane/include/renderer/SDL.h"
#include "hurricane/include/input.h"

#include "hurricane/include/anim.h"
#include "hurricane/include/util/log.h"
#include "hurricane/include/clock.h"
#include <stdio.h>

hc_renderer renderer;
__declspec(dllimport) void exported();
void update(double delta) {
}
int main() {
    printf("Hello world!\n");
    // hc_sdl_finish();
    exported();
    // hc_video_finish();
    return 0;
}
