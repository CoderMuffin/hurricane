// #define FAST_OBJ_IMPLEMENTATION
// #include "fast_obj.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <hurricane/geometry.h>

int hc_getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

const hc_geometry hc_geometry_none = {
    .vertices = NULL, .faces = NULL, .colors = NULL, .face_count = 0};


const hc_geometry hc_geometry_cube = {
    .vertices = (fixed[]){1,  -1, 1, 1,  -1, -1, 1,  1, -1, 1,  1, 1,
                           -1, -1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1},
    .faces =
        (unsigned int[]){4, 0, 3, 4, 3, 7, 0, 1, 2, 0, 2, 3, 1, 5, 6, 1, 6, 2,
                         5, 4, 7, 5, 7, 6, 7, 3, 2, 7, 2, 6, 0, 5, 1, 0, 4, 5},
    // .colors =
    //     (unsigned char[]){
    //         0,   0,   255, // face front
    //         0,   0,   255, //
    //         0,   255, 0,   // face top
    //         0,   255, 0,   //
    //         255, 0,   0,   // face right
    //         255, 0,   0,   //
    //         0,   255, 255, // face left
    //         0,   255, 255, //
    //         255, 255, 0,   // face back
    //         255, 255, 0,   //
    //         255, 0,   255, // face bottom
    //         255, 0,   255, //
    //     },
        (unsigned char[]){
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
            128, 128, 128,
        },
    .face_count = 12};