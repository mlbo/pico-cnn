/** 
 * @brief benchmark for different activation function implementations
 *
 * @author Konstantin Luebeck (University of Tuebingen, Chair for Embedded Systems)
 */

#define JPEG
#define DEBUG

#define NUM_ACTIVATIONS 1024

// 0 = relu
// 1 = softmax
#define FUNCTION 0

#include "pico-cnn/pico-cnn.h"
#include <stdio.h>
#include <omp.h>

enum mode_t {NAIVE, CPU, CGRA, GPU};

void usage() {
    printf("./convolution_benchmark MODE PATH_TO_INPUT_IMAGE\n");
    printf("\tMODE: naive, cpu, cgra, gpu\n");
    printf("\tPATH_TO_INPUT_IMAGE: *.pgm, *.{jpg,jpeg,JPG,JPEG}\n");
}

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "no mode/path to input image provided!\n");
        usage();
        return 1;
    }

    // check if naive of opt was chosen
    mode_t mode = NAIVE;

    if(strcmp(argv[1], "naive") == 0) {
        mode = NAIVE;
    } else if(strcmp(argv[1], "cpu") == 0) {
        mode = CPU;
    } else if(strcmp(argv[1], "cgra") == 0) {
        mode = CGRA;
    } else if(strcmp(argv[1], "gpu") == 0) {
        mode = GPU;
    }

    // check if input image is pgm or jpeg
    uint8_t jpg = 0;
    char * extension;

    extension = strrchr(argv[2], '.');
    
    if(strcmp(extension+1, "pgm") == 0) {
        jpg = 0;
    } else if(strcmp(extension+1, "jpg") == 0 || strcmp(extension+1, "jpeg") == 0 || strcmp(extension+1, "JPG") == 0 || strcmp(extension+1, "JPEG") == 0) {
        jpg = 1;
    }

    uint16_t height, width;
    fp_t* input_image;
  

    if(jpg == 0) {
        fp_t* input_image_pgm;
        if(read_pgm(&input_image_pgm, argv[2], 0, 0.0, 1.0, &height, &width) != 0) {
            fprintf(stderr, "could not read pgm image '%s'!\n", argv[1]);
            return 1;
        }
        input_image = input_image_pgm;
    } else if(jpg == 1) {
        fp_t** input_image_jpg;
        if(read_jpeg(&input_image_jpg, argv[2], 0.0, 1.0, &height, &width) != 0) {
            fprintf(stderr, "could not read jpeg image '%s'!\n", argv[1]);
            return 1;
        }
        input_image = input_image_jpg[0];
        free(input_image_jpg[1]);
        free(input_image_jpg[2]);
    }

    #ifdef DEBUG
    write_pgm(input_image, height, width, "input.pgm");
    #endif

    int i;

    // multiply every second pixel with -1.0
    for(i = 0; i < height*width; i+=2) {
        input_image[i] = input_image[i]*-1.0;
    }

    fp_t** output_image = (fp_t**) malloc(NUM_ACTIVATIONS*height*width*sizeof(fp_t*));

    for(i = 0; i < NUM_ACTIVATIONS; i++) {
        output_image[i] = (fp_t*) malloc(height*width*sizeof(fp_t));
    }

    if(mode == NAIVE) {
        for(i = 0; i < NUM_ACTIVATIONS; i++) {
            relu_naive(input_image, 55, 55, output_image[i]);
        }
    } else if(mode == CPU) {
        for(i = 0; i < NUM_ACTIVATIONS; i++) {
            relu_cpu(input_image, 55, 55, output_image[i]);
        }
    }

    free(input_image);
    free(output_image);

    return 0;
}
