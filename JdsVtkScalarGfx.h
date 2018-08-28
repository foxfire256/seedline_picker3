#ifndef JDSVTKSCALARGFX_H
#define JDSVTKSCALARGFX_H

#include "JdsVtkScalar.h"
#include "jds_opengl.hpp"

class JdsVtkScalarGfx
{
public:
	JdsVtkScalar *d;

	JdsVtkScalarGfx(JdsVtkScalar *d_in);
	void gen_wing_vao();
	void draw_wing_cubes_vao();
	void gen_hh_vao();
	void draw_hh_vao();
	void get_single_cube_verts(glm::uvec3 c, GLfloat *data);

private:
	GLuint cube_vbo, hh_vbo, cube_vao, hh_vao;
};

#endif // JDSVTKSCALARGFX_H
