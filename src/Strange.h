#ifndef STRANGE_H
#define STRANGE_H

// ALife includes
#include "Vector.h"
#include "Array.h"

namespace alife {

///////////////////
// Fill Functions
///////////////////

// You will want to scale these up significantly or the range will fall very small, somewhere like -1 to 1.
// There is internal range checking for array access, but not much will be filled without scaling

void pickover2DFill(geometry::Array2D& array, int iterationFrames, int initializationFrames = 100, double scale = 1,
                    int initX = 1, int initY = 1, double a = -0.966918, double b = 2.879879, double c = 0.765145,
                    double d = 0.744728);

void pickover3DFill(geometry::Array3D& array, int iterationFrames, int initializationFrames = 100, double scale = 1,
                    int initX = 1, int initY = 1, int initZ = 1, double a = 2.24, double b = 0.43, double c = -0.65,
                    double d = -2.43);

////////////////////
// Pickover classes
////////////////////

class Pickover2D {

public:
    Pickover2D(bool saveHistory, int initializationFrames = 100, double initX = 0.01, double initY = 0.01,
               double a = -0.966918,  double b = 2.879879, double c = 0.765145, double d = 0.744728);

    void reset(int initializationFrames = 100);
    geometry::Vec2d update();
    const std::vector<geometry::Vec2d>& getHistory();

private:

    void initialize(int initializationFrames);

    double a, b, c, d, xn, yn, initX, initY;
    std::vector<geometry::Vec2d> history;
    bool saveHistory;
};

class Pickover3D {

public:
    Pickover3D(bool saveHistory, int initializationFrames = 100, double initX = 0.01, double initY = 0.01, double initZ = 0.01,
               double a = 2.24, double b = 0.43, double c = -0.65, double d = -2.43);

    void reset(int initializationFrames);
    geometry::Vec3d update();
    const std::vector<geometry::Vec3d>& getHistory();

private:

    void initialize(int initializationFrames);

    double a, b, c, d, xn, yn, zn, initX, initY, initZ;
    std::vector<geometry::Vec3d> history;
    bool saveHistory;
};

} // fractal namespace

#endif // STRANGE_H
