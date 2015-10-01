#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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
    char *input = "image.ppm";
    char *output = "output.ppm";
    char *kernel;
    int threads = 3;

    if (argc != 5) {
        usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        input = argv[1];
        kernel = argv[2];
        output = argv[3];
        threads = atoi(argv[4]);
    }

    img_t *img_input = load_ppm(input);
    CHECK_NULL(img_input, "Error while opening the input");

    img_t *img_output = alloc_img(img_input->width, img_input->height);
    CHECK_NULL(img_output, "Error while allocating the output buffer");

    if (threads == 0) {
        convolve(img_input, img_output, KERNEL_EDGE, 0, 0,
                 img_input->width * img_input->height);
    } else {
        convolve_params_t arg_t1 = {img_input, img_output, KERNEL_EDGE,
                                    0,         0,          242688};
        convolve_params_t arg_t2 = {img_input, img_output, KERNEL_EDGE,
                                    0,         237,        242688};
        convolve_params_t arg_t3 = {img_input, img_output, KERNEL_EDGE,
                                    0,         474,        242688};
        pthread_t t1, t2, t3;
        pthread_create(&t1, NULL, convolve_thread, &arg_t1);
        pthread_create(&t2, NULL, convolve_thread, &arg_t2);
        pthread_create(&t3, NULL, convolve_thread, &arg_t3);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);
    }

    bool write_ok = write_ppm(output, img_output);
    CHECK_RETURN(write_ok, false, "Error while writing output");

    free_img(img_input);
    free_img(img_output);
    return EXIT_SUCCESS;
}
