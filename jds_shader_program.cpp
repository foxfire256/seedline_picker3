#include "jds_shader_program.hpp"
#include "jds_shader.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace jds
{

//------------------------------------------------------------------------------
shader_program::shader_program()
{
	linked = 0;
}

//------------------------------------------------------------------------------
shader_program::~shader_program()
{
	if(linked)
		glDeleteProgram(id);
}

//------------------------------------------------------------------------------
void shader_program::add_shader(shader *s)
{
	sl.push_back(s);
}

//------------------------------------------------------------------------------
void shader_program::link()
{
	print_opengl_error();
	id = glCreateProgram();

	for(unsigned int i = 0; i < sl.size(); i++)
	{
		glAttachShader(id, sl[i]->get_id());
		print_opengl_error();
	}

	glBindFragDataLocation(id, 0, "out_color");
	print_opengl_error();

	glLinkProgram(id);
	print_opengl_error();

	GLint status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
		cerr << "Shader link ERROR!\n";
	}
	else
		linked = 1;

	print_program_info_log();
}

//------------------------------------------------------------------------------
void shader_program::print_program_info_log()
{
	int length = 0, chars_written = 0;
	GLchar *info_log;

	print_opengl_error();
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
	print_opengl_error();

	if(length > 0)
	{
		info_log = new GLchar[length];
		if(info_log == NULL)
		{
			cerr << "ERROR couldn't allocate " << length << " bytes for " <<
				id << " info log\n";
			return;
		}

		glGetProgramInfoLog(id, length, &chars_written, info_log);

		cout << "GLSL Program ID " << id << ": " << info_log;

		delete [] info_log;
	}

	print_opengl_error();
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, float x, float y, float z)
{
	int u = glGetUniformLocation(id, uname);
	if(u >= 0)
	{
		glUniform3f(u, x, y, z);
	}
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, float x)
{
	int u = glGetUniformLocation(id, uname);
	if(u >= 0)
	{
		glUniform1f(u, x);
	}
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, int x)
{
	print_opengl_error();
	int u = glGetUniformLocation(id, uname);
	print_opengl_error();
	if(u >= 0)
	{
		glUniform1i(u, x);
		print_opengl_error();
	}
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, const mat4 &m)
{
	int u = glGetUniformLocation(id, uname);
	if(u >= 0)
	{
		glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(m));
	}
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, const glm::mat3 &m)
{
	int u = glGetUniformLocation(id, uname);
	if(u >= 0)
	{
		glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(m));
	}
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, const vec3 &v)
{
	int u = glGetUniformLocation(id, uname);
	if(u >= 0)
	{
		glUniform3f(u, v.x, v.y, v.z);
	}
}

//------------------------------------------------------------------------------
void shader_program::set_uniform(const char *uname, const vec4 &v)
{
	int u = glGetUniformLocation(id, uname);
	if(u >= 0)
	{
		glUniform4f(u, v.x, v.y, v.z, v.w);
	}
}

}
