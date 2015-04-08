#pragma once

#include <vector>
#include <SFML/Window.hpp>

#include "PlayerMesh.h"
#include "Camera.h"

#if defined(SFML_SYSTEM_WINDOWS)
void *loadGLFunc(const char *name);
#endif

class OpenGLRenderer {
public:
	OpenGLRenderer(int width, int height);

    void mainloop();

	void resize(unsigned int width, unsigned int height);
	void draw();
protected:
    sf::Window window;
	class _loadfunctionsonwindows {
#if defined(SFML_SYSTEM_WINDOWS)
	public:
		_loadfunctionsonwindows() {
			glActiveTexture = (PFNGLACTIVETEXTUREPROC)loadGLFunc("glActiveTexture");
			glAttachShader = (PFNGLATTACHSHADERPROC)loadGLFunc("glAttachShader");
			glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)loadGLFunc("glBindAttribLocation");
			glBindBuffer = (PFNGLBINDBUFFERPROC)loadGLFunc("glBindBuffer");
			glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)loadGLFunc("glBindFragDataLocation");
			glBindTexture = (PFNGLBINDTEXTUREPROC)loadGLFunc("glBindTexture");
			glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)loadGLFunc("glBindVertexArray");
			glBufferData = (PFNGLBUFFERDATAPROC)loadGLFunc("glBufferData");
			glClear = (PFNGLCLEARPROC)loadGLFunc("glClear");
			glCreateProgram = (PFNGLCREATEPROGRAMPROC)loadGLFunc("glCreateProgram");
			glDeleteProgram = (PFNGLDELETEPROGRAMPROC)loadGLFunc("glDeleteProgram");
			glDeleteTextures = (PFNGLDELETETEXTURESPROC)loadGLFunc("glDeleteTextures");
			glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)loadGLFunc("glDeleteVertexArrays");
			glDrawArrays = (PFNGLDRAWARRAYSPROC)loadGLFunc("glDrawArrays");
			glDrawElements = (PFNGLDRAWELEMENTSPROC)loadGLFunc("glDrawElements");
			glEnable = (PFNGLENABLEPROC)loadGLFunc("glEnable");
			glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)loadGLFunc("glEnableVertexAttribArray");
			glGenBuffers = (PFNGLGENBUFFERSPROC)loadGLFunc("glGenBuffers");
			glGenTextures = (PFNGLGENTEXTURESPROC)loadGLFunc("glGenTextures");
			glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)loadGLFunc("glGenVertexArrays");
			glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)loadGLFunc("glGetProgramInfoLog");
			glGetProgramiv = (PFNGLGETPROGRAMIVPROC)loadGLFunc("glGetProgramiv");
			glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)loadGLFunc("glGetUniformLocation");
			glLinkProgram = (PFNGLLINKPROGRAMPROC)loadGLFunc("glLinkProgram");
			glPixelStorei = (PFNGLPIXELSTOREIPROC)loadGLFunc("glPixelStorei");
			glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)loadGLFunc("glProgramUniform1i");
			glTexImage2D = (PFNGLTEXIMAGE2DPROC)loadGLFunc("glTexImage2D");
			glTexParameterf = (PFNGLTEXPARAMETERFPROC)loadGLFunc("glTexParameterf");
			glTexParameteri = (PFNGLTEXPARAMETERIPROC)loadGLFunc("glTexParameteri");
			glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)loadGLFunc("glUniformMatrix4fv");
			glUseProgram = (PFNGLUSEPROGRAMPROC)loadGLFunc("glUseProgram");
			glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)loadGLFunc("glVertexAttribPointer");
			glViewport = (PFNGLVIEWPORTPROC)loadGLFunc("glViewport");
			// 2nd compile
			glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)loadGLFunc("glDeleteBuffers");
			glDeleteShader = (PFNGLDELETESHADERPROC)loadGLFunc("glDeleteShader");
			// 3rd compile
			glBufferSubData = (PFNGLBUFFERSUBDATAPROC)loadGLFunc("glBufferSubData");
			glCompileShader = (PFNGLCOMPILESHADERPROC)loadGLFunc("glCompileShader");
			glCreateShader = (PFNGLCREATESHADERPROC)loadGLFunc("glCreateShader");
			glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)loadGLFunc("glGetShaderInfoLog");
			glGetShaderiv = (PFNGLGETSHADERIVPROC)loadGLFunc("glGetShaderiv");
			glShaderSource = (PFNGLSHADERSOURCEPROC)loadGLFunc("glShaderSource");
			// etc
			glGetError = (PFNGLGETERRORPROC)loadGLFunc("glGetError");
		}
#endif
	} functions;
    Camera camera;
    PlayerMesh p_mesh;
    std::vector<Player> players;
    int current_player;
};

