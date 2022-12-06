#if defined(ENABLE_RGB_MATRIX_LEARNING)
RGB_MATRIX_EFFECT(LEARNING)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define CYCLE_MODULUS 8

bool LEARNING(effect_params_t* params) {
    static uint8_t cycle;
    static uint8_t row;
    static uint8_t col;
    static uint8_t led;
    
    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        cycle = row = col = led = 0;
    }

    cycle = (cycle + 1) % CYCLE_MODULUS;
    if (cycle == 0) {
        rgb_matrix_set_color(led, 0x00, 0x00, 0x00);
        do {
            col = (col + 1) % LED_MATRIX_COLS;
            if (col == 0) {
                 row = (row + 1) % LED_MATRIX_ROWS;
            }
        } while (rgb_matrix_map_row_column_to_led(row, col, &led) == 0);
    }
    rgb_matrix_map_row_column_to_led(row, col, &led);
    rgb_matrix_set_color(led, 0xff, 0xff, 0xff);
    return false;
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN_2)
