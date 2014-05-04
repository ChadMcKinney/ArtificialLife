#pragma once

// System includes
#include <iostream>


// Local includes
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Vector.h"
#include "cinder/Sphere.h"

class AICamera {

public:	

    AICamera(cinder::Vec3f pos);
    ~AICamera();
	
	void keyboardInput(bool w, bool a, bool s, bool d, bool q, bool e, bool shift);
	// provides flying fps style camera rotation based on a direction vector
	void mouseInput(cinder::Vec2f mouseVec);
    void moveBy(cinder::Vec3f mov);
    void moveTo(cinder::Vec3f pos);
	void update();
	void setup(float windowAspectRatio);
    const cinder::CameraPersp& getCam();
	cinder::Quatf getRotation();
    cinder::Vec3f getEye();
    cinder::Vec3f getRotationVector();
    bool canView(cinder::Vec3f point);
    cinder::Sphere frustumSphere;

private:
	
	void updateViewMatrix();
    void addToCameraPosition(cinder::Vec3f vectorToAdd);
	void pitchCamera(float pitch);
	void yawCamera(float yaw);
	void rollCamera(float roll);

	cinder::CameraPersp mCam;
    cinder::Vec3f mEye, mTarget, mUp, mRight, mForward;
	float mYaw, mPitch, mRoll, mRotationSpeed, mMoveSpeed, mMinMoveSpeed, mMaxMoveSpeed;
    cinder::Quatf mCameraRotation;
};
