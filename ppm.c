/**
 * @file ppm.c
 * @brief Load/Write PPM file
 * @author Florent Gluck, Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
// This is ppm.c, a little bit modified (file provided by the professor)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h"

// Write a 24-bit RGB PPM file.
// Return false if the image wasn't written properly.
// If filename is "-", use stdout
bool write_ppm(char *filename, img_t *img) {
    bool use_stdout = strcmp(filename, "-") == 0;
    FILE *f = use_stdout ? stdout : fopen(filename, "w");
    if (f == NULL)
        return false;
    // Write the header
    fprintf(f, "P3\n%d %d\n255\n", img->width, img->height);
    // Write image content
    int count = 0;
    for (int i = 0; i < img->width * img->height; i++) {
        pixel_t *p = &img->data[i];
        fprintf(f, "%d %d %d ", p->r, p->g, p->b);
        if (++count % 5 ==
            0) // New line every 5 pixels (max 70 characters/line)
            fprintf(f, "\n");
    }
    if (!use_stdout) {
        fclose(f);
    }
    return true;
}

// Load a 24-bit RGB PPM file and return the loaded image.
// The function takes care of allocating the memory for the image.
// If filename is "-", use stdin
img_t *load_ppm(char *filename) {
    bool use_stdin = strcmp(filename, "-") == 0;
    FILE *f = use_stdin ? stdin : fopen(filename, "r");
    if (f == NULL)
        return NULL;

    char line[128]; // The PPM format states a line is at most 70 character long
    unsigned int width, height;
    unsigned int maxval;

    // Reads the PPM header, making sure it's valid
    int matches;
    // Fist line should be P3
    matches = fscanf(f, "%2s", line); // read 2 characters
    if (matches != 1 || strcmp(line, "P3") != 0)
        goto error;
    // Second line should be image width and height
    matches = fscanf(f, "%u %u", &width, &height);
    if (matches != 2)
        goto error;
    // Third line should be the maximum value per component
    matches = fscanf(f, "%u", &maxval);
    if (matches != 1)
        goto error;

    // Allocate memory for image structure and image data
    img_t *img = alloc_img(width, height);

    // Next, should be the image data in RGB order
    for (unsigned int i = 0; i < width * height; i++) {
        unsigned r, g, b;
        matches = fscanf(f, "%u %u %u", &r, &g, &b);
        if (matches != 3)
            goto error;
        if (r > maxval || g > maxval || b > maxval)
            goto error;
        pixel_t p = {r, g, b};
        img->data[i] = p;
    }
    if (!use_stdin) {
        fclose(f);
    }
    return img;

error:
    if (!use_stdin) {
        fclose(f);
    }
    return NULL;
}
