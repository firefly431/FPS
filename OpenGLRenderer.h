#pragma once
#include <vector>

class OpenGLRenderer
{
public:
	OpenGLRenderer();

	void resize(unsigned int width, unsigned int height);
	void draw();
protected:
	// SFML context would go here
	std::vector<Player> players;
	Shader scene_vshader, player_vshader, basic_fshader;
	Shader::Program scene_program, player_program;
	StaticMesh scene_mesh;
	PlayerMesh player_mesh;
};

