#ifndef ARRAY_H
#define ARRAY_H

// System includes
#include <vector>
#include <math.h>

// Fractal includes
#include "Vector.h"

namespace geometry {

// Array classes for holding double information in fixed array sizes and accessing them easily.

class Array2D {

public:

    Array2D(int width, int height, double initializedValue = 0);
    Array2D(Vec2i extents = Vec2i(0, 0), double initializedValue = 0);

    void fill(double value);
    double at(int x, int y);
    double at(Vec2i indexes);
    double operator ()(int x, int y); // Ex: Array2D a2d(Vec2i(3, 3)); a2d(2, 2);
    double operator ()(Vec2i indexes); // Ex: Array2D a2d(Vec2i(3, 3)); a2d(Vec2i(2, 2));
    std::vector<double>& operator [](int x); // To Allow for [x][y] type indexing. Ex: Array2D a2d(Vec2i(3, 3)); a2d[2][2];

    Vec2i extents;
    int width, height;
    std::vector< std::vector<double> > array;

private:

    void initializeArray(int initializedValue);
};

class Array3D {

public:

    Array3D(int width, int height, int depth, double initializedValue = 0);
    Array3D(Vec3i extents = Vec3i(0, 0, 0), double initializedValue = 0);

    void fill(double value);
    double at(int x, int y, int z);
    double at(Vec3i indexes);
    double operator ()(int x, int y, int z); // Ex: Array3D a3d(Vec3i(3, 3, 3)); a3d(2, 2, 2);
    double operator ()(Vec3i indexes); // Ex: Array3D a3d(Vec3i(3, 3, 3)); a3d(Vec3i(2, 2, 2));

    // To Allow for [x][y][z] type indexing. Ex: Array3D a3d(Vec3i(3, 3, 3)); a2d[2][2][2];
    std::vector< std::vector<double> >& operator [](int x);

    Vec3i extents;
    int width, height, depth;
    std::vector< std::vector< std::vector<double> > > array;

private:

    void initializeArray(int initializedValue);
};

/* If I ever need this I'll continue to work on it. For now, just this is just a note for possible future development
class HyperArray { // A class for containing data in an arbitrary number of dimensions

public:

    HyperArray(std::vector<int> extents, double initialzedValue = 0);

    void fill(double value);
    double at(int x, ...);
    double at(std::vector<int> indexes);
    double operator ()(int x, ...);
    double operator ()(std::vector<int> indexes);

    std::vector<int> extents;
    int dimensions;

    union data {
        HyperArray* h;
        double d;
    };

    std::vector<data> array;
};*/

} // geometry namespace

#endif // ARRAY_H
