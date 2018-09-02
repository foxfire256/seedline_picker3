#include "jds_vtk_scalar_gfx.h"
#include <glm/gtc/type_ptr.hpp>

//------------------------------------------------------------------------------
jds_vtk_scalar_gfx::jds_vtk_scalar_gfx(jds_vtk_scalar *d_in)
{
	d = d_in;
}


//------------------------------------------------------------------------------
// c is the index of the cube to draw
void jds_vtk_scalar_gfx::get_single_cube_verts(glm::uvec3 c, GLfloat *data)
{
	for(unsigned int i = 0; i < d->cube_indicies.size(); i++)
	{
		data[i * 3] = d->cv[c.x][c.y][c.z][d->cube_indicies[i]].x;
		data[i * 3 + 1] = d->cv[c.x][c.y][c.z][d->cube_indicies[i]].y;
		data[i * 3 + 2] = d->cv[c.x][c.y][c.z][d->cube_indicies[i]].z;
	}
}

//------------------------------------------------------------------------------
void jds_vtk_scalar_gfx::gen_hh_vao()
{
	glGenVertexArrays(1, &hh_vao);
	glBindVertexArray(hh_vao);
	glGenBuffers(1, &hh_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hh_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * d->hedge_hog_count * 3 * 2,
		d->hedge_hog_array, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	/*
	 * glVertexAttribPointer(
	 *	index (layout = 0) in shader,
	 *	components to use per vertex,
	 *	data type GL_FLOAT,
	 *	normalize the data 0 or 1,
	 *	byte offset between consecutive generic vertex attributes,
	 *	first component
	 **/
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
	jds::print_opengl_error();
}

//------------------------------------------------------------------------------
void jds_vtk_scalar_gfx::draw_hh_vao()
{
	glBindVertexArray(hh_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
	glDrawArrays(GL_LINES, 0, d->hedge_hog_count * 2);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(GLfloat) * 6, 0);
	glDrawArrays(GL_POINTS, 0, d->hedge_hog_count);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, hh_vbo);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, 0);
	glDrawArrays(GL_LINES, 0, d->hedge_hog_count);

	glColor4f(0.0f, 0.0f, 1.0f, 0.95f);
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 6, 0);
	glDrawArrays(GL_POINTS, 0, d->hedge_hog_count);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
}

//------------------------------------------------------------------------------
void jds_vtk_scalar_gfx::gen_wing_vao()
{
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * d->cube_vert_count * 3,
		d->cube_vert_array, GL_STATIC_DRAW);
	jds::print_opengl_error();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------------------------
void jds_vtk_scalar_gfx::draw_wing_cubes_vao()
{
	glBindVertexArray(hh_vao);
	glDrawArrays(GL_LINES, 0, d->cube_vert_count);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, 0);

	glDrawArrays(GL_LINES, 0, d->cube_vert_count);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
}

