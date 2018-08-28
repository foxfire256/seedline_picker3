#ifndef SEEDLINEGFX_H
#define SEEDLINEGFX_H

#include "SeedLine.h"
#include "jds_opengl.hpp"

class SeedLineGfx
{
public:
	SeedLineGfx(SeedLine *sl_in, unsigned int vert_max_in = 32);
	~SeedLineGfx();

	void update();
	void draw();

private:
	SeedLine *sl;
	GLuint vao, vbo_vert, vert_count, vert_count_max;
	GLfloat *sl_data;
};

#endif // SEEDLINEGFX_H
