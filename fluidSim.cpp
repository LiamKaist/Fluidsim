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
float cellTypeArray[50][50] = {0.0}; /* 0 for an obstacle , 1 for fluid */
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

void accountForIncompressibility(float velocityX[51][51], float velocityY[51][51], float cellTypes[50][50])
{
    float divergence;
    float s;
    /* From 1 to 49 to prevent side effects, I am considering the fluid to be encapsulated in a box */
    for(int i=1;i<49;i++)
    {
        for(int k=1;k<49;k++)
        {
            divergence = velocityX[i][k+1] - velocityX[i][k] + velocityY[i][k] - velocityY[i+1][k];
            printf("Divergence Before :%f\n",divergence);
            s = cellTypes[i-1][k] + cellTypes[i][k-1] + cellTypes[i+1][k] + cellTypes[i][k+1];
            velocityX[i][k] += divergence * cellTypes[i][k-1]/s ;
            velocityX[i][k+1] -= divergence * cellTypes[i][k+1]/s;
            velocityY[i+1][k] += divergence * cellTypes[i+1][k]/s;
            velocityY[i][k] -= divergence * cellTypes[i-1][k]/s;
            divergence = velocityX[i][k+1] - velocityX[i][k] + velocityY[i][k] - velocityY[i+1][k];
            printf("Divergence After :%f\n",divergence);
        }
    }
}

void advectVelocities()
{

}

int main()
{
    /* Should create a grid now and define the borders of the display */
    CImg<unsigned char> visu = initGrid(500,500);
    CImg<unsigned char> replacement(100,100,1,3,0);
    CImgDisplay draw_disp(visu,"Fluid simulation");
    const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };

    /* Setting the type of the cells for divergence calculation */
    for(int i = 1;i<49;i++)
    {
        for(int k = 1;k<49;k++)
        {
            cellTypeArray[i][k] = 1;
        } 
    }
    updateVelocities(velocitiesX,velocitiesY,0.0,-9.81,0.0333); /* DO NOT USE FRACTIONS 1/2, they do not work as floats */
    for(int i = 0;i<51;i++)
    {
        for(int k = 0;k<51;k++)
        {
            printf("%f\n",velocitiesY[i][k]);
        }
    }
    velocitiesY[3][3] = 1.3;
    accountForIncompressibility(velocitiesX,velocitiesY,cellTypeArray);

    for(int i = 1;i<50;i++)
    {
        for(int k = 1;k<50;k++)
        {
            printf("After incompressibility condition %f\n",velocitiesY[i][k]);
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