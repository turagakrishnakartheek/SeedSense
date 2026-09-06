#include "esp_camera.h"

// =========================
// Camera Pins
// =========================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0

#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup()
{
  Serial.begin(115200);

  delay(2000);

  Serial.println("SeedSense USB Mode");

  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 10000000;

  config.pixel_format = PIXFORMAT_RGB565;

  config.frame_size = FRAMESIZE_QQVGA;

  config.jpeg_quality = 15;

  config.fb_count = 1;

  config.fb_location = CAMERA_FB_IN_DRAM;

  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

  if (esp_camera_init(&config) != ESP_OK)
  {
    Serial.println("Camera Init Failed");

    while (1);
  }

  Serial.println("Camera Ready");
  Serial.println("READY");
}

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();

    if (cmd == 'c')
    {
      camera_fb_t *fb = esp_camera_fb_get();

      if (fb == NULL)
      {
        Serial.println("FAIL");
        return;
      }

      Serial.write("SS01", 4);

      Serial.print(fb->len);
      Serial.write('\n');

      Serial.write(fb->buf, fb->len);

      Serial.write("EE99", 4);

      esp_camera_fb_return(fb);
    }
  }
}