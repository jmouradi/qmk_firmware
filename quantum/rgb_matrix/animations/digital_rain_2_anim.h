#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN_2)
RGB_MATRIX_EFFECT(DIGITAL_RAIN_2)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#        ifndef RGB_DIGITAL_RAIN_DROPS
// lower the number for denser effect/wider keyboard
#            define RGB_DIGITAL_RAIN_DROPS 32
#        endif

#define CUST_ROWS 6
#define CUST_COLS 14

void get_transpose(uint8_t row, uint8_t col, uint8_t *nrow, uint8_t *ncol) {
    if (row == 4 && col == 5) {
        *nrow = 5;
        *ncol = 3;
    } else if (row == 4 && col == 7) {
        *nrow = 11;
        *ncol = 3;
    } else if (row == 5 && col == 3) {
        *nrow = 5;
        *ncol = 4;
    } else if (row == 5 && col == 10) {
        *nrow = 11;
        *ncol = 2;
    } else {      
      *nrow = row + (CUST_ROWS * (col / MATRIX_COLS));
      *ncol = col % MATRIX_COLS; 
    }
    return;
}

uint8_t get_transpose_coordinate (uint8_t row, uint8_t col, uint8_t* led_i) {
  uint8_t transpose_row, transpose_col; 
  get_transpose(row, col, &transpose_row, &transpose_col);
  return rgb_matrix_map_row_column_to_led(transpose_row, transpose_col, led_i);
}

bool DIGITAL_RAIN_2(effect_params_t* params) {
    // algorithm ported from https://github.com/tremby/Kaleidoscope-LEDEffect-DigitalRain
    const uint8_t drop_ticks           = 64;
    const uint8_t pure_blue_intensity = (((uint16_t)rgb_matrix_config.hsv.v) * 3) >> 2;
    const uint8_t max_brightness_boost = (((uint16_t)rgb_matrix_config.hsv.v) * 3) >> 2;
    const uint8_t max_intensity        = rgb_matrix_config.hsv.v;
    const uint8_t decay_ticks          = 0xff / max_intensity;

    static uint8_t drop  = 0;
    static uint8_t decay = 0;

    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof(g_rgb_frame_buffer));
        drop = 0;
    }

    decay++;
    for (uint8_t raw_col = 0; raw_col < CUST_COLS; raw_col++) {
        for (uint8_t raw_row = 0; raw_row < CUST_ROWS; raw_row++) {
            uint8_t row, col;
            get_transpose(raw_row, raw_col, &row, &col);
            if (raw_row == 0 && drop == 0 && rand() < RAND_MAX / RGB_DIGITAL_RAIN_DROPS) {
                // top row, pixels have just fallen and we're
                // making a new rain drop in this column
                g_rgb_frame_buffer[row][col] = max_intensity;
            } else if (g_rgb_frame_buffer[row][col] > 0 && g_rgb_frame_buffer[row][col] < max_intensity) {
                // neither fully bright nor dark, decay it
                if (decay == decay_ticks) {
                    g_rgb_frame_buffer[row][col]--;
                }
            }
            // set the pixel colour
            uint8_t led[LED_HITS_TO_REMEMBER];
            uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);

            // TODO: multiple leds are supported mapped to the same row/column
            if (led_count > 0) {
                if (g_rgb_frame_buffer[row][col] > pure_blue_intensity) {
                    const uint8_t boost = (uint8_t)((uint16_t)max_brightness_boost * (g_rgb_frame_buffer[row][col] - pure_blue_intensity) / (max_intensity - pure_blue_intensity));
                    rgb_matrix_set_color(led[0], boost, boost, max_intensity);
                } else {
                    const uint8_t blue = (uint8_t)((uint16_t)max_intensity * g_rgb_frame_buffer[row][col] / pure_blue_intensity);
                    rgb_matrix_set_color(led[0], 0, 0, blue);
                }
            }
        }
    }
    if (decay == decay_ticks) {
        decay = 0;
    }

    if (++drop > drop_ticks) {
        // reset drop timer
        drop = 0;
        for (uint8_t raw_row = CUST_ROWS - 1; raw_row > 0; raw_row--) {
            for (uint8_t raw_col = 0; raw_col < CUST_COLS; raw_col++) {
                uint8_t row, col;
                get_transpose(raw_row, raw_col, &row, &col);
                // if ths is on the bottom row and bright allow decay

                if ((raw_row == MATRIX_ROWS - 1 || (raw_row == 3 && raw_col == 5)) && g_rgb_frame_buffer[row][col] == max_intensity) {
                    g_rgb_frame_buffer[row][col]--;
                }
                // check if the pixel above is bright
                if (g_rgb_frame_buffer[row - 1][col] >= max_intensity) { // Note: can be larger than max_intensity if val was recently decreased
                    // allow old bright pixel to decay
                    g_rgb_frame_buffer[row - 1][col] = max_intensity - 1;
                    // make this pixel bright
                    g_rgb_frame_buffer[row][col] = max_intensity;
                }
            }
        }
    }
    return false;
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && !defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN)
