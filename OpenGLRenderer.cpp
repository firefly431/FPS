#include "OpenGLRenderer.h"

#include <Windows.h>
#include <gl/GL.h>

OpenGLRenderer::OpenGLRenderer() {}


OpenGLRenderer::~OpenGLRenderer() {}

void OpenGLRenderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the meshes
	scene_mesh.use();
	scene_mesh.draw();
	player_mesh.use();
	auto it = players.cbegin(), end = players.cend();
	for (; it != end; ++it) {
		const Player &p = *it;
		player_mesh.draw(p);
	}
	Mesh::unbind();
	// swap buffers
}

void OpenGLRenderer::resize(unsigned int width, unsigned int height) {
	// IDK
}