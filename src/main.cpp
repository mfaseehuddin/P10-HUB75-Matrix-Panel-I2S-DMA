#include <Arduino.h>

#include <ESP32-HUB75-32x16-MatrixPanel-I2S-DMA.h>

#define PANEL_RES_X 64
#define PANEL_RES_Y 16
#define PANEL_CHAIN 1

MatrixPanel32x16_I2S_DMA *dma_display = nullptr;

// custom color def
uint16_t BLACK = dma_display->color565(0, 0, 0);
uint16_t WHITE = dma_display->color565(255, 255, 255);
uint16_t RED = dma_display->color565(255, 0, 0);
uint16_t GREEN = dma_display->color565(0, 255, 0);
uint16_t BLUE = dma_display->color565(0, 0, 255);

uint8_t wheelVal = 0;

void setup()
{

    HUB75_I2S_CFG matrixConfig(
        PANEL_RES_X * 2,
        PANEL_RES_Y * 2,
        PANEL_CHAIN);

    dma_display = new MatrixPanel32x16_I2S_DMA(matrixConfig);
    dma_display->begin();
    dma_display->clearScreen();

    dma_display->drawText(0, 0, "Faseeh", WHITE, BLACK, 1, 1);

    delay(1000);
    dma_display->clearScreen();

    for (uint16_t x = 0; x < PANEL_RES_X; x++)
    {
        for (uint16_t y = 0; y < PANEL_RES_Y; y++)
        {
            dma_display->drawPixel(x, y, dma_display->color565(255, 0, 0));
        }
        delay(20);
        dma_display->clearScreen();
    }

    int half_sun[50] = {
        0x0000,
        0x0000,
        0x0000,
        0xffe0,
        0x0000,
        0x0000,
        0xffe0,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0xffe0,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0xffe0,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0x0000,
        0x0000,
        0x0000,
        0xffe0,
        0x0000,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0x0000,
        0xffe0,
        0x0000,
        0x0000,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0xffe0,
        0x0000,
        0x0000,
    };

    dma_display->drawIco(half_sun, 5,5, 10, 5);

    
}

void loop()
{
}