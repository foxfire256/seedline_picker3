#include "jds_opengl.hpp"

#include <GL/glu.h>
#include <glm/glm.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
#pragma warning(disable : 4996) // crt is secure, disable the warning
#endif

namespace jds
{
#define print_opengl_error() print_opengl_error2((char *)__FILE__, __LINE__)

//------------------------------------------------------------------------------
// Returns 1 if an OpenGL error occurred, 0 otherwise.
int print_opengl_error2(char *file, int line)
{
	GLenum gl_err;
	int	ret_code = 0;

	gl_err = glGetError();
	while(gl_err != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line,
			   gluErrorString(gl_err));
		ret_code = 1;
		gl_err = glGetError();
		fflush(stdout);
	}
	return ret_code;
}

//------------------------------------------------------------------------------
// checks the completeness of the currently bound draw framebuffer
void check_draw_framebuffer()
{
	GLenum fbo_status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	printf("Draw framebuffer status: ");

	switch(fbo_status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		printf("GL_FRAMEBUFFER_COMPLETE\n");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("GL_FRAMEBUFFER_UNDEFINED\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
		break;
	default:
		printf("Unknow enum for glCheckFramebufferStatus()\n");
		break;
	}
}

//------------------------------------------------------------------------------
void start_glew()
{
	// GLEW init
	GLenum err;

	print_opengl_error();
	glewExperimental = GL_TRUE;
	err = glewInit();
	print_opengl_error();
	if(GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong.
		printf("Error: %s\n", glewGetErrorString(err));
	}
	printf("GLEW version %s\n", glewGetString(GLEW_VERSION));
	// done GLEW init

	print_opengl_error();

	if(glewIsSupported("GL_VERSION_5_1"))
	{
		printf("GLEW GL_VERSION_5_1\n");
	}
	else if(glewIsSupported("GL_VERSION_5_0"))
	{
		printf("GLEW GL_VERSION_5_0\n");
	}
	else if(glewIsSupported("GL_VERSION_4_4"))
	{
		printf("GLEW GL_VERSION_4_4\n");
	}
	else if(glewIsSupported("GL_VERSION_4_3"))
	{
		printf("GLEW GL_VERSION_4_3\n");
	}
	else if(glewIsSupported("GL_VERSION_4_2"))
	{
		printf("GLEW GL_VERSION_4_2\n");
	}
	else if(glewIsSupported("GL_VERSION_4_1"))
	{
		printf("GLEW GL_VERSION_4_1\n");
	}
	else if(glewIsSupported("GL_VERSION_4_0"))
	{
		printf("GLEW GL_VERSION_4_0\n");
	}
	else if(glewIsSupported("GL_VERSION_3_2"))
	{
		printf("GLEW GL_VERSION_3_2\n");
	}
	else if(glewIsSupported("GL_VERSION_3_1"))
	{
		printf("GLEW GL_VERSION_3_1\n");
	}
	else if(glewIsSupported("GL_VERSION_3_0"))
	{
		printf("GLEW GL_VERSION_3_0\n");
	}
	else if(glewIsSupported("GL_VERSION_2_1"))
	{
		printf("GLEW GL_VERSION_2_1\n");
	}
	else if(glewIsSupported("GL_VERSION_2_0"))
	{
		printf("GLEW GL_VERSION_2_0\n");
	}

	fflush(stdout);

	// check for errors first
	print_opengl_error();
}

//------------------------------------------------------------------------------
void print_opengl_info()
{
	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));
}

//------------------------------------------------------------------------------
float get_glsl_version()
{
	int length;
	char short_version[5];
	char *version = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

	if(version)
		length = (int)strlen(version);
	else
		return 0.0f;

	// only want 4 characters from the version string
	// Intel likes to add a bunch of crap after the GLSL Version
	if(length >= 4)
	{
		memcpy(short_version, version, 4);
		short_version[4] = 0x00;
	}
	else if(length < 4)
	{
		memset(short_version, 0x00, 5);
		memcpy(short_version, version, length);
	}

	return (float)atof(short_version);
}

//------------------------------------------------------------------------------
void print_glm_version()
{
	int major, minor, patch, revision;
	// defined in setup.hpp
	major = GLM_VERSION_MAJOR;
	minor = GLM_VERSION_MINOR;
	patch = GLM_VERSION_PATCH;
	revision = GLM_VERSION_REVISION;

	printf("GLM Version %d.%d.%d.%d\n", major, minor, patch, revision);
}

}
