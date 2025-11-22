#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>


int main(void) {
    uint16_t w_ratio = 16u;
    uint16_t h_ratio = 9u;
    uint16_t factor = 60u;

    uint16_t w = w_ratio * factor;
    uint16_t h = h_ratio * factor;
    

    uint16_t max_offset = 60u;
    #pragma omp parallel for schedule(dynamic)
    for (uint16_t offset = 0; offset < max_offset; offset++) { 
        char output_fp[256];
        /* Open output file corresponding to current offset */
        snprintf(output_fp, sizeof(output_fp), "checker-%04u.ppm", offset);
        FILE *f = fopen(output_fp, "wb");
        if (NULL == f) {
            fprintf(stderr, "[ERROR] Could not open %s because: %s\n", output_fp, strerror(errno));
            exit(EXIT_FAILURE);
        }
        /* Preamble output file */
        fprintf(f, "P6\n"
                "%d %d\n"
                "255\n", w, h);
        
        for (uint16_t y = 0; y < h; ++y) { /* iter rows */
            for (uint16_t x = 0; x < w; ++x) {
                if (((x + offset) / factor + (y + offset) / factor) % 2) {
                    fputc(0xFF, f); 
                    fputc(0x00, f); 
                    fputc(0x00, f); 
                } else {
                    fputc(0x00, f); 
                    fputc(0x00, f); 
                    fputc(0x00, f); 
                }
            }
        }
        fclose(f);
        printf("[INFO] Generated as %s\n", output_fp);
    }

    return EXIT_SUCCESS;
}
