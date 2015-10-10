/**
 * @file main.c
 * @brief Main program
 * @author Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "convolution.h"
#include "image.h"
#include "ppm.h"

#define CHECK_NULL(var, msg) CHECK_RETURN(var, NULL, msg)
#define CHECK_RETURN(var, value, msg)                                          \
    if (var == value) {                                                        \
        perror(msg);                                                           \
        return EXIT_FAILURE;                                                   \
    }

//
// Print usage informations
//
void usage(char *argv0) {
    printf("Usage:\n");
    printf("%s <input> <kernel> <output> <threads>\n", argv0);
}

//
// Main
//
int main(int argc, char *argv[]) {
    char *input_fn;
    char *output_fn;
    char *kernel_fn;
    int nb_threads;

    // Application needs 5 parameters (4 + application name)
    if (argc != 5) {
        usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        input_fn = argv[1];
        kernel_fn = argv[2];
        output_fn = argv[3];
        nb_threads = atoi(argv[4]);
    }

    // Load image
    img_t *img_input = load_ppm(input_fn);
    CHECK_NULL(img_input, "Error while opening the input");

    // Load kernel
    kernel_t kernel;
    bool load_kernel_ok = load_kernel(kernel_fn, &kernel);
    CHECK_RETURN(load_kernel_ok, false, "Error while opening the kernel");

    // Prepare output image
    img_t *img_output = alloc_img(img_input->width, img_input->height);
    CHECK_NULL(img_output, "Error while allocating the output buffer");

    struct timespec start, finish;
    // Start clock
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (nb_threads == 0) {
        // Sequential
        convolve(img_input, img_output, kernel, 0, 0,
                 img_input->width * img_input->height);
    } else {
        // Thread(s)
        pthread_t threads[nb_threads];
        convolve_params_t convolve_params[nb_threads];
        int nb_pixel = img_input->width * img_input->height;
        int pixel_done = 0;

        // launch threads
        for (int i = 0; i < nb_threads; i++) {
            int pixel_to_do = (nb_pixel - pixel_done) / (nb_threads - i);
            int y = pixel_done / img_input->width;
            int x = pixel_done - (y * img_input->width);
            convolve_params_t arg = {img_input, img_output, kernel,
                                     x,         y,          pixel_to_do};
            pixel_done += pixel_to_do;
            convolve_params[i] = arg;
            pthread_create(&threads[i], NULL, convolve_thread,
                           &convolve_params[i]);
        }

        // join threads
        for (int i = 0; i < nb_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    // Show time elapsed
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    fprintf(stderr, "Convolution elapsed time: %f ms\n", elapsed_ms);

    bool write_ok = write_ppm(output_fn, img_output);
    CHECK_RETURN(write_ok, false, "Error while writing output");

    // Free memory
    free_img(img_input);
    free_img(img_output);
    free_kernel(kernel);
    return EXIT_SUCCESS;
}
