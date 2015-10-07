/**
 * @file ppm.h
 * @brief Load/Write PPM file
 * @author Florent Gluck, Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
#ifndef PPM_H
#define PPM_H
#include <stdint.h>
#include <stdbool.h>

#include "image.h"

extern img_t *load_ppm(char *filename);
extern bool write_ppm(char *filename, img_t *img);
#endif
