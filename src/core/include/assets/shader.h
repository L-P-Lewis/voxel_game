#pragma once
#include "glm/fwd.hpp"
#ifndef CORE_ASSETS_SHADER_H_
#define CORE_ASSETS_SHADER_H_

#include <string>

/**
 * Shader asset class
 *
 * Handles the compilation and usage of shaders
 */
class Shader{
	private:
		unsigned int program_id;
	public:
		/** Create and compile a shader with the given vertex and fragment functions */
		Shader(std::string vertex, std::string fragment);
		/** Begin using the shader with the given context */
		void use() const;
		/** Sets a boolean shader uniform */
		void set_uniform(const std::string &uniform, bool value) const;
		/** Sets a float shader uniform */
		void set_uniform(const std::string &uniform, float value) const;
		/** Sets a int shader uniform */
		void set_uniform(const std::string &uniform, int value) const;
		/** Sets a mat4 uniform */
		void set_uniform(const std::string &uniform, glm::mat4 value) const;
};

#endif
