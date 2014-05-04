#ifndef FLOCKING_H
#define FLOCKING_H

// System includes
#include <vector>

// Local includes
#include "Vector.h"
#include "abstractca.h"

namespace alife {

class Boid {

public:

    Boid(int id, geometry::Vec3d location, geometry::Vec3i borderMin, geometry::Vec3i borderMax, bool borderWrapping, bool borderRepulsion,
         geometry::Vec3d acceleration, geometry::Vec3d velocity, double maxSpeed, double maxForce);

    void run(const std::vector<Boid*>& boids);
    void applyForce(geometry::Vec3d force);
    void update();
    geometry::Vec3d steer(geometry::Vec3d target);
    void seek(geometry::Vec3d target);
    geometry::Vec3d separate(const std::vector<Boid*>& boids);
    void flock(const std::vector<Boid*>& boids);
    geometry::Vec3d align(const std::vector<Boid*>& boids);
    geometry::Vec3d cohesion(const std::vector<Boid*>& boids);
    void drift();
    geometry::Vec3d steer();

    void wrapToBorders();
    void borderRepulse();
    bool getBorderWrapping();
    void setBorderWrapping(bool borderWrapping);
    int getID();
    geometry::Vec3d getLocation();
    geometry::Vec3d getVelocity();
    geometry::Vec3d getAcceleration();
    double getMaxForce();
    double getMaxSpeed();

    static double desiredSeparation;
    static double neighborDist;

private:

    geometry::Vec3i borderMin, borderMax;
    bool borderWrapping, borderRepulsion;

    geometry::Vec3d location;
    geometry::Vec3d velocity;
    geometry::Vec3d acceleration;
    geometry::Vec3d target;
    geometry::Vec3d borderCenter;

    double driftAngle;
    double maxForce;    // Maximum steering force
    double maxSpeed;    // Maximum speed

    int id;
};

class Flock {

public:

    Flock(geometry::Vec3i borderMin = geometry::Vec3i(), geometry::Vec3i borderMax = geometry::Vec3i(),
          bool borderWrapping = false, bool borderRepulsion = false);
    ~Flock();

    void run();
    void seek(geometry::Vec3d target);
    void drift();

    // Returns the boid id for the created boid
    int addBoid(geometry::Vec3d location, geometry::Vec3d acceleration = geometry::Vec3d(0,0,0),
                 geometry::Vec3d velocity = geometry::Vec3d(cell::randomRange<int>(-100, 100) / 40.0,
                                                          cell::randomRange<int>(-100, 100) / 40.0,
                                                          cell::randomRange<int>(-100, 100) / 40.0 + 0.01),
                 double maxSpeed = 5.0, double maxForce = 0.75);

    // Flock takes ownership of the the item and will free the memory upon removal or on the Flocks deconstructor
    int addBoid(Boid* boid);
    void removeBoid(int id);
    void clear();
    const std::vector<Boid*>& getBoids();
    bool getBorderWrapping();
    void setBorderWrapping(bool borderWrapping);
    int getNumberOfBoids();
    int size();

private:

    std::vector<Boid*> boids;
    geometry::Vec3i borderMin, borderMax;
    bool borderWrapping;
    bool borderRepulsion;
    int boidIDCounter;
};

} // alife namespace


#endif // FLOCKING_H
