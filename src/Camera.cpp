// System includes
#include <iostream>

// Local includes
#include "src/Camera.h"

using namespace cinder;
using namespace gl;

AICamera::AICamera(Vec3f pos) :
	mEye(pos),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mRotationSpeed(0.0015f),
    mMoveSpeed(20.0f),
	mMinMoveSpeed(15.0f),
    mMaxMoveSpeed(60.0f)
{
	
}

AICamera::~AICamera()
{

}

void AICamera::keyboardInput(bool w, bool a, bool s, bool d, bool q, bool e, bool shift)
{
    Vec3f moveVector(0, 0, 0);
	
	if(w) 
	{
        moveVector += Vec3f(0, 0, -1);
	}
	
	if(a) 
	{
        moveVector += Vec3f(-1, 0, 0);
	}
	
	if(s) 
	{
        moveVector += Vec3f(0, 0, 1);
	}
	
	if(d) 
	{
        moveVector += Vec3f(1, 0, 0);
	}
	
	if(q)
	{
		rollCamera(0.03f);
	}
	
	else if(e)
	{
		rollCamera(-0.03f);
	}

	
	if(w || a || s || d || q || e)
	{
		addToCameraPosition(moveVector);
	}
}

void AICamera::mouseInput(Vec2f mouseVec)
{
    mYaw = mRotationSpeed * mouseVec.x * -1;
    mPitch = mRotationSpeed * mouseVec.y * -1;

    pitchCamera(mPitch);
    yawCamera(mYaw);
	updateViewMatrix();
    mYaw = 0;
    mPitch = 0;
}

void AICamera::moveBy(Vec3f mov)
{

}

void AICamera::moveTo(Vec3f pos)
{

}

void AICamera::update()
{
	updateViewMatrix();
    frustumSphere = cinder::Sphere(mCam.getCenterOfInterestPoint(), mEye.distance(mCam.getCenterOfInterestPoint()) * 0.7);
}

void AICamera::setup(float windowAspectRatio)
{
	GLfloat distance = 100000.0f;
    mCam.setPerspective(60.0f, windowAspectRatio, 0.1f, distance);
    mUp = Vec3f::yAxis();
    mRight = Vec3f::xAxis();
    mForward = Vec3f::zAxis();
    update();
}

const CameraPersp& AICamera::getCam()
{
	return mCam;
}

cinder::Quatf AICamera::getRotation()
{
	return mCameraRotation;
}

cinder::Vec3f AICamera::getRotationVector()
{
    return mCam.getViewDirection();
}

bool AICamera::canView(Vec3f point)
{
    return frustumSphere.intersects(cinder::Ray(mEye, (point - mEye).normalized()));
}

cinder::Vec3f AICamera::getEye()
{
	return mEye;
}

void AICamera::updateViewMatrix()
{
    Vec3f cameraOriginalTarget = Vec3f(0, 0, -1);
    Vec3f cameraRotatedTarget = mCameraRotation * cameraOriginalTarget;
    Vec3f cameraFinalTarget = mEye + cameraRotatedTarget;
    mCam.lookAt(mEye, cameraFinalTarget, mUp);
}

void AICamera::addToCameraPosition(Vec3f vectorToAdd)
{
    Vec3f rotatedVector = mCameraRotation * vectorToAdd;
    mEye += mMoveSpeed * rotatedVector;
}

void AICamera::pitchCamera(float pitch)
{
    mCameraRotation *= Quatf(mRight, pitch).normalized();
    mUp = Vec3f::yAxis() * mCameraRotation;
    mForward = Vec3f::zAxis() * mCameraRotation;
}

void AICamera::yawCamera(float yaw)
{
    mCameraRotation *= Quatf(mUp, yaw).normalized();
    mRight = Vec3f::xAxis() * mCameraRotation;
    mForward = Vec3f::zAxis() * mCameraRotation;
}

void AICamera::rollCamera(float roll)
{
    mCameraRotation *= Quatf(mForward, roll).normalized();
    mUp = Vec3f::yAxis() * mCameraRotation;
    mRight = Vec3f::xAxis() * mCameraRotation;
}
