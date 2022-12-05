#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_LEARNING)
RGB_MATRIX_EFFECT(LEARNING)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
// lower the number for denser effect/wider keyboard

#include "animation_common.h"

#define CYCLE_MODULUS 8

bool LEARNING(effect_params_t* params) {
    static uint8_t cycle;
    static uint8_t row; 
    static uint8_t col;

    uint8_t led;
    
    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof(g_rgb_frame_buffer));
        cycle = row = col = 0;
    }

    cycle = (cycle + 1) % CYCLE_MODULUS;
    if (cycle == 0) {
        get_transpose_coordinate(row, col, &led);
        rgb_matrix_set_color(led, 0x00, 0x00, 0x00);
        do {
            col = (col + 1) % CUST_COLS;
            if (col == 0) {
                 row = (row + 1) % CUST_ROWS;
            }
        } while (get_transpose_coordinate(row, col, &led) == 0);
    }
    get_transpose_coordinate(row, col, &led);
    rgb_matrix_set_color(led, 0xff, 0xff, 0xff);
    return false;
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && !defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN_2)
