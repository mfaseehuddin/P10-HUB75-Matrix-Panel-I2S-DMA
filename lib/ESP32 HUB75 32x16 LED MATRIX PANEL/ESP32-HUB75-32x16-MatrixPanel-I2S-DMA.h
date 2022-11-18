// 32x16 Matrix Panel I2S DMA Library
// Author: mfaseehuddin

#ifndef ESP32_HUB75_32x16_MatrixPanel_I2S_DMA_h
#define ESP32_HUB75_32x16_MatrixPanel_I2S_DMA_h

#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h" //we use the base mrfaptastic library for ESP32 HUB75 LED MATRIX PANEL, and map to 32x16 matrix
#include "glcdfont.c"

#endif

// what does ifndef do? -> it checks if the library is already defined, if not, it will define it. If it is defined, it will not define it again. This is to prevent redefinition of the library.
// can we use ifndef for variables? -> yes, but it is not recommended. It is better to use #define to define variables. #define is a preprocessor directive, and it will replace the variable with the value. #ifndef is a compiler directive, and it will not compile the code if the variable is already defined.

// we will derive a gfx library from the base library

// Path: lib/ESP32 HUB75 32x16 LED MATRIX PANEL/ESP32-HUB75-32x16-MatrixPanel-I2S-DMA-GFX.h
class MatrixPanel32x16_I2S_DMA : public MatrixPanel_I2S_DMA
{

private:
public:
    MatrixPanel32x16_I2S_DMA(const HUB75_I2S_CFG &config) : MatrixPanel_I2S_DMA(config){};

    struct point
    {
    private:
        int oX;
        int oY;

    public:
        int x;
        int y;
        point(int xIn, int yIn)
        {
            oX = xIn;
            oY = yIn;
            // todo: define the oY to oX Calculation Mathematically
            oX += (oX / 8) * 8;
            // calculing x offset
            // if (this->oX >= 56 && this->oX <= 64)
            // {
            //     this->oX = this->oX + 56;
            // }
            // else if (this->oX >= 48 && this->oX <= 55)
            // {
            //     this->oX = this->oX + 48;
            // }
            // else if (this->oX >= 40 && this->oX <= 47)
            // {
            //     this->oX = this->oX + 40;
            // }
            // else if (this->oX >= 32 && this->oX <= 39)
            // {
            //     this->oX = this->oX + 32;
            // }
            // else if (this->oX >= 24 && this->oX <= 31)
            // {
            //     this->oX = this->oX + 24;
            // }
            // else if (this->oX >= 16 && this->oX <= 23)
            // {
            //     this->oX = this->oX + 16;
            // }
            // else if (this->oX >= 8 && this->oX <= 15)
            // {
            //     this->oX = this->oX + 8;
            // }
            // todo: can these conditions be written mathematically? -> x = oX + (oY % 4 == 0 ? 8 : 0) ✅ works
            x = oX + (((oY >= 4 && oY <= 7) || (oY >= 11 && oY <= 14)) ? 8 : 0);
            // x = oX + (oY % 4 == 0 ? 8 : 0);
            // if (oY > 3 && oY < 8 || oY > 11 && oY < 16)
            // {
            //     this->x = oX + 8;
            // }
            // else
            // {
            //     this->x = oX;
            // }
            // todo: find pattern for extending this -> cant be done until I am able to chain more displays
            // y = oY + ((oY >= 8 && oY <= 16) ? 8 : 0);
            // if (oY > 7 && oY < 13 || oY > 12 && oY < 17) // can this condition be simplified? -> if(oY >= 8 && oY <= 16)
            if (oY >= 8 && oY <= 16) // can this condition be simplified? -> if(oY >= 8 && oY <= 16)
            {
                this->y = oY + 8;
            }
            else
            {
                this->y = oY;
            }
        }

        void debugPoint()
        {
            // if the serial is not initialized, we will initialize it
            if (!Serial)
            {
                Serial.begin(115200);
            }
            Serial.print(" x: ");
            Serial.print(x);
            Serial.print(" y: ");
            Serial.print(y);
        }
    };

    // todo override the commonly used functions from the base library //the overriding will be simple as the base library already works, we just need to map the points to the 32x16 matrix using our struct point

    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
        point p(x, y);
        uint8_t r, g, b;
        // color breakdown
        color565to888(color, r, g, b);
        updateMatrixDMABuffer(p.x, p.y, r, g, b);
    }

    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x = 1, uint8_t size_y = 1)
    {
        if (!gfxFont)
        { // 'Classic' built-in font
            if (!gfxFont)
            { // 'Classic' built-in font

                if ((x >= _width) ||              // Clip right
                    (y >= _height) ||             // Clip bottom
                    ((x + 6 * size_x - 1) < 0) || // Clip left
                    ((y + 8 * size_y - 1) < 0))   // Clip top
                    return;

                if (!_cp437 && (c >= 176))
                    c++; // Handle 'classic' charset behavior

                startWrite();
                for (int8_t i = 0; i < 5; i++)
                { // Char bitmap = 5 columns
                    uint8_t line = pgm_read_byte(&font[c * 5 + i]);
                    for (int8_t j = 0; j < 8; j++, line >>= 1)
                    {
                        if (line & 1)
                        {
                            if (size_x == 1 && size_y == 1)
                                drawPixel(x + i, y + j, color);
                            // writePixel(x + i, y + j, color);
                            // else
                            //   writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, color);
                        }
                        else if (bg != color)
                        {
                            if (size_x == 1 && size_y == 1)
                                drawPixel(x + i, y + j, bg);
                            // writePixel(x + i, y + j, bg);
                            // else
                            //   writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
                        }
                    }
                }
                endWrite();
            }
        }
        /**
        * Todo: add support for custom fonts
        else
        { // Custom font

            // Character is assumed previously filtered by write() to eliminate
            // newlines, returns, non-printable characters, etc.  Calling
            // drawChar() directly with 'bad' characters of font may cause mayhem!

            c -= (uint8_t)pgm_read_byte(&gfxFont->first);
            GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
            uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);

            uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
            uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height);
            int8_t xo = pgm_read_byte(&glyph->xOffset),
                   yo = pgm_read_byte(&glyph->yOffset);
            uint8_t xx, yy, bits = 0, bit = 0;
            int16_t xo16 = 0, yo16 = 0;

            if (size_x > 1 || size_y > 1)
            {
                xo16 = xo;
                yo16 = yo;
            }

            // Todo: Add character clipping here

            // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
            // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
            // has typically been used with the 'classic' font to overwrite old
            // screen contents with new data.  This ONLY works because the
            // characters are a uniform size; it's not a sensible thing to do with
            // proportionally-spaced fonts with glyphs of varying sizes (and that
            // may overlap).  To replace previously-drawn text when using a custom
            // font, use the getTextBounds() function to determine the smallest
            // rectangle encompassing a string, erase the area with fillRect(),
            // then draw new text.  This WILL infortunately 'blink' the text, but
            // is unavoidable.  Drawing 'background' pixels will NOT fix this,
            // only creates a new set of problems.  Have an idea to work around
            // this (a canvas object type for MCUs that can afford the RAM and
            // displays supporting setAddrWindow() and pushColors()), but haven't
            // implemented this yet.

            startWrite();
            for (yy = 0; yy < h; yy++)
            {
                for (xx = 0; xx < w; xx++)
                {
                    if (!(bit++ & 7))
                    {
                        bits = pgm_read_byte(&bitmap[bo++]);
                    }
                    if (bits & 0x80)
                    {
                        if (size_x == 1 && size_y == 1)
                        {
                            writePixel(x + xo + xx, y + yo + yy, color);
                        }
                        else
                        {
                            writeFillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
                                          size_x, size_y, color);
                        }
                    }
                    bits <<= 1;
                }
            }
            endWrite();

        } // End classic vs custom font
        */
    }

    void drawText(int16_t x, int16_t y, const char *text, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
    {
        while (*text)
        {
            drawChar(x, y, *text++, color, bg, size_x, size_y);
            x += 6 * size_x; // Move cursor right
        }
    }

    uint16_t colorWheel(uint8_t pos)
    {
        if (pos < 85)
        {
            return color565(pos * 3, 255 - pos * 3, 0);
        }
        else if (pos < 170)
        {
            pos -= 85;
            return color565(255 - pos * 3, 0, pos * 3);
        }
        else
        {
            pos -= 170;
            return color565(0, pos * 3, 255 - pos * 3);
        }
    }

    friend uint16_t colorWheel(uint8_t pos)
    {
        if (pos < 85)
        {
            return color565(pos * 3, 255 - pos * 3, 0);
        }
        else if (pos < 170)
        {
            pos -= 85;
            return color565(255 - pos * 3, 0, pos * 3);
        }
        else
        {
            pos -= 170;
            return color565(0, pos * 3, 255 - pos * 3);
        }
    }

    void drawIco(int *ico, int16_t x, int16_t y, int16_t rows, int16_t cols)
    {
        /*  drawIcon draws a C style bitmap.
 Example 10x5px bitmap of a yellow sun

  int half_sun [50] = {
      0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
      0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000,
      0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
      0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0,
      0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
  };

  MatrixPanel_I2S_DMA matrix;

  matrix.drawIcon (half_sun, 0,0,10,5);
*/

        int i, j;
        for (i = 0; i < cols; i++)
        {
            for (j = 0; j < rows; j++)
            {
                drawPixel(x + j, y + i, (uint16_t)(ico[i * rows + j]));
            }
        }
    }
};

