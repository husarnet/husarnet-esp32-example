# Project template for ESP32

## Setting up

You will need

1. As a first step, download ESP32 toolchain: https://docs.espressif.com/projects/esp-idf/en/stable/get-started/#setup-toolchain

2. Get ESP-IDF:

   ```
   git clone --recursive https://github.com/husarnet/esp-idf ~/apps/husarnet-esp-idf
   ```

   Export the environment variable ESP_IDF:

   ```
   export ESP_IDF=$HOME/apps/husarnet-esp-idf
   ```

3. Build the example:

   ```
   make -j4
   ```
