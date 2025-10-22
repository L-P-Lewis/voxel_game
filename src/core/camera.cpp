#include "camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"


void Camera::setPosition(const glm::dvec3 &position)
{
	this->position = position;
	updateView = true;
}

void Camera::setYaw(const float yaw)
{
	this->yaw = glm::radians(yaw);
	updateView = true;
}

void Camera::setPitch(const float pitch)
{
	this->pitch = glm::radians(pitch);
	updateView = true;
}

void Camera::setRoll(const float roll)
{
	this->roll = glm::radians(roll);
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

void Camera::setAspectRatio(const float aspectRatio)
{
	this->aspectRatio = aspectRatio;
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
	return projection;
	// assert(false);
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
	return view;
	// assert(false);
}

void Camera::recalculateViewMatrix()
{
	view = glm::lookAt(position, position + forwards, up);
	// assert(false);
}

void Camera::recalculateProjectionMatrix()
{
	projection = glm::perspective(fov, aspectRatio, near, far);
	// assert(false);
}
