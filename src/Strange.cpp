// System includes
#include <math.h>
#include <iostream>

// alife includes
#include "Strange.h"

using namespace geometry;

namespace alife {

//////////////////
// Fill Functions
//////////////////

void pickover2DFill(Array2D &array, int iterationFrames, int initializationFrames, double scale, int initX, int initY,
                double a, double b, double c, double d)
{
    // Initialize the coordinates
    double xn = initX;
    double yn = initY;

    // Update initial conditions
    for(int i = 0; i < initializationFrames; ++i)
    {
        double xnew = sin(yn*b) + c*sin(xn*b);
        double ynew = sin(xn*a) + d*sin(yn*a);
        xn = xnew;
        yn = ynew;
    }

    // Iteratre through the frames
    for(int i = 0; i < iterationFrames; ++i)
    {
        // compute a new point using the strange attractor equations
        double xnew = sin(yn*b) + c*sin(xn*b);
        double ynew = sin(xn*a) + d*sin(yn*a);

        // save the new point
        xn = xnew;
        yn = ynew;

        double xMod = (xn * scale) + (array.width / 2);
        double yMod = (yn * scale) + (array.height / 2);

        if(xMod < array.width && xMod >= 0
                && yMod < array.height && yMod >= 0) // If within the array bounds
        {
            array[(int) xMod][(int) yMod] = 1; // Write it to the matrix
        }
    }
}

void pickover3DFill(Array3D &array, int iterationFrames, int initializationFrames, double scale,
                int initX, int initY, int initZ, double a, double b, double c, double d)
{
    // Initialize the coordinates
    double xn = initX;
    double yn = initY;
    double zn = initZ;

    // Update initial conditions
    for (int i = 0; i < initializationFrames; ++i) {

        double xnew = sin(a*yn) - zn*cos(b*xn);
        double ynew = zn*sin(c*xn) - cos(d*yn);
        double znew = sin(xn);

        xn = xnew;
        yn = ynew;
        zn = znew;
    }

    // Iterate through the frames
    for(int i = 0; i < iterationFrames; ++i)
    {
        double xnew = sin(a*yn) - zn*cos(b*xn);
        double ynew = zn*sin(c*xn) - cos(d*yn);
        double znew = sin(xn);

        // Save the new point
        xn = xnew;
        yn = ynew;
        zn = znew;

        double xMod = (xn * scale) + (array.width / 2);
        double yMod = (yn * scale) + (array.height / 2);
        double zMod = (zn * scale) + (array.depth / 2);

        if(xMod < array.width && xMod >= 0
                && yMod < array.height && yMod >= 0
                && zMod < array.depth && zMod >= 0) // If within the array bounds
        {
            array[(int) xMod][(int) yMod][(int) zMod] = 1; // Write it to the matrix
        }
    }
}

////////////////////
// Pickover Classes
////////////////////

// Pickover2D
Pickover2D::Pickover2D(bool saveHistory, int initializationFrames, double initX, double initY, double a, double b, double c,
                       double d) :
    saveHistory(saveHistory), xn(initX), yn(initY), initX(initX), initY(initY), a(a), b(b), c(c), d(d)
{
    initialize(initializationFrames);
}

void Pickover2D::reset(int initializationFrames)
{
    xn = initX;
    yn = initY;
    history.clear();
    initialize(initializationFrames);
}

Vec2d Pickover2D::update()
{
    // compute a new point using the strange attractor equations
    double xnew = sin(yn*b) + c*sin(xn*b);
    double ynew = sin(xn*a) + d*sin(yn*a);

    // save the new point
    xn = xnew;
    yn = ynew;

    Vec2d vec(xn, yn);

    if(saveHistory)
        history.push_back(vec);

    return vec;
}

const std::vector<Vec2d>& Pickover2D::getHistory()
{
    return history;
}

void Pickover2D::initialize(int initializationFrames)
{
    for(int i = 0; i < initializationFrames; ++i)
    {
        update();
    }

    if(saveHistory)
        history.clear();
}


// Pickover3D
Pickover3D::Pickover3D(bool saveHistory, int initializationFrames, double initX, double initY, double initZ, double a, double b,
                       double c, double d) :
    saveHistory(saveHistory), xn(initX), yn(initY), zn(initZ), initX(initX), initY(initY), initZ(initZ), a(a), b(b), c(c), d(d)
{
    initialize(initializationFrames);
}

void Pickover3D::reset(int initializationFrames)
{
    xn = initX;
    yn = initY;
    zn = initZ;
    history.clear();
    initialize(initializationFrames);
}

Vec3d Pickover3D::update()
{
    // compute a new point using the strange attractor equations
    double xnew = sin(a*yn) - zn*cos(b*xn);
    double ynew = zn*sin(c*xn) - cos(d*yn);
    double znew = sin(xn);

    // save the new point
    xn = xnew;
    yn = ynew;
    zn = znew;


    Vec3d vec(xn, yn, zn);

    if(saveHistory)
        history.push_back(vec);

    return vec;
}

const std::vector<Vec3d>& Pickover3D::getHistory()
{
    return history;
}

void Pickover3D::initialize(int initializationFrames)
{
    for(int i = 0; i < initializationFrames; ++i)
    {
        update();
    }

    if(saveHistory)
        history.clear();
}

} // alife
