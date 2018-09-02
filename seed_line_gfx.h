#ifndef seed_line_gfx_H
#define seed_line_gfx_H

#include "seed_line.h"
#include "jds_opengl.hpp"

class seed_line_gfx
{
public:
	seed_line_gfx(seed_line *sl_in, unsigned int vert_max_in = 32);
	~seed_line_gfx();

	void update();
	void draw();

private:
	seed_line *sl;
	GLuint vao, vbo_vert, vert_count, vert_count_max;
	GLfloat *sl_data;
};

#endif // seed_line_gfx_H
