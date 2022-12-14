#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_ON_OFF)
RGB_MATRIX_EFFECT(TYPING_ON_OFF)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool TYPING_ON_OFF(effect_params_t* params) {
    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
    }

    uint8_t led_min = RGB_MATRIX_LED_PROCESS_LIMIT * params->iter;
    uint8_t led_max = led_min + RGB_MATRIX_LED_PROCESS_LIMIT;

    for (int i = led_min; i < led_max; i++) {
        uint8_t row = i % LED_MATRIX_ROWS;
        uint8_t col = i / LED_MATRIX_ROWS;
        // set the pixel colour
        uint8_t led[LED_HITS_TO_REMEMBER];
        uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);
        for (uint8_t j = 0; j < led_count; ++j) {
            if (!HAS_ANY_FLAGS(g_led_config.flags[led[j]], params->flags)) {
                continue;
            }
            g_rgb_frame_buffer[row][col] ^= 1;
            if (g_rgb_frame_buffer[row][col] == 1) {
              rgb_matrix_set_color(led[j], 0xff, 0xff, 0xff);
            } else {
              rgb_matrix_set_color(led[j], 0x00, 0x00, 0x00);
            }
        }
    }
    return true;
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_ON_OFF)
