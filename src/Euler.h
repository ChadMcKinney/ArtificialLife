#ifndef EULER_H
#define EULER_H

#include "Math.h"
#include "cinder/Quaternion.h"
#include "cinder/Vector.h"

namespace geometry {

class Euler // EVERYTHING IS IN RADIANS!
{
public:

    // Constructor which takes yaw, pitch and roll values as reals (radians).
    Euler(double y = 0.0f, double p = 0.0f, double r = 0.0f) :
        mYaw(y), mPitch(p), mRoll(r), mChanged(true), mCachedQuaternion(0, 0, 0, 0)
    {

    }

    // Get the Yaw angle.
    inline double getYaw()
    {
        return mYaw;
    }

    // Get the Pitch angle.
    inline double getPitch()
    {
        return mPitch;
    }

    // Get the Roll angle.
    inline double getRoll()
    {
        return mRoll;
    }

    // Set the yaw.
    inline Euler &setYaw(double y)
    {
        mYaw = y;
        mChanged = true;
        return *this;
    }

    // Set the pitch.
    inline Euler &setPitch(double p)
    {
        mPitch = p;
        mChanged = true;
        return *this;
    }

    // Set the roll.
    inline Euler &setRoll(double r)
    {
        mRoll = r;
        mChanged = true;
        return *this;
    }

    // Apply a relative yaw. (Adds angle to current yaw)
    inline Euler &yaw(double y)
    {
        mYaw += y;
        mChanged = true;
        return *this;
    }

    // Apply a relative pitch. (Adds angle to current pitch)
    inline Euler &pitch(double p)
    {
        mPitch += p;
        mChanged = true;
        return *this;
    }

    // Apply a relative roll. (Adds angle to current roll)
    inline Euler &roll(double r)
    {
        mRoll += r;
        mChanged = true;
        return *this;
    }

    // Get a vector pointing forwards.
    inline cinder::Vec3d getForward()
    {
        return toQuaternion() * -cinder::Vec3d::zAxis();
    }

    // Get a vector pointing to the right.
    inline cinder::Vec3d getRight()
    {
        return toQuaternion() * cinder::Vec3d::xAxis();
    }

    // Get a vector pointing up.
    inline cinder::Vec3d getUp()
    {
        return toQuaternion() * cinder::Vec3d::yAxis();
    }

    // Calculate the quaternion of the euler object.
    // The result is cached, it is only recalculated when the component euler angles are changed.
    inline cinder::Quatd toQuaternion()
    {
        if(mChanged)
        {
            mCachedQuaternion = cinder::Quatd(cinder::Vec3d::yAxis(), mYaw) * cinder::Quatd(cinder::Vec3d::xAxis(), mPitch)
                    * cinder::Quatd(cinder::Vec3d::zAxis(), mRoll);
            mChanged = false;
        }
        return mCachedQuaternion;
    }

    // Casting operator. This allows any ogre function that wants a Quaternion to accept a Euler instead.
    inline operator cinder::Quatd()
    {
        return toQuaternion();
    }

    // Set the yaw and pitch to face in the given direction.
    // The direction doesn't need to be normalised.
    // setYaw - if false, the yaw isn't changed.
    // setPitch - if false, the pitch isn't changed.
    // Roll is always unaffected.
    Euler &setDirection(const cinder::Vec3d &v, bool setYaw = true, bool setPitch = true)
    {
        cinder::Vec3d d(v.normalized());
        if(setPitch)
            mPitch = sin(d.y);
        if(setYaw)
            mYaw = atan2(-d.x, -d.z);//+alife::PI/2.0;
        mChanged = setYaw||setPitch;

        return *this;
    }

    // Normalise the selected rotations to be within the +/-180 degree range.
    // The normalise uses a wrap around, so for example a yaw of 360 degrees becomes 0 degrees,
    // and -190 degrees becomes 170.
    // normYaw, normPitch, normRoll - only angles set to true are normalised.
    inline void normalise(bool normYaw = true, bool normPitch = true, bool normRoll = true)
    {
        if(normYaw)
        {
            double yaw = mYaw;
            if(yaw < -alife::PI)
            {
                yaw = fmod(yaw, alife::TWO_PI);
                if(yaw < -alife::PI)
                {
                    yaw += alife::TWO_PI;
                }
                mYaw = yaw;
                mChanged = true;
            }

            else if(yaw > alife::PI)
            {
                yaw = fmod(yaw, alife::TWO_PI);
                if(yaw > alife::PI)
                {
                    yaw -= alife::TWO_PI;
                }
                mYaw = yaw;
                mChanged = true;
            }
        }

        if(normPitch)
        {
            double pitch = mPitch;
            if(pitch < -alife::PI)
            {
                pitch = fmod(pitch, alife::TWO_PI);
                if(pitch < -alife::PI)
                {
                    pitch += alife::TWO_PI;
                }
                mPitch = pitch;
                mChanged = true;
            }
            else if(pitch > alife::PI)
            {
                pitch = fmod(pitch, alife::TWO_PI);
                if(pitch > alife::PI)
                {
                    pitch -= alife::TWO_PI;
                }
                mPitch = pitch;
                mChanged = true;
            }
        }

        if(normRoll)
        {
            double roll= mRoll;
            if(roll < -alife::PI)
            {
                roll = fmod(roll, alife::TWO_PI);
                if(roll < -alife::PI)
                {
                    roll += alife::TWO_PI;
                }
                mRoll = roll;
                mChanged = true;
            }
            else if(roll > alife::PI)
            {
                roll = fmod(roll, alife::TWO_PI);
                if(roll > alife::PI)
                {
                    roll -= alife::TWO_PI;
                }
                mRoll = roll;
                mChanged = true;
            }
        }
    }

    // Return the relative euler angles required to rotate from the current forward direction to the specified dir vector.
    // The result euler can then be added to the current euler to immediately face dir.
    // setYaw, setPitch - only the angles set to true are calculated. If false, the angle is set to 0.
    // shortest - if false, the full value of each angle is used. If true, the angles are normalised and the shortest
    // rotation is found to face the correct direction. For example, when false a yaw of 1000 degrees and a dir of
    // (0,0,-1) will return a -1000 degree yaw. When true, the same yaw and dir would give 80 degrees (1080 degrees faces
    // the same way as (0,0,-1).
    // The rotation won't flip upside down then roll instead of a 180 degree yaw.
    inline Euler getRotationTo(const cinder::Vec3d &dir, bool setYaw = true, bool setPitch = true, bool shortest = true)
    {
        Euler t1;
        Euler t2;
        t1.setDirection(dir, setYaw, setPitch);
        t2=t1-*this;
        if(shortest && setYaw)
        {
            t2.normalise();
        }
        return t2;
    }

    // Clamp the yaw angle to a range of +/-limit.
    inline Euler &limitYaw(const double &limit)
    {
        if(mYaw > limit)
        {
            mYaw = limit;
            mChanged = true;
        }
        else if(mYaw < -limit)
        {
            mYaw = -limit;
            mChanged = true;
        }
        return *this;
    }

    // Clamp the pitch angle to a range of +/-limit.
    inline Euler &limitPitch(const double &limit)
    {
        if(mPitch > limit)
        {
            mPitch = limit;
            mChanged = true;
        }
        else if(mPitch < -limit)
        {
            mPitch = -limit;
            mChanged = true;
        }
        return *this;
    }

    // Clamp the roll angle to a range of +/-limit.
    inline Euler &limitRoll(const double &limit)
    {
        if(mRoll > limit)
        {
            mRoll = limit;
            mChanged = true;
        }
        else if(mRoll < -limit)
        {
            mRoll = -limit;
            mChanged = true;
        }
        return *this;
    }

    // stream operator, for printing the euler component angles to a stream
    inline friend std::ostream &operator<<(std::ostream &o, const Euler &e)
    {
        o << "<Y:" << e.mYaw << ", P:" << e.mPitch << ", R:" << e.mRoll << ">";
        return o;
    }

    // Add two euler objects.
    inline Euler operator+(const Euler &rhs) const
    {
        return Euler(mYaw + rhs.mYaw, mPitch + rhs.mPitch, mRoll + rhs.mRoll);
    }

    // Subtract two euler objects. This finds the difference as relative angles.
    inline Euler operator-(const Euler &rhs) const
    {
        return Euler(mYaw - rhs.mYaw, mPitch - rhs.mPitch, mRoll - rhs.mRoll);
    }

    // Interpolate the euler angles by rhs.
    inline Euler operator*(double rhs) const
    {
        return Euler(mYaw * rhs, mPitch * rhs, mRoll * rhs);
    }

    // Interpolate the euler angle by lhs.
    inline friend Euler operator*(double lhs, const Euler &rhs)
    {
        return Euler(lhs * rhs.mYaw, lhs * rhs.mPitch, lhs * rhs.mRoll);
    }

    // Multiply two eulers. This has the same effect as multiplying quaternions.
    // The result is a quaternion.
    inline cinder::Quatd operator*(const Euler &rhs) const
    {
        Euler e1(*this), e2(rhs);
        return e1.toQuaternion()*e2.toQuaternion();
    }

    // Apply the euler rotation to the vector rhs.
    inline cinder::Vec3d operator*(const cinder::Vec3d &rhs)
    {
        return toQuaternion() * rhs;
    }

protected:
    double mYaw;							// Rotation around the Y axis.
    double mPitch;							// Rotation around the X axis.
    double mRoll;							// Rotation around the Z axis.
    cinder::Quatd mCachedQuaternion;   // Cached quaternion equivalent of this euler object.
    bool mChanged;						    // Is the cached quaternion out of date?
};

} // geometry namespace

#endif // EULER_H
