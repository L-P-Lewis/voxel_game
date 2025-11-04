#include "layer/game_layer.h"
#include "chunk.h"


Chunk chunk;

std::string vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 normal;\n"
    "layout (location = 2) in vec2 normal;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"out vec3 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(position, 1.0);\n"
	"	vColor = normal;\n"
    "}\0";

std::string fragment_shader_source = "#version 330 core\n"
	"in vec3 vColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(vColor, 1.0f);\n"
	"}";

Shader *shader;

GameLayer::GameLayer()
{
	this->registry.RegisterBlock("air", Block::AllSides(0));
	this->registry.RegisterBlock("stone", Block::AllSides(1));
	chunk.Fill(1);

	shader = new Shader(vertex_shader_source, fragment_shader_source);

	this->camera.setFov(45);
	this->camera.setNearPlane(0.1f);
	this->camera.setFarPlane(1000.0f);
	this->camera.setPosition(glm::dvec3(0.0, 0.0, -3.0));
	this->camera.setYaw(90);
	this->camera.setRoll(0);
	this->camera.setPitch(0);
	this->camera.setPerspective();
	this->camera.setAspectRatio(1.0);
	this->camera.recalculateProjectionMatrix();
	this->camera.recalculateViewMatrix();
}

void GameLayer::render(float deltaTime) 
{
	chunk.Draw(*shader);
}

bool GameLayer::tick() 
{return true;}

void GameLayer::resize(int width, int height) 
{}
