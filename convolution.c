#include <stdlib.h>
#include <stdio.h>

#include "convolution.h"

float get_kernel_value(kernel_t kernel, int i, int j) {
    int x = i + kernel.half_n;
    int y = j + kernel.half_n;
    int width = kernel.half_n * 2 + 1;
    return kernel.values[y * width + x];
}

bool load_kernel(char *filename, kernel_t *kernel) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return false;
    }
    fscanf(f, "%d", &kernel->half_n);
    int size = (kernel->half_n * 2 + 1);
    size *= size;
    kernel->values = malloc(sizeof(float) * size);
    if (kernel->values == NULL) {
        fclose(f);
        return false;
    }

    for (int i = 0; i < size; i++) {
        fscanf(f, "%f", &kernel->values[i]);
    }

    fclose(f);
    return true;
}

void free_kernel(kernel_t kernel) { free(kernel.values); }

void convolve_pixel(img_t *src, img_t *dst, kernel_t kernel, int x, int y) {
    pixel_t dst_value;
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;

    for (int i = -kernel.half_n; i <= kernel.half_n; i++) {
        for (int j = -kernel.half_n; j <= kernel.half_n; j++) {
            pixel_t src_value = get_pixel(src, x + i, y + j);
            float kernel_value = get_kernel_value(kernel, i, j);
            r += kernel_value * src_value.r;
            g += kernel_value * src_value.g;
            b += kernel_value * src_value.b;
        }
    }
    // printf("%f %f %f\n", r, g, b);
    dst_value = pixel_from_float(r, g, b);
    set_pixel(dst, x, y, dst_value);
}

void convolve(img_t *src, img_t *dst, kernel_t kernel, int start_x, int start_y,
              int count) {
    int width = src->width;
    int height = src->height;
    int x = start_x;
    int pixel_done = 0;
    for (int y = start_y; y < height && pixel_done < count; y++) {
        for (; x < width && pixel_done < count; x++) {
            convolve_pixel(src, dst, kernel, x, y);
            pixel_done++;
        }
        x = 0;
    }
}

void *convolve_thread(void *params) {
    convolve_params_t *p = (convolve_params_t *)params;
    convolve(p->src, p->dst, p->kernel, p->start_x, p->start_y, p->count);
    return NULL;
}
