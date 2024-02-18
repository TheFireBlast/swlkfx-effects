#include <stdint.h>
#include <math.h>

// #include <hyprland/src/render/shaders/Textures.hpp>

// TODO: would be nice to get the values from the config file instead

#pragma region Settings
const float noise = 5.0f;
const float contrast = 0.7f;
const float brightness = 0.6f;
#pragma endregion

typedef struct
{
    float r, g, b;
} Color;

float fract(float x)
{
    float i;
    return modff(x, &i);
}

float dot(float x1, float y1, float x2, float y2)
{
    return x1 * x2 + y1 * y2;
}

float hash(float x, float y)
{
    return fract(sinf(dot(x, y, 12.9898f, 78.233f)) * 43758.5453f);
}

int clamp_float_to_byte(float x)
{
    if (x < 0)
        return 0;
    if (x > 255)
        return 255;
    return (int)x;
}

float gain(float x, float k)
{
    float a = 0.5 * powf(2.0 * ((x < 0.5) ? x : 1.0 - x), k);
    return (x < 0.5) ? a : 1.0 - a;
}

// just copied the shader code from hyprland,
// may not have been a good idea
uint32_t swaylock_pixel(uint32_t pix, int x, int y, int width, int height)
{
    Color pixColor = {
        (float)((pix & 0xFF0000) >> 16) / 255.0f,
        (float)((pix & 0x00FF00) >> 8) / 255.0f,
        (float)((pix & 0x0000FF) >> 0) / 255.0f,
    };

    if (contrast != 1.0)
    {
        pixColor.r = gain(pixColor.r, contrast);
        pixColor.g = gain(pixColor.g, contrast);
        pixColor.b = gain(pixColor.b, contrast);
    }

    float noiseHash = hash(x, y);
    float noiseAmount = fract(noiseHash) - 0.5;
    pixColor.r += noiseAmount * noise;
    pixColor.g += noiseAmount * noise;
    pixColor.b += noiseAmount * noise;

    // brightness
    if (brightness < 1.0f)
    {
        pixColor.r *= brightness;
        pixColor.g *= brightness;
        pixColor.b *= brightness;
    }

    return (clamp_float_to_byte(pixColor.r * 255.0f) << 16) |
           (clamp_float_to_byte(pixColor.g * 255.0f) << 8) |
           (clamp_float_to_byte(pixColor.b * 255.0f) << 0);
}
