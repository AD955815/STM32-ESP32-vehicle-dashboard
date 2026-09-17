# STM32-ESP32-vehicle-dashboard
Distributed vehicle dashboard and offline navigation system based on STM32, ESP32-S3, FreeRTOS and LVGL9.5

# Distributed Vehicle Dashboard & Offline Navigation System

A distributed embedded vehicle dashboard and offline navigation
system based on STM32F103, ESP32-S3, FreeRTOS and LVGL.

## System Architecture

STM32F103C8T6
    │
   CAN
    ↓
STM32F103ZET6
    │
   UART
    ↓
ESP32-S3
    │
 LVGL
    ↓
480 × 320 LCD
