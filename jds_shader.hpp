#ifndef JDS_SHADER_H
#define JDS_SHADER_H

#include "jds_opengl.hpp"

namespace jds
{
enum shader_type {FRAG_SHADER = 1, VERT_SHADER = 0, GEO_SHADER = 2,
	TESS_CTRL_SHADER = 3, TESS_EVAL_SHADER = 4, COMP_SHADER = 5};

class shader
{
	GLuint shader_id;
public:
	shader();
	shader(char *fname, shader_type t);
	~shader();
	void print_shader_info_log();
	int is_loaded(){return loaded;}
	void load(char *fname, shader_type t);
	GLuint get_id(){return shader_id;}
	int get_size(){return size;}
	char *get_filename(){return filename;}

private:
	char *filename;
	int loaded, size;
	unsigned char *src;
	shader_type type;

	void load_file(char *fname);
};
}

#endif // JDS_SHADER_H
