/* File 'D:\ELECTRONIC ASSEMBLY LCD-Tools Portable\Data\logo.BMP' as include

 the array starts with a 2 byte header:
  1th Byte: Width of image in dots
  2th Byte: Height of image in dots
  After that image data will follow */

#define Image_logo_BMP_LEN  110

const unsigned char Image_logo_BMP[Image_logo_BMP_LEN] __attribute__((section(".progmem.data")))=
{
   27, 27,
    0,  0,252,252, 12, 12,204,204,204,204,204,204,204,204,204,204,
  204,204,204,204,204,204,204,204,204,  0,  0,  0,  0,255,255,  0,
    0,255,255,  0,  0,252,252, 12, 12,204,204,204,204,204,204,204,
  204,204,204,204,  0,  0,  0,  0,255,255,  0,  0,255,255,  0,  0,
  255,255,  0,  0,255,255,  0,  0,156,156,156,128,128,252,252,  0,
    0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
    1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0
};
