#include "camera.h"


void Camera::setPosition(const glm::dvec3 &position)
{
	this->position = position;
	updateView = true;
}

void Camera::setYaw(const float yaw)
{
	this->yaw = yaw;
	updateView = true;
}

void Camera::setPitch(const float pitch)
{
	this->pitch = pitch;
	updateView = true;
}

void Camera::setRoll(const float roll)
{
	this->roll = roll;
	updateView = true;
}

void Camera::setNearPlane(const float near)
{
	this->near = near;
	updateProjection = true;
}

void Camera::setFarPlane(const float far)
{
	this->far = far;
	updateProjection = true;
}

void Camera::setFov(const float fov)
{
	this->fov = fov;
	updateProjection = true;
}

glm::mat4 Camera::getProjectionMatrix()
{
	if (updateProjection)
	{
		this->recalculateProjectionMatrix();
		updateProjection = false;
	}
	return static_cast<const Camera*>(this)->getProjectionMatrix();
}

glm::mat4 Camera::getProjectionMatrix() const
{
	//TODO: Implement
	assert(false);
}

glm::mat4 Camera::getViewMatrix()
{
	if (updateView)
	{
		this->recalculateViewMatrix();
		updateView = false;
	}
	return static_cast<const Camera*>(this)->getViewMatrix();

}

glm::mat4 Camera::getViewMatrix() const
{
	//TODO: Implement
	assert(false);
}

void Camera::recalculateViewMatrix()
{
	//TODO: Implement
	assert(false);
}

void Camera::recalculateProjectionMatrix()
{
	//TODO: Implement
	assert(false);
}
