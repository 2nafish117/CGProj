#pragma once

#include "OpenGLDependencies.h"
#include "Util.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum Action
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	SPRINT,
	WALK
};

const static float SPRINT_SPEED	= 20.0;
const static float WALK_SPEED	= 1.0;
const static float NORMAL_SPEED	= 5.0;
const static float MIN_FOV		= 20.0;
const static float DEF_FOV		= 60.0;
const static float MAX_FOV		= 360.0;
const static float ZOOM_SPEED	= 2.0;
const static float SENSITIVITY	= 0.05;
const static float MIN_PITCH	= -89.0;
const static float MAX_PITCH	= 89.0;
const static float MIN_YAW		= -180.0;
const static float MAX_YAW		= 180.0;
const static float ASPECT_RATIO	= 16.0 / 9.0;
const static float MAX_Z		= 200.0;
const static float MIN_Z		= 0.1;

class Camera
{
public:
	glm::vec3 mPos;
	glm::vec3 mFront, mUp, mRight; // front actually points towards the camera 
								   // also keep these normalised at all times
	glm::vec3 mWorldUp;
	float mYaw, mPitch;
	float mFov;
	float mAspectRatio;
	float mZNear, mZFar;
	float mCameraSpeed;
	float mSensitivity;

public:
	Camera(
		const glm::vec3& pos	= glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& front	= glm::vec3(0.0f, 0.0f, -1.0f),
		float yaw				= -90.0f, float pitch		= 0.0f,
		float fov				= DEF_FOV, float aspectRatio= ASPECT_RATIO
	)
		: mPos(pos), mFront(front), mWorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		mYaw(yaw), mPitch(pitch), 
		mFov(fov), mAspectRatio(aspectRatio),
		mZNear(MIN_Z), mZFar(MAX_Z), mCameraSpeed(NORMAL_SPEED), mSensitivity(SENSITIVITY)
	{
		updateCameraVectors();
	}

	~Camera() {  }

	inline glm::mat4 getViewMatrix() const { return glm::lookAt(mPos, mPos + mFront, mUp); }
	inline glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(mFov), mAspectRatio, mZNear, mZFar); }
	inline glm::mat4 getViewProjectionMatrix() const { return getProjectionMatrix() * getViewMatrix(); }

	void updateCameraVectors();

	void handleKeyboardInput(Action dir, bool pressed, float dt);
	void handleMouseButtonInput(int left, int right, int middle, float dt);
	void handleMouseMoveInput(double mouseXpos, double mouseYpos, float deltaTime);
	static void handleMouseScrollEvent(GLFWwindow* win, double x, double y);
private:
	double lastMouseX = 0.0;
	double lastMouseY = 0.0;
};