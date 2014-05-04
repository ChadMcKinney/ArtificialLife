#include "Array.h"

namespace geometry {

////////////
// Array2D
////////////

Array2D::Array2D(int width, int height, double initializedValue) :
    width(width),
    height(height)
{
    extents = Vec2i(width, height);
    initializeArray(initializedValue);
}

Array2D::Array2D(Vec2i extents, double initializedValue) :
    extents(extents)
{
    width = extents.x;
    height = extents.y;
    initializeArray(initializedValue);
}

void Array2D::fill(double value)
{
    for(int x = 0; x < width; ++x)
    {
        array.push_back(std::vector<double>());
        for(int y = 0; y < height; ++y)
        {
            array[x][y] = 0;
        }
    }
}

double Array2D::at(int x, int y)
{
    return array[x][y];
}

double Array2D::at(Vec2i indexes)
{
    return array[indexes.x][indexes.y];
}

double Array2D::operator ()(int x, int y)
{
    return array[x][y];
}

double Array2D::operator ()(Vec2i indexes)
{
    return array[indexes.x][indexes.y];
}

std::vector<double>& Array2D::operator [](int x)
{
    return array[x];
}

void Array2D::initializeArray(int initializedValue)
{
    for(int x = 0; x < width; ++x)
    {
        array.push_back(std::vector<double>());
        for(int y = 0; y < height; ++y)
        {
            array[x].push_back(initializedValue);
        }
    }
}


////////////
// Array3D
///////////

Array3D::Array3D(int width, int height, int depth, double initializedValue) :
    width(width),
    height(height),
    depth(depth)
{
    extents = Vec3i(width, height, depth);
    initializeArray(initializedValue);
}

Array3D::Array3D(Vec3i extents, double initializedValue) :
    extents(extents)
{
    width = extents.x;
    height = extents.y;
    depth = extents.z;
    initializeArray(initializedValue);
}

void Array3D::fill(double value)
{
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            for(int z = 0; z < depth; ++z)
            {
                array[x][y][z] = 0.0;
            }
        }
    }
}

double Array3D::at(int x, int y, int z)
{
    return array[x][y][z];
}

double Array3D::at(Vec3i indexes)
{
    return array[indexes.x][indexes.y][indexes.z];
}

double Array3D::operator ()(int x, int y, int z)
{
    return array[x][y][z];
}

double Array3D::operator ()(Vec3i indexes)
{
    return array[indexes.x][indexes.y][indexes.z];
}

std::vector< std::vector<double> >& Array3D::operator [](int x)
{
    return array[x];
}

void Array3D::initializeArray(int initializedValue)
{
    for(int x = 0; x < width; ++x)
    {
        array.push_back(std::vector<std::vector< double> >());
        for(int y = 0; y < height; ++y)
        {
            array[x].push_back(std::vector<double>());
            for(int z = 0; z < depth; ++z)
            {
                array[x][y].push_back(initializedValue);
            }
        }
    }
}

} // geometry namespace
