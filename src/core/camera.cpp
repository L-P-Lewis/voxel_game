#include "camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <iostream>


Camera::Camera()
{
	this->position = glm::dvec3(1.0);
	this->forwards = glm::dvec3(0.0, 0.0, -1.0);
	this-> up = glm::dvec3(0.0, 1.0, 0.0);
	this->updateProjection = true;
	this->updateView = true;
}

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
	glm::vec3 direction;
	direction.x = cos(yaw) * cos(pitch);
	direction.y = sin(pitch);
	direction.z = sin(yaw) * cos(pitch);
	//std::cout << "(" << direction.x << "," << direction.y << "," <<direction.z << ")" << std::endl;
	forwards = glm::normalize(direction);
	view = glm::lookAt(position, position + forwards, glm::dvec3(0.0, 1.0, 0.0));
	// assert(false);
}

void Camera::recalculateProjectionMatrix()
{
	projection = glm::perspective(fov, aspectRatio, near, far);
	// assert(false);
}


bool Camera::TestFrustumCull(glm::vec3 min, glm::vec3 max)
{
	glm::vec3 points[8] = {
		glm::vec3(min.x, min.y, min.z),
		glm::vec3(max.x, min.y, min.z),
		glm::vec3(min.x, max.y, min.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(min.x, max.y, max.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(max.x, max.y, min.z),
		glm::vec3(max.x, max.y, max.z)
	};
	ViewPlane near_plane = ViewPlane(position, glm::normalize(forwards));
	for (int i = 0; i < 8; i++) {
		if (near_plane.TestPoint(points[i])) return true;
	}
	return false;
}
