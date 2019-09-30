#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct rgb
{
    float r,g,b;
} rgb;


void saveBMP(char* path, int w, int h, int dpi, rgb* imageBuffer)
{
    FILE* img;
    int img_size = w*h;
    int file_size = 54+4*img_size;
    int ppm = dpi*39.375;


    struct BMP_file_header
    {
        unsigned char   BMP_type[2];// 2 bytes
        int             file_size;  // 4 bytes
        short           reserved1;  // 2 bytes
        short           reserved2;  // 2 bytes
        unsigned int    offset_bits;// 4 byte
                                    //---TOTAL: 14 bytes
    } fh;

    struct BMP_image_header
    {
        unsigned int    header_size;    // 4 bytes
        unsigned int    w;              // 4 bytes
        unsigned int    h;              // 4 bytes
        short int       planes;         // 2 bytes
        short int       bit_count;      // 2 bytes
        unsigned int    compression;    // 4 bytes
        unsigned int    img_size;       // 4 bytes
        unsigned int    ppm_x;          // 4 bytes
        unsigned int    ppm_y;          // 4 bytes
        unsigned int    clr_used;       // 4 bytes
        unsigned int    clr_important;  // 4 bytes
                                        //---TOTAL: 40 bytes
    } ih;

    memcpy(&fh.BMP_type, "BM", 2);
    fh.file_size        = file_size;
    fh.reserved1        = 0;
    fh.reserved2        = 0;
    fh.offset_bits      = 0;

    ih.header_size      = sizeof(ih);
    ih.w                = w;
    ih.h                = h;
    ih.planes           = 1;
    ih.bit_count        = 24;
    ih.compression      = 0;
    ih.img_size         = img_size;
    ih.ppm_x            = ppm;
    ih.ppm_y            = ppm;
    ih.clr_used         = 0;
    ih.clr_important    = 0;

    img = fopen(path, "wb");
    fwrite(&fh, 1, 14, img);
    fwrite(&ih, 1, sizeof(ih), img);

    for(int i=0; i<img_size; i++)
    {
        rgb pixel = imageBuffer[i];
        float r = pixel.r;
        float g = pixel.g;
        float b = pixel.b;
        unsigned char color[3] = {b, g, r};
        fwrite(color, 1, sizeof(color), img);



    }

    fclose(img);

}



typedef struct point{
    float x;
    float y;
} point;






point nextPoint(point p, float a, float b, float c)
{
    point result;
    float sgn = p.x<0 ? -1 : 1;
    result.x = p.y - sqrt(fabs(b*p.x-c));
    result.y = a - p.x;
    return result;
}


void plot(point p, rgb* array, int r, int g, int b, int w, int h, float xmin, float xmax, float ymin, float ymax)
{
    float x = floor((p.x-xmin)/(xmax-xmin)*w);
    float y = floor((p.y-ymin)/(ymax-ymin)*h);

    if(x<0 || x>= w || y<0 || y>= h) return;

    int a = y * w + x;
    array[a].r = r;
    array[a].g = g;
    array[a].b = b;

}



int main(void)
{


    int width  = 1000,
        height = 1000,
        dpi = 96;

    rgb* pixels = calloc(width * height, sizeof(rgb));

    point p={0};

    for(int i=0;i<10000000;i++)
    {
        p = nextPoint(p,2.3251875278223,0.15857825,0.3187578125);
        int r = 20+((i/10000)*16)%236;
        int g = 100+((i/5900)*27)%156;
        int b = ((i/10000)*16)%256; 
        plot(p, pixels, r, g, b, width, height, -3,3, -3, 3);
    }

            
    saveBMP("hopalong.bmp", width, height, dpi, pixels);

    return 0;
}


