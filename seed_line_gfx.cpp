#include "seed_line_gfx.h"
#include "jds_opengl.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//------------------------------------------------------------------------------
seed_line_gfx::seed_line_gfx(seed_line *sl_in, unsigned int vert_max_in)
{
	sl = sl_in;
	vert_count_max = vert_max_in;
	sl_data = new GLfloat[vert_max_in * 3];
	for(unsigned int i = 0; i < vert_max_in * 3; i++)
		sl_data[i] = 0.0f;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo_vert);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vert_max_in * 3, sl_data,
		GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
}

//------------------------------------------------------------------------------
seed_line_gfx::~seed_line_gfx()
{
}

//------------------------------------------------------------------------------
// this should update the data
void seed_line_gfx::update()
{
	// check to make sure we have enough memory
	if(sl->pos.size() > vert_count_max)
	{
		std::cerr << "seed_line_gfx: not enough vertex data allocated!\n";
		return;
	}

	// copy the data
	for(unsigned int i = 0; i < sl->pos.size(); i++)
	{
		sl_data[i * 3] = sl->pos[i].x;
		sl_data[i * 3 + 1] = sl->pos[i].y;
		sl_data[i * 3 + 2] = sl->pos[i].z;
	}
	vert_count = sl->pos.size();

	if(vert_count > 0)
	{
		// update the vao
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vert_count * 3, sl_data,
			GL_DYNAMIC_DRAW);
	}
}

//------------------------------------------------------------------------------
// draw using vao
void seed_line_gfx::draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, vert_count);
}
