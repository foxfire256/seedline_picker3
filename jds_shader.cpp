#include "jds_shader.hpp"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <cstring>
#include <cstdlib>
#include <cstdio>

#ifdef _MSC_VER
#pragma warning(disable : 4996) // crt is secure, disable the warning
#endif

namespace jds
{

//------------------------------------------------------------------------------
shader::shader()
{
	filename = NULL;
	loaded = 0;
	size = 0;
	src = NULL;
}

//------------------------------------------------------------------------------
shader::shader(char *fname, shader_type t)
{
	filename = NULL;
	loaded = 0;
	size = 0;
	src = NULL;

	load(fname, t);
}

//------------------------------------------------------------------------------
shader::~shader()
{
	if(filename)
		delete [] filename;
	if(src)
		free(src);

	if(loaded)
		glDeleteShader(shader_id);
}

//------------------------------------------------------------------------------
void shader::load(char *fname, shader_type t)
{
	load_file(fname);

	type = t;

	// TODO: find a work around for this HACK
	char **src_hack;
	src_hack = (char **)malloc(sizeof(char *));
	src_hack[0] = (char *)malloc(sizeof(char) * (strlen((char *)src) + 1));

	memcpy(src_hack[0], (char *)src, strlen((char *)src));
	src_hack[0][strlen((char *)src)] = 0x00;

	if(type == FRAG_SHADER)
		shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	else if(type == VERT_SHADER)
		shader_id = glCreateShader(GL_VERTEX_SHADER);
	else if(type == GEO_SHADER)
		shader_id = glCreateShader(GL_GEOMETRY_SHADER);
#ifdef GLEW_ARB_tessellation_shader
	else if(type == TESS_CTRL_SHADER)
		shader_id = glCreateShader(GL_TESS_CONTROL_SHADER);
	else if(type == TESS_EVAL_SHADER)
		shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER);
#endif
#ifdef GLEW_ARB_compute_shader
	else if(type == COMP_SHADER)
		shader_id = glCreateShader(GL_COMPUTE_SHADER);
#endif

	glShaderSource(shader_id, 1, (const GLchar **)src_hack, NULL);
	print_opengl_error();

	glCompileShader(shader_id);
	print_opengl_error();

	print_shader_info_log();

	loaded = 1;

	free(src_hack[0]);
	free(src_hack);
	free(src);
	src = NULL;
}

//------------------------------------------------------------------------------
// TODO: convert this to C++ iostream
void shader::load_file(char *fname)
{
	filename = new char[strlen(fname)];
	strcpy(filename, fname);

	FILE *f;
	int fsize, result;

	// try to open the file
	f = fopen(filename, "rt");
	if(f == NULL)
	{
		printf("ERROR: failed to open shader: %s\n", filename);
		fflush(stdout);
		size = 0;
		src = NULL;
		return;
	}

	// find the file size
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);

	// allocate some memory
	src = (unsigned char *)malloc(sizeof(unsigned char) * (fsize + 1));
	if(src == NULL)
	{
		printf("ERROR: loading shader: %s, failed to allocate %d bytes memory\n", filename, fsize);
		fflush(stdout);
		size = 0;
		return;
	}
	size = fsize;

	// read the file
	result = (int)fread(src, sizeof(char), fsize, f);
	src[fsize] = '\0';
	if(result != fsize)
	{
		printf("ERROR: loading shader: %s\n", filename);
		printf("expected %d bytes but only read %d\n", fsize, result);
	}
	else
	{
		//printf("Loaded shader source: %s  OK\n", filename);
	}

	fclose(f);
}

//------------------------------------------------------------------------------
void shader::print_shader_info_log()
{
	int length = 0, chars_written = 0;
	GLchar *info_log;

	print_opengl_error();
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

	if(length > 0)
	{
		info_log = new GLchar[length];
		if(info_log == NULL)
		{
			cerr << "ERROR couldn't allocate " << length << " bytes for " <<
				filename << " info log\n";
			return;
		}

		glGetShaderInfoLog(shader_id, length, &chars_written, info_log);

		cout << filename << ": " << info_log;

		delete [] info_log;
	}

	print_opengl_error();
}

//------------------------------------------------------------------------------
}
