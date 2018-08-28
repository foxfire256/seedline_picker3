#ifndef JDS_SHADER_PROGRAM_HPP
#define JDS_SHADER_PROGRAM_HPP

#include "jds_opengl.hpp"
#include <vector>
using std::vector;
#include <glm/glm.hpp>

namespace jds
{

class shader;

class shader_program
{
	GLuint id;
public:
	shader_program();
	~shader_program();
	GLuint get_id(){return id;}
	void add_shader(shader *s);
	void link();
	void print_program_info_log();
	void use(){glUseProgram(id);}

	void set_uniform(const char *uname, float x, float y, float z);
	void set_uniform(const char *uname, float x);
	void set_uniform(const char *uname, int x);
	void set_uniform(const char *uname, const glm::mat4 &m);
	void set_uniform(const char *uname, const glm::mat3 &m);
	void set_uniform(const char *uname, const glm::vec3 &v);
	void set_uniform(const char *uname, const glm::vec4 &v);

private:
	vector<shader *> sl;
	int linked;
};

}

#endif // JDS_SHADER_PROGRAM_HPP
