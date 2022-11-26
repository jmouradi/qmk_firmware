/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/
#define ORYX_CONFIGURATOR
#define IGNORE_MOD_TAP_INTERRUPT
#undef RGB_MATRIX_HUE_STEP
#define RGB_MATRIX_HUE_STEP 24

#define USB_SUSPEND_WAKEUP_DELAY 0
#define FIRMWARE_VERSION u8"9QWEO/3xPgA"
#define RAW_USAGE_PAGE 0xFF60
#define RAW_USAGE_ID 0x61
#define LAYER_STATE_8BIT
#define RGB_MATRIX_STARTUP_SPD 60
