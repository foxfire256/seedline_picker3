#ifndef jds_vtk_scalar_gfx_H
#define jds_vtk_scalar_gfx_H

#include "jds_vtk_scalar.h"
#include "jds_opengl.hpp"

class jds_vtk_scalar_gfx
{
public:
	jds_vtk_scalar *d;

	jds_vtk_scalar_gfx(jds_vtk_scalar *d_in);
	void gen_wing_vao();
	void draw_wing_cubes_vao();
	void gen_hh_vao();
	void draw_hh_vao();
	void get_single_cube_verts(glm::uvec3 c, GLfloat *data);

private:
	GLuint cube_vbo, hh_vbo, cube_vao, hh_vao;
};

#endif // jds_vtk_scalar_gfx_H
