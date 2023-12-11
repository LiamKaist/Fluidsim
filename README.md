# First step 

Using CImg library for plotting the simulation
Define the borders of the simulation plot
As defined in the CImg constructor, the simulation will take place in a 500 by 500 pixel grid 

Investigating a way to change the pixel values by hand, find a way to update the frame periodically and in a clean way

# Second step

Creating the simulation using https://cg.informatik.uni-freiburg.de/intern/seminar/gridFluids_fluid-EulerParticle.pdf
as a base

Initialisation and parameters associated with each cell 

For generating n frames :

1. Initialize Grid with some Fluid
2. for( i from 1 to n )
Let t = 0.0
While t < tf rame
Calculate ∆t
Advect Fluid
Pressure Projection (Pressure Solve)
Advect Free Surface
t = t + ∆t
Write frame i to disk

December 10 2023 :

I have a grid of 50 by 50 cells, velocities are however not stored in the centers of the cells , but on the sides of the cells.
Which means there will be (50 + 1 velocities ) by (50 + 1 velocities) stored in an array

11/12/2023

First , create a table containing all the velocities of the staggered grid
There are 50 by 50 cells, hence 51 by 51 velocities to store due to the staggered grid. (Why use a staggered grid? This should be understood)

