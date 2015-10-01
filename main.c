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

void usage(char *argv0) {
    printf("Usage:\n");
    printf("%s <input> <kernel> <output> <threads>\n", argv0);
}

int main(int argc, char *argv[]) {
    char *input_fn;
    char *output_fn;
    char *kernel_fn;
    int threads = 3;

    if (argc != 5) {
        usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        input_fn = argv[1];
        kernel_fn = argv[2];
        output_fn = argv[3];
        threads = atoi(argv[4]);
    }

    img_t *img_input = load_ppm(input_fn);
    CHECK_NULL(img_input, "Error while opening the input");

    kernel_t kernel;
    bool load_kernel_ok = load_kernel(kernel_fn, &kernel);
    CHECK_RETURN(load_kernel_ok, false, "Error while opening the kernel");

    img_t *img_output = alloc_img(img_input->width, img_input->height);
    CHECK_NULL(img_output, "Error while allocating the output buffer");

    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (threads == 0) {
        convolve(img_input, img_output, kernel, 0, 0,
                 img_input->width * img_input->height);
    } else {
        convolve_params_t arg_t1 = {img_input, img_output, kernel,
                                    0,         0,          242688};
        convolve_params_t arg_t2 = {img_input, img_output, kernel,
                                    0,         237,        242688};
        convolve_params_t arg_t3 = {img_input, img_output, kernel,
                                    0,         474,        242688};
        pthread_t t1, t2, t3;
        pthread_create(&t1, NULL, convolve_thread, &arg_t1);
        pthread_create(&t2, NULL, convolve_thread, &arg_t2);
        pthread_create(&t3, NULL, convolve_thread, &arg_t3);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    double elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Convolution elapsed time: %f ms\n", elapsed_ms);

    bool write_ok = write_ppm(output_fn, img_output);
    CHECK_RETURN(write_ok, false, "Error while writing output");

    free_img(img_input);
    free_img(img_output);
    return EXIT_SUCCESS;
}
