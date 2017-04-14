//
//  camera.h
//  glutCpp
//
//  Created by Ruben Cuadra on 2/10/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//  Tomado del repo de: Ryan Pridgeon

#pragma once

class Camera
{
private:
	// a vector pointing in the directio nyoure facing
	float forwardVec[3];

	// a vector pointing to the right of where your facing (to describe orientation
	float rightVec[3];

	// a vector pointing upwards from where youre facing
	float upVec[3];

	// a vector describing the position of the camera
	float position[3];

	// the camera speed
	float cameraSpeed;
	float cameraTurnSpeed;

public:
	Camera(void);

	// transform the opengl view matrix for the orientation
	void transformOrientation(void);

	// transform the opoengl view matrix for the translation
	void transformTranslation(void);

	// points the camera at the given point in 3d space
	void pointAt(float* targetVec);

	// speed up the camera speed
	void speedUp(void);

	// slow down the camera speed
	void slowDown(void);

	// move the camera forward
	void forward(void);

	// strafe left
	void left(void);

	// strafe right
	void right(void);

	// move the camera backward
	void backward(void);

	// roll the camera to the right
	void rollRight(void);

	// roll the camera to the left
	void rollLeft(void);

	// pitch the camera up
	void pitchUp(void);

	// pitch the camera down
	void pitchDown(void);

	// yaw left
	void yawLeft(void);

	// yaw right
	void yawRight(void);
    
    float getSpeed();
    float getTurnSpeed();
    float getPositionX();
    float getPositionY();
    float getPositionZ();
};
