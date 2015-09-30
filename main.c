#include <stdlib.h>
#include <stdio.h>

#include "convolution.h"
#include "image.h"
#include "ppm.h"


int main(void) {
    char *input = "image.ppm";
    char *output = "output.ppm";
    

    img_t *img_input = load_ppm(input);
    //TODO check
    img_t *img_output = alloc_img(img_input->width, img_input->height);

    convolve(img_input, img_output, KERNEL_EDGE, 0, 0, img_input->width*img_input->height);  

    write_ppm(output, img_output);
    //TODO check
    
    free_img(img_input);
    free_img(img_output);
    return EXIT_SUCCESS;
}
