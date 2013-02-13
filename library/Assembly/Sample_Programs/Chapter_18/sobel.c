#include <math.h>

#define matrix(a,b,c) a[(b)*(cols)+(c)]

void sobel ( unsigned char *data, float *output, long rows, long cols )
{
    int r, c;
    int gx, gy;


    for ( r = 1; r < rows-1; r++ ) {
        for ( c = 1; c < cols-1; c++ ) {
            gx = -matrix(data,r-1,c-1) + matrix(data,r-1,c+1) +
                 -2*matrix(data,r,c-1) + 2*matrix(data,r,c+1) +
                 -matrix(data,r+1,c-1) + matrix(data,r+1,c+1);
            gy = -matrix(data,r-1,c-1) - 2*matrix(data,r-1,c)
                        - matrix(data,r-1,c+1) +
                 matrix(data,r+1,c-1) + 2*matrix(data,r+1,c)
                        + matrix(data,r+1,c+1);
            matrix(output,r,c) = sqrt((float)(gx)*(float)(gx)+(float)(gy)*(float)(gy));
        }
    }
}
