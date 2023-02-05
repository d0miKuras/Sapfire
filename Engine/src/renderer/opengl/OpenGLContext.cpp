#include "engpch.h"
#include "OpenGLContext.h"
#include "Log.h"
#include <SDL_video.h>

OpenGLContext::OpenGLContext(SDL_Window *window) : mWindowHandle(window)
{
}

void OpenGLContext::Init()
{
	// Set profile to core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Set version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// GPU (hardware) accel
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	// OpenGL context
	mGlContext = SDL_GL_CreateContext(mWindowHandle);
	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		ENGINE_ERROR("Failed to initialize window.");
		return;
	}
	glGetError(); // to clean benign error code
}

void OpenGLContext::SwapBuffers()
{
	/* glClearColor(0.83f, 0.83f, 0.83f, 1.0f); */
	/* // clear color and depth buffers */
	/* glClear(GL_COLOR_BUFFER_BIT); */
	SDL_GL_SwapWindow(mWindowHandle);
}

void OpenGLContext::Shutdown()
{
	SDL_GL_DeleteContext(mGlContext);
}
