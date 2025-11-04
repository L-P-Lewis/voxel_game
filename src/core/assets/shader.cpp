#include "assets/shader.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <iostream>
#include <string>


Shader::Shader(std::string vertex, std::string fragment)
{
	if (vertex.empty() || fragment.empty()) {return;}

	const char *raw_vertex, *raw_fragment;
	unsigned int vert_shader, frag_shader;
	int success;
	char infoLog[512];

	raw_vertex = vertex.c_str();
	raw_fragment = fragment.c_str();

	std::cout << "Vertex:" << std::endl << vertex << std::endl;
	std::cout << "Fragment:" << std::endl << fragment << std::endl;
   
	// vertex Shader
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &raw_vertex, NULL);
	glCompileShader(vert_shader);
	// print compile errors if any
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
    	glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	

	// fragment Shader
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &raw_fragment, NULL);
	glCompileShader(frag_shader);
	// print compile errors if any
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
    	glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// shader Program
	program_id = glCreateProgram();
	glAttachShader(program_id, vert_shader);
	glAttachShader(program_id, frag_shader);
	glLinkProgram(program_id);
	// print linking errors if any
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if(!success)
	{
	    glGetProgramInfoLog(program_id, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	  
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
}


void Shader::use() const
{
	glUseProgram(program_id);
}


void Shader::set_uniform(const std::string &uniform, bool value) const 
{
	glUniform1i(glGetUniformLocation(program_id, uniform.c_str()), (int)value); 
}


void Shader::set_uniform(const std::string &uniform, int value) const
{
	glUniform1i(glGetUniformLocation(program_id, uniform.c_str()), value); 
}


void Shader::set_uniform(const std::string &uniform, float value) const
{
	glUniform1f(glGetUniformLocation(program_id, uniform.c_str()), value); 
}


void Shader::set_uniform(const std::string &uniform, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
