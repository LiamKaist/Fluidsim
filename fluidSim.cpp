#include "./include/CImg/CImg.h"
#include "stdio.h"
using namespace cimg_library;


CImg<unsigned char> initGrid(int pixel_length, int pixel_width)
{
    CImg<unsigned char> table(pixel_length,pixel_width,1,3,255);
    return table;
}

void fillCell(unsigned char *table, int row,int column,unsigned char colour)
{
    int offset = column*10 + row*10*500;
    for(int i = 0;i<10;i++)
    {
        for(int k = 0;k<10;k++)
        {
            for(int c = 0;c<3;c++)
            {
                table[i + k*500 + offset + c*250000] = colour; /* Change 250000 to something more generic */
            }
        }
    }
}

float velocitiesX[51][51] = {0.0};
float velocitiesY[51][51] = {0.0};
/*
class GridCell
{
    public:
    float velocities[4]; // 0 is left side velocity, 1 is top side velocity, 2 is right side velocity, 3 is bottom side velocity

    GridCell()
    {
        for(int i = 0;i<4;i++)
        {
            velocities[i] = 0.0;
        }    
    }

    // Computes velocities with properties of divergence for an incompressible fluid
    void computeDivergence()
    {
        float divergence;
        divergence = (velocities[2] - velocities[0]) + (velocities[3]- velocities[1]);
        velocities[0] += divergence/4;
        velocities[2] -= divergence/4;
        velocities[1] += divergence/4;
        velocities[3] -= divergence/4; 
    }
};
*/

void updateVelocities(float velocityX[51][51], float velocityY[51][51],float accelerationX,float accelerationY, float timestep)
{
    /* Update velocities about X */
    for(int i = 0;i<51;i++)
    {
        for(int k = 0;k<51;k++)
        {
            velocityX[i][k] += accelerationX * timestep;
        }
    }
    /* Update velocities about Y */
    for(int i = 0;i<51;i++)
    {
        for(int k = 0;k<51;k++)
        {
            velocityY[i][k] += accelerationY * timestep;
        }
    }
}

int main()
{
    /* Should create a grid now and define the borders of the display */
    CImg<unsigned char> visu = initGrid(500,500);
    CImg<unsigned char> replacement(100,100,1,3,0);
    CImgDisplay draw_disp(visu,"Fluid simulation");
    const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
    updateVelocities(velocitiesX,velocitiesY,0.0,-9.81,0.0333); /* DO NOT USE FRACTIONS 1/2, they do not work as floats */
    for(int i = 0;i<51;i++)
    {
        for(int k = 0;k<51;k++)
        {
            printf("%f\n",velocitiesY[i][k]);
        }
    }

    while(!draw_disp.is_closed())
    {
        unsigned char * ptr = visu.data();
        for(int i = 0;i<50;i++)
        {
            for(int k=0;k<50;k++)
            {
                fillCell(ptr,i,k,0);
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int k=0;k<50;k++)
            {
                fillCell(ptr,i,k,255);
            }
        }
        draw_disp.display(visu);


    }
    return 0;   
}