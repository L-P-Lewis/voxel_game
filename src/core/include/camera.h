#pragma once
#ifndef VOXEL_CORE_CAMERA_H_
#define VOXEL_CORE_CAMERA_H_

#include <glm/glm.hpp>


/**
 * Projection Mode Enumeration
 */
enum ProjectionMode {
	PERSPECTIVE,
	ORTHOGRAPHIC
};


/**
 * A general purpose Camera class
 *
 * Storage class to make working with projection matricies from camera usage easier
 */
class Camera {
	private:
		glm::dvec3 position;
		glm::vec3 forwards;
		glm::vec3 up;
		glm::vec3 left;
		float yaw;
		float pitch;
		float roll;
		float near;
		float far;
		float fov;
		ProjectionMode projection_mode;
	public:
		void setPosition(glm::dvec3 position) {this->position = position;};
		glm::dvec3 getPosition() const {return this->position;};
		glm::vec3 getLookVector() const {return this->forwards;};
		glm::vec3 getUpVector() const {return this->up;};
		glm::vec3 getLeftVector() const {return this->left;};
		void setYaw(float yaw);
		float getYaw() const {return this->yaw;};
		void setPitch(float pitch);
		float getPitch() const {return this->pitch;};
		void setRoll(float roll);
		float getRoll() const {return this->pitch;};
		void setNearPlane(float near) {this->near = near;};
		float getNearPlane() const {return this->near;};
		void setFarPlane(float far) {this->far = far;};
		float getFarPlane() const {return this->far;};
		void setFov(float fov) {this->fov = fov;};
		float getFov() const {return this->fov;};
		ProjectionMode getProjectionMode() const {return this->projection_mode;}
		void setPerspective() {this->projection_mode = ProjectionMode::PERSPECTIVE;};
		void setOrthographic() {this->projection_mode = ProjectionMode::ORTHOGRAPHIC;};
		glm::mat4 getProjectionMatrix() const;
		glm::mat4 getViewMatrix() const;
};


#endif
