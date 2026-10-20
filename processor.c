#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "inf1102_config.h"

#define IMAGE_SIZE (INF1102_IMAGE_WIDTH * INF1102_IMAGE_HEIGHT * 3)

static void edge_detect(uint8_t *im_in, uint8_t *im_out, int width, int height) {
    memset(im_out, 0, width * height * 3);	// initialize the image to pure black
    for (int y = 0; y < height; y++) {		// loop from top to bottom
        for (int x = 0; x < width-1; x++) {	// loop from left to right 

            // initialize all variables
            float prevR, prevG, prevB, nextR, nextG, nextB, diffR, diffG, diffB, total_diff, edge;

            // starting index of each pixel = (y * width + x) * 3
            int current_index = (y * width + x) * 3;

            prevR = (float) im_in[current_index]; 
            prevG = (float) im_in[current_index+1]; 
            prevB = (float) im_in[current_index+2]; 

            nextR = (float) im_in[current_index+3]; 
            nextG = (float) im_in[current_index+4]; 
            nextB = (float) im_in[current_index+5]; 
            
            // Calculate difference between neighbouring pixels 
            diffR = fabsf(prevR - nextR);
            diffG = fabsf(prevG - nextG);
            diffB = fabsf(prevB - nextB);

            // Average differences across all channels
            total_diff = (diffR + diffG + diffB) / 3;

            // If the pixels are very different from each other, is an edge
            if (total_diff > 50) {
                edge = 255.0;
            }
            else {
                edge = 0.0;
            }

            im_out[current_index+2] = im_out[current_index+1] = im_out[current_index] = (uint8_t) edge;
        }
    }

}

/* ==================== Main ==================== */

int main(void) 
{
    char magic[3];
    int width, height, maxval;
    //read the "magic" value that tells us what kind of image it is, and the size
    if (scanf("%2s %d %d %d", magic, &width, &height, &maxval) != 4) 
    {
        fprintf(stderr, "failed to read PPM header\n");
        return 1;
    }
    getchar(); // consume newline

	//request two memory spaces, one for the input image and one for the output image
    uint8_t *im_in = malloc(width * height * 3);
    uint8_t *im_out = malloc(width * height * 3);
    //end with an error if either memory space can't be allocated
    if (!im_in || !im_out) { perror("malloc"); return 1; }

	//read in the entire image (*3 is for the three colours, R, G & B)
    size_t nread = fread(im_in, 1, width * height * 3, stdin);
    //if the thing that got read in is the wrong size, stop with an error.
    if ((int)nread != width * height * 3) 
    {
        fprintf(stderr, "short read from stdin\n");
        return 1;
    }

    /****************************************************************************/	
    // Hi professor!!!!! I hope you are doing well :>
    edge_detect(im_in, im_out, width, height);

    /****************************************************************************/

    fprintf(stderr, "Writing output...\n");
    
    //write the first line of the image (the type, ad the size)
    fprintf(stdout, "P6\n%d %d\n255\n", width, height);
    fflush(stdout);
    
    //write the entire output image
    size_t nw = fwrite(im_out, 1, width * height * 3, stdout);
    if ((int)nw != width * height * 3) 
    {
        fprintf(stderr, "short write to stdout\n");
        return 1;
    }

	//free up the memory we used
    free(im_in);
    free(im_out);
    return 0;
}
