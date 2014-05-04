#include "Flocking.h"

using namespace alife::cell;
using namespace geometry;

namespace alife {

////////////////
// Boid
////////////////

double Boid::desiredSeparation = 50.0;
double Boid::neighborDist = 100.0;

Boid::Boid(int id, Vec3d location, Vec3i borderMin, Vec3i borderMax, bool borderWrapping, bool borderRepulsion,
           Vec3d acceleration, Vec3d velocity, double maxSpeed, double maxForce) :
    id(id),
    location(location),
    borderMin(borderMin),
    borderMax(borderMax),
    borderWrapping(borderWrapping),
    borderRepulsion(borderRepulsion),
    acceleration(acceleration),
    velocity(velocity),
    maxSpeed(maxSpeed),
    maxForce(maxForce)
{
    Vec3i center = borderMin + ((borderMax - borderMin) / 2);
    borderCenter = Vec3d(center.x, center.y, center.z);
}

void Boid::run(const std::vector<Boid*>& boids)
{
    flock(boids);
    update();

    if(borderWrapping)
        wrapToBorders();

    if(borderRepulsion)
        borderRepulse();
}

void Boid::applyForce(Vec3d force)
{
    acceleration = acceleration + force;
}

void Boid::update()
{
    velocity = velocity + acceleration;
    velocity.limit(maxSpeed);
    location = location + velocity;
    acceleration = acceleration * 0;
}

geometry::Vec3d Boid::steer(geometry::Vec3d target)
{
    Vec3d desired = target - location;
    double distance = desired.length();

    if(distance > 0)
    {
        desired.normalize();
        desired = desired * maxSpeed;
        Vec3d steer = desired - velocity;
        steer.limit(maxForce);
        return steer;
    }

    else
    {
        return Vec3d(0, 0, 0);
    }
}

void Boid::seek(Vec3d target)
{
    acceleration += steer(target);
}

geometry::Vec3d Boid::separate(const std::vector<Boid*>& boids)
{
    Vec3d steer(0, 0, 0);
    int count = 0;

    for(int i = 0; i < boids.size(); ++i)
    {
        Boid* boid = boids[i];
        double distance = location.distance(boid->getLocation());

        if(distance > 0 && distance < desiredSeparation)
        {
            Vec3d difference = location - boid->getLocation();
            difference.normalize();
            difference = difference / distance;
            steer = steer + difference;
            ++count;
        }
    }

    if(count > 0)
    {
        steer = steer / (double) count; // Average
    }

    if(steer.length() > 0)
    {
        steer.normalize();
        steer = steer * maxSpeed;
        steer = steer - velocity;
        steer.limit(maxForce);
    }

    return steer;
}

void Boid::flock(const std::vector<Boid*>& boids)
{
    Vec3d separationForce = separate(boids) * 1.5; // Emphasize separation over the other two forces
    Vec3d alignForce = align(boids);
    Vec3d cohesionForce = cohesion(boids);

    applyForce(separationForce);
    applyForce(alignForce);
    applyForce(cohesionForce);
}

geometry::Vec3d Boid::align(const std::vector<Boid*>& boids)
{
    Vec3d sum(0, 0, 0);
    int count = 0;

    for(int i = 0; i < boids.size(); ++i)
    {
        Boid* boid = boids[i];
        double distance = location.distance(boid->getLocation());

        if(distance > 0 && distance < neighborDist)
        {
            sum = sum + boid->getVelocity();
            ++count;
        }
    }

    if(count > 0)
    {
        sum = sum / (double) count;
        //sum.normalize();
        //sum = sum * maxSpeed;
        //Vec3d steer = sum - velocity;
        //steer.limit(maxForce);
        return sum;
    }

    else
    {
        return Vec3d(0, 0, 0);
    }
}

geometry::Vec3d Boid::cohesion(const std::vector<Boid*>& boids)
{
    Vec3d sum(0, 0, 0);
    int count = 0;

    for(int i = 0; i < boids.size(); ++i)
    {
        Boid* boid = boids[i];
        double distance = location.distance(boid->getLocation());
        if(distance > 0 && distance < neighborDist)
        {
            sum = sum + boid->getLocation();
            ++count;
        }
    }

    if(count > 0)
    {
        sum = sum / (double) count;
        return steer(sum);
    }

    else
    {
        return Vec3d(0, 0, 0);
    }
}

void Boid::drift()
{
    double radius = 30.0;
    double diameter = 60.0;
    int driftBounds = 0.45;
    driftAngle += randomRange<int>(-driftBounds, driftBounds);

    Vec3d center = velocity;
    center.normalize();
    center *= diameter;
    center += location;

    Vec3d offset(radius * cos(driftAngle), radius * sin(driftAngle));
    Vec3d target = center + offset;
    acceleration += steer(target);
}

void Boid::wrapToBorders()
{
    // Wrap x coordinate
    if(location.x >= borderMax.x)
        location.x = borderMin.x;

    else if(location.x < borderMin.x)
        location.x = borderMax.x -1;

    // Wrap y coordinate
    if(location.y >= borderMax.y)
        location.y = borderMin.y;

    else if(location.y < borderMin.y)
        location.y = borderMax.y -1;

    // Wrap z coordinate
    if(location.z >= borderMax.z)
        location.z = borderMin.z;

    else if(location.z < borderMin.z)
        location.z = borderMax.z -1;
}

void Boid::borderRepulse()
{
    if(location.x < borderMin.x || location.x > borderMax.x
            || location.y < borderMin.y || location.y > borderMax.y
            || location.z < borderMin.z || location.z > borderMax.z)
    {
        seek(borderCenter);
    }
}

bool Boid::getBorderWrapping()
{
    return borderWrapping;
}

void Boid::setBorderWrapping(bool borderWrapping)
{
    this->borderWrapping = borderWrapping;
}

int Boid::getID()
{
    return id;
}

Vec3d Boid::getLocation()
{
    return location;
}

geometry::Vec3d Boid::getVelocity()
{
    return velocity;
}

geometry::Vec3d Boid::getAcceleration()
{
    return acceleration;
}

double Boid::getMaxForce()
{
    return maxForce;
}

double Boid::getMaxSpeed()
{
    return maxSpeed;
}

////////////////
// Flock
////////////////

Flock::Flock(Vec3i borderMin, Vec3i borderMax, bool borderWrapping, bool borderRepulsion):
    borderMin(borderMin),
    borderMax(borderMax),
    borderWrapping(borderWrapping),
    borderRepulsion(borderRepulsion),
    boidIDCounter(0)
{

}

Flock::~Flock()
{
    clear();
}

void Flock::run()
{
    for(int i = 0; i < boids.size(); ++i)
    {
        boids[i]->run(boids);
    }
}

void Flock::seek(Vec3d target)
{
    for(int i = 0; i < boids.size(); ++i)
    {
        boids[i]->seek(target);
    }
}

void Flock::drift()
{
    for(int i = 0; i < boids.size(); ++i)
    {
        boids[i]->drift();
    }
}

int Flock::addBoid(Vec3d location, Vec3d acceleration, Vec3d velocity, double maxSpeed, double maxForce)
{
    boids.push_back(new Boid(boidIDCounter, location, borderMin, borderMax, borderWrapping, borderRepulsion, acceleration, velocity,
                               maxSpeed, maxForce));
    ++boidIDCounter;
    return boidIDCounter;
}

int Flock::addBoid(Boid *boid)
{
    boids.push_back(boid);
    ++boidIDCounter;
    return boidIDCounter;
}

void Flock::removeBoid(int id)
{
    for(int i = 0; i < boids.size(); ++i)
    {
        if(id == boids[i]->getID())
        {
            delete boids[i];
            boids.erase(boids.begin() + i);
        }
    }
}

void Flock::clear()
{
    for(int i = 0; i < boids.size(); ++i)
    {
        delete boids[i];
    }

    boids.clear();
}

const std::vector<Boid*>& Flock::getBoids()
{
    return boids;
}

bool Flock::getBorderWrapping()
{
    return borderWrapping;
}

void Flock::setBorderWrapping(bool borderWrapping)
{
    this->borderWrapping = borderWrapping;

    for(int i = 0; i < boids.size(); ++i)
    {
        boids[i]->setBorderWrapping(borderWrapping);
    }
}

int Flock::getNumberOfBoids()
{
    return boids.size();
}

int Flock::size()
{
    return boids.size();
}

} // alife namespace
