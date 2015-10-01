#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "convolution.h"
#include "image.h"
#include "ppm.h"

int main(void) {
    char *input = "image.ppm";
    char *output = "output.ppm";
    int threads = 3;

    img_t *img_input = load_ppm(input);
    // TODO check
    img_t *img_output = alloc_img(img_input->width, img_input->height);

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

    write_ppm(output, img_output);
    // TODO check

    free_img(img_input);
    free_img(img_output);
    return EXIT_SUCCESS;
}
