#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <cglm/cglm.h>

/* not perfect but enough for here */
#define ABS(x) ((x) > 0 ? (x): -(x))

void vec2_yx(const vec2 v, vec2 res) {
    res[0] = v[1];
    res[1] = v[0];
}

void vec2_xyyx(const vec2 v, vec4 res) {
    res[0] = v[0];
    res[1] = v[1];
    res[2] = v[1];
    res[3] = v[0];
}

void vec2_cos(vec2 v) {
    v[0] = cosf(v[0]);
    v[1] = cosf(v[1]);
}

void vec4_sin(vec4 v) {
    v[0] = sinf(v[0]);
    v[1] = sinf(v[1]);
    v[2] = sinf(v[2]);
    v[3] = sinf(v[3]);
}

void vec4_tanh(vec4 v) {
    v[0] = tanhf(v[0]);
    v[1] = tanhf(v[1]);
    v[2] = tanhf(v[2]);
    v[3] = tanhf(v[3]);
}

void vec4_exp(vec4 v) {
    v[0] = expf(v[0]);
    v[1] = expf(v[1]);
    v[2] = expf(v[2]);
    v[3] = expf(v[3]);
}


int main(void) {
    uint16_t w_ratio = 16u;
    uint16_t h_ratio = 9u;
    uint16_t factor = 60u;

    uint16_t w = w_ratio * factor;
    uint16_t h = h_ratio * factor;
    
    uint16_t max_ts = 240u;
    char output_fp[256];
    for (uint16_t ts = 0; ts < max_ts; ts++) { 
        /* Open output file corresponding to current ts */
        snprintf(output_fp, sizeof(output_fp), "plasma-%04u.ppm", ts);
        FILE *f = fopen(output_fp, "wb");
        if (NULL == f) {
            fprintf(stderr, "[ERROR] Could not open %s because: %s\n", output_fp, strerror(errno));
            return EXIT_FAILURE;
        }
        /* Preamble output file */
        fprintf(f, "P6\n"
                "%d %d\n"
                "255\n", w, h);
        
        vec2 r = {(float)w, (float)h};
        float t = (float)ts / (float)max_ts * 2.0f * M_PI;
        for (uint16_t y = 0; y < h; ++y) { /* iter rows */
            for (uint16_t x = 0; x < w; ++x) {
                vec2 FC = {(float)x, (float)y};
                vec2 p;
                glm_vec2_scale(FC, 2.0f, p);
                glm_vec2_sub(p, r, p);
                glm_vec2_divs(p, r[1], p);
                vec2 l = GLM_VEC2_ZERO_INIT;                 
                glm_vec2_adds(l, 4.0f - 4.0f * ABS(0.7f - glm_vec2_dot(p, p)), l);
                vec2 v;
                glm_vec2_mul(p, l, v);
                vec4 o = GLM_VEC4_ZERO_INIT;  /* o: output of a single fragment (pixel) */
                uint8_t iy = 0;
                for (; iy++ < 8;) {
                    vec2 tmp0;
                    vec2_yx(v, tmp0);
                    glm_vec2_scale(tmp0, (float)iy, tmp0);
                    glm_vec2_add(tmp0, (vec2){0.0f, (float)iy}, tmp0);
                    glm_vec2_adds(tmp0, t, tmp0);
                    vec2_cos(tmp0);
                    glm_vec2_divs(tmp0, (float)iy, tmp0);
                    glm_vec2_adds(tmp0, 0.7f, tmp0);
                    glm_vec2_add(v, tmp0, v);

                    vec4 tmp1;
                    vec2_xyyx(v, tmp1);
                    vec4_sin(tmp1);
                    glm_vec4_adds(tmp1, 1.0f, tmp1);
                    glm_vec4_scale(tmp1, ABS(v[0] - v[1]), tmp1);
                    glm_vec4_add(o, tmp1, o);
                }
                vec4 tmp3;
                glm_vec4_scale((vec4){-1.0f, 1.0f, 2.0f, 0.0f}, -p[1], tmp3);
                glm_vec4_adds(tmp3, l[0] - 4.0f, tmp3);
                vec4_exp(tmp3);
                glm_vec4_scale(tmp3, 5.0f, tmp3);
                glm_vec4_div(tmp3, o, o);
                vec4_tanh(o);

                fputc((char)(o[0] * 255.0f), f);
                fputc((char)(o[1] * 255.0f), f);
                fputc((char)(o[2] * 255.0f), f);
            }
        }
        fclose(f);
        printf("[INFO] Generated %s (%3d/%3d)\n", output_fp, ts + 1, max_ts);
    }

    return EXIT_SUCCESS;
}

