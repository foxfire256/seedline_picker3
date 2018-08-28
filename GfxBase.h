#ifndef GFXBASE_H
#define GFXBASE_H

#define GLM_FORCE_ONLY_XYZW
#include <glm/glm.hpp>
#include "jds_shader.hpp"
#include "jds_shader_program.hpp"
#include "JdsVtkScalar.h"
#include "JdsVtkScalarGfx.h"
#include "jds_counter.hpp"
#include "SeedLine.h"
#include "SeedLineGfx.h"
#include "dragonfly_wing.h"

class GfxBase
{
public:
	GfxBase();
	~GfxBase();

	void gfx_resize(int width, int height);
	void gfx_init_gl();
	void gfx_render();

protected:
	int frames, framerate;
	int win_w, win_h;
	float glsl_version;

	glm::vec3 eye, target, up, right, front, picker;
	float picker_length;
	glm::uvec3 picker_ind;
	glm::vec3 ppos, pvel;

	jds::counter *timer;
	jds::shader *prim_frag, *prim_vert;
	jds::shader_program *psp;
	jds::shader *wing_vert, *wing_frag, *s2_vert, *s2_frag;
	jds::shader_program *wsp, *s2sp;
	glm::mat4 MV, P;
	glm::mat3 normal_matrix;

	// model stuff
	JdsVtkScalar *sdata;
	JdsVtkScalarGfx *sgfx;
	SeedLine *sl;
	SeedLineGfx *slgfx;
	dragonfly_wing *dfw;
	std::string data_root;

	GLuint cube_vao, cube_vbo, lines_vao, lines_vbo;
	GLfloat *cube_data, *lines_data;

	// render control
	int draw_cubes, draw_vectors, draw_wing_model;

	// movement
	int forward, back, yaw_cw, yaw_ccw, pitch_up, pitch_down, roll_right,
		roll_left, strafe_left, strafe_right, strafe_up, strafe_down;
	float yaw_rate, pitch_rate, roll_rate;
	float vel;

	void load_files();
	void udpate_cube_and_lines();
	void print_cube_verts();
	void update_camera();
};

#endif // GFXBASE_H
