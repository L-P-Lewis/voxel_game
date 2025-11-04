#include "assets/assets.h"
#include "assets/shader.h"
#include "assets/texture.h"
#include <fstream>
#include <iostream>



Shader AssetManager::GetShader(std::string shader_id)
{
	std::string vertex_file = shader_id + ".vert";
	std::string fragment_file = shader_id + ".frag";
	std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(this->base_path / vertex_file);
        fShaderFile.open(this->base_path / fragment_file);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return Shader("", "");
    }
	return Shader(vertexCode, fragmentCode);
}


Texture AssetManager::GetTexture(std::string texture_id)
{
	std::string texture_file = texture_id + ".png";
	std::ifstream tex_file;
    tex_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try {
		tex_file.open(this->base_path / texture_file);
		return Texture(this->base_path / texture_file);
	} 
	catch (std::ifstream::failure e) {
		return Texture();
	}
}
