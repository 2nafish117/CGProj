#include "Camera.h"

#include <tuple>
#include <iostream>
// glm::vec3 Camera::mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

void Camera::updateCameraVectors()
{
	static glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}

void Camera::handleKeyboardInput(Action act, bool pressed, float dt)
{
	if (act == FRONT)
		mPos += mFront * mCameraSpeed * dt;
	if (act == BACK)
		mPos -= mFront * mCameraSpeed * dt;
	if (act == RIGHT)
		mPos += mRight * mCameraSpeed * dt;
	if (act == LEFT)
		mPos -= mRight * mCameraSpeed * dt;
	if (act == SPRINT && pressed)
		mCameraSpeed = SPRINT_SPEED;
	else if (act == WALK && pressed)
		mCameraSpeed = WALK_SPEED;
	else
		mCameraSpeed = NORMAL_SPEED;
	updateCameraVectors();
}

void Camera::handleMouseButtonInput(int left, int right, int middle, float dt)
{

}

void Camera::handleMouseMoveInput(double mouseXpos, double mouseYpos, float dt)
{
	double dx = mouseXpos - lastMouseX;
	double dy = mouseYpos - lastMouseY;
	lastMouseX = mouseXpos;
	lastMouseY = mouseYpos;

	mPitch -= (float) dy * mSensitivity;
	mPitch = (float) utl::clamp(mPitch, MIN_PITCH, MAX_PITCH);
	mYaw += (float) dx * mSensitivity;
	mYaw = utl::loop(mYaw, MIN_YAW, MAX_YAW);

	updateCameraVectors();
}

void Camera::handleMouseScrollEvent(GLFWwindow * win, double xOffset, double yOffset)
{
	typedef std::tuple<Camera*, double> Args;
	Args* args = (std::tuple<Camera*, double>*) glfwGetWindowUserPointer(win);

	if (args == nullptr)
		return;
	Camera* cam = std::get<0>(*args);
	double dt = std::get<1>(*args);

	double oldFov = cam->mFov;
	cam->mFov -= yOffset * ZOOM_SPEED;
	cam->mFov = (float)utl::clamp(cam->mFov, MIN_FOV, MAX_FOV);
	cam->mSensitivity = cam->mSensitivity * (cam->mFov / oldFov);
}
