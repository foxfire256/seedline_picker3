#include "GfxBase.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <string>
using std::string;

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
using glm::ivec4;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#ifdef _MSC_VER
#pragma warning(disable : 4996) // crt is secure, disable the warning
#define snprintf _snprintf_s
#endif

//------------------------------------------------------------------------------
GfxBase::GfxBase()
{
	framerate = frames = 0;

	eye = vec3(14.67f, 14.88f, 13.04f);
	target = vec3(14.67f, 14.88f, 14.04f);
	up = vec3(0.0f, 1.0f, 0.0f);
	right = vec3(1.0f, 0.0f, 0.0f);
	front = vec3(0.0f, 0.0f, 1.0f);

	picker = target;
	picker.z = 13.34f;

	yaw_rate = 45.0f;
	roll_rate = 45.0f;
	pitch_rate = 45.0f;
	vel = 0.75;

	yaw_cw = yaw_ccw = 0;
	pitch_down = pitch_up = 0;
	forward = back = 0;
	roll_left = roll_right = 0;
	strafe_left = strafe_right = 0;
	strafe_up = strafe_down = 0;

	picker_length = 0.3f;

	draw_cubes = 0;
	draw_vectors = 0;
	draw_wing_model = 1;

	timer = new jds::counter();
	timer->init();
}

//------------------------------------------------------------------------------
GfxBase::~GfxBase()
{
}

//------------------------------------------------------------------------------
void GfxBase::gfx_resize(int width, int height)
{
	win_w = width;
	win_h = height;

	glViewport(0, 0, width, height);

	P = glm::perspective(65.0f, (float)width / (float)height, 0.01f, 40.0f);
	MV = glm::lookAt(eye, target, up);

	normal_matrix = glm::mat3(glm::vec3(MV[0]), glm::vec3(MV[1]),
		glm::vec3(MV[2]));
	s2sp->use();
	s2sp->set_uniform((char *)"MV", MV);
	s2sp->set_uniform((char *)"P", P);
	s2sp->set_uniform((char *)"normal_matrix", normal_matrix);
	psp->use();
	psp->set_uniform((char *)"P", P);
	psp->set_uniform((char *)"MV", MV);
	wsp->use();
	wsp->set_uniform((char *)"P", P);
	wsp->set_uniform((char *)"MV", MV);
	wsp->set_uniform((char *)"normal_matrix", normal_matrix);
}

//------------------------------------------------------------------------------
void GfxBase::gfx_init_gl()
{
	jds::start_glew();
	jds::print_opengl_info();
	glsl_version = jds::get_glsl_version();
	jds::print_glm_version();
	jds::print_opengl_error();

	std::string dir;
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
	dir = string("c:/dev/seedline_picker3/");
#else
	dir = string("");
#endif

	prim_vert = new jds::shader((char *)(dir + "prim_render_v330.vert").c_str(),
		jds::VERT_SHADER);
	prim_frag = new jds::shader((char *)(dir + "prim_render_v330.frag").c_str(),
		jds::FRAG_SHADER);
	psp = new jds::shader_program();
	psp->add_shader(prim_vert);
	psp->add_shader(prim_frag);
	psp->link();
	psp->use();
	psp->set_uniform((char *)"color", glm::vec4(1.0f));

	wing_vert = new jds::shader((char *)(dir + "smooth_ads_v330.vert").c_str(),
		jds::VERT_SHADER);
	wing_frag = new jds::shader((char *)(dir + "smooth_ads_v330.frag").c_str(),
		jds::FRAG_SHADER);
	wsp = new jds::shader_program();
	wsp->add_shader(wing_vert);
	wsp->add_shader(wing_frag);
	wsp->link();
	wsp->use();
	wsp->set_uniform((char *)"light_pos", glm::vec4(eye, 1.0f));
	wsp->set_uniform((char *)"La", glm::vec3(0.6f, 0.6f, 0.6f));
	wsp->set_uniform((char *)"Ls", glm::vec3(0.1f, 0.1f, 0.1f));
	wsp->set_uniform((char *)"Ld", glm::vec3(0.4f, 0.4f, 0.4f));
	wsp->set_uniform((char *)"shininess", 7.0f);
	wsp->set_uniform((char *)"Ka", glm::vec3(1.0f, 1.0f, 1.0f));
	wsp->set_uniform((char *)"Ks", glm::vec3(1.0f, 1.0f, 1.0f));
	wsp->set_uniform((char *)"Kd", glm::vec3(1.0f, 1.0f, 1.0f));

	s2_frag = new jds::shader((char*)(dir + "smooth_ads_twoside_v330.frag").c_str(),
		jds::FRAG_SHADER);
	s2_vert = new jds::shader((char*)(dir + "smooth_ads_twoside_v330.vert").c_str(),
		jds::VERT_SHADER);
	s2sp = new jds::shader_program();
	s2sp->add_shader(s2_vert);
	s2sp->add_shader(s2_frag);
	s2sp->link();
	s2sp->use();
	s2sp->set_uniform((char *)"light_pos", glm::vec4(eye, 1.0f));
	s2sp->set_uniform((char *)"La", glm::vec3(0.6f, 0.6f, 0.6f));
	s2sp->set_uniform((char *)"Ls", glm::vec3(0.1f, 0.1f, 0.1f));
	s2sp->set_uniform((char *)"Ld", glm::vec3(0.4f, 0.4f, 0.4f));
	s2sp->set_uniform((char *)"shininess", 7.0f);
	s2sp->set_uniform((char *)"Ka", glm::vec3(1.0f, 1.0f, 1.0f));
	s2sp->set_uniform((char *)"Ks", glm::vec3(1.0f, 1.0f, 1.0f));
	s2sp->set_uniform((char *)"Kd", glm::vec3(1.0f, 1.0f, 1.0f));

	jds::print_opengl_error();

	glLineWidth(1.25f);
	glPointSize(1.25f);

/*
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
	if(this->format().profile() != QGLFormat::CoreProfile)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glShadeModel(GL_SMOOTH);
	}
#endif
*/

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); // enable alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	jds::print_opengl_error();

	load_files();

	jds::print_opengl_error();

	cube_data = new GLfloat[12 * 2 * 3];
	lines_data = new GLfloat[3 * 2 * 3];

	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 72, cube_data,
		GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

	glGenVertexArrays(1, &lines_vao);
	glBindVertexArray(lines_vao);
	glGenBuffers(1, &lines_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, lines_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 18, lines_data,
		GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

	udpate_cube_and_lines();

	jds::print_opengl_error();

	printf("GL Init Done!\n");

	fflush(stdout);
}

//------------------------------------------------------------------------------
void GfxBase::gfx_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wsp->use();
	dfw->draw();

	psp->use();
	psp->set_uniform((char *)"color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	glBindVertexArray(cube_vao);
	glDrawArrays(GL_LINES, 0, 24);

	glDisable(GL_BLEND);
	glLineWidth(0.95f);

	psp->use();
	psp->set_uniform((char *)"color", glm::vec4(0.9f, 0.0f, 0.0f, 1.0f));
	glBindVertexArray(lines_vao);
	glDrawArrays(GL_LINES, 0, 6);

	psp->set_uniform((char *)"color", glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
	slgfx->draw();

	glEnable(GL_BLEND);
	glLineWidth(1.25f);

	frames++;
}

//------------------------------------------------------------------------------
void GfxBase::update_camera()
{
	double delta_t = timer->update();

	glm::mat4 transform(1.0f);

	if(yaw_cw)
	{
		transform = glm::rotate(transform, (float)delta_t * -yaw_rate, up);
	}
	else if(yaw_ccw)
	{
		transform = glm::rotate(transform, (float)delta_t * yaw_rate, up);
	}

	if(yaw_cw || yaw_ccw)
	{
		front = vec3(transform * glm::vec4(front, 1.0f));
		right = vec3(transform * glm::vec4(right, 1.0f));
	}

	transform = mat4(1.0f);
	if(pitch_up)
	{
		transform = glm::rotate(transform, (float)delta_t * -pitch_rate, right);
	}
	else if(pitch_down)
	{
		transform = glm::rotate(transform, (float)delta_t * pitch_rate, right);
	}

	if(pitch_down || pitch_up)
	{
		front = vec3(transform * glm::vec4(front, 1.0f));
		up = vec3(transform * glm::vec4(up, 1.0f));
	}

	transform = mat4(1.0f);
	if(roll_left)
	{
		transform = glm::rotate(transform, (float)delta_t * -roll_rate, front);
	}
	else if(roll_right)
	{
		transform = glm::rotate(transform, (float)delta_t * roll_rate, front);
	}

	if(roll_left || roll_right)
	{
		right = vec3(transform * glm::vec4(right, 1.0f));
		up = vec3(transform * glm::vec4(up, 1.0f));
	}

	if(forward)
	{
		eye = eye + (front * vel * (float)delta_t);
	}
	else if(back)
	{
		eye = eye - (front * vel * (float)delta_t);
	}

	if(strafe_right)
	{
		eye = eye - (right * vel * (float)delta_t);
	}
	else if(strafe_left)
	{
		eye = eye + (right * vel * (float)delta_t);
	}

	if(strafe_up)
	{
		eye = eye + (up * vel * (float)delta_t);
	}
	else if(strafe_down)
	{
		eye = eye - (up * vel * (float)delta_t);
	}

	if(yaw_cw || yaw_ccw || pitch_down || pitch_up || roll_left || roll_right ||
		forward || back || strafe_down || strafe_left || strafe_right ||
		strafe_up)
	{
		target = eye + front;

		picker = eye + front * picker_length;
		sdata->find_nearest_index(picker, picker_ind);

		ppos = sdata->p[picker_ind.x][picker_ind.y][picker_ind.z];
		pvel = sdata->v[picker_ind.x][picker_ind.y][picker_ind.z];

		/*
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z,			// camera location
				target.x, target.y, target.z,	// camera target
				up.x, up.y, up.z);				// camera up unit vector
				*/
		MV = glm::lookAt(eye, target, up);

		normal_matrix = glm::mat3(glm::vec3(MV[0]), glm::vec3(MV[1]),
			glm::vec3(MV[2]));

		psp->use();
		psp->set_uniform((char *)"MV", MV);
		wsp->use();
		wsp->set_uniform((char *)"light_pos", glm::vec4(eye, 1.0f));
		wsp->set_uniform((char *)"MV", MV);
		wsp->set_uniform((char *)"normal_matrix", normal_matrix);

		s2sp->use();
		s2sp->set_uniform((char *)"MV", MV);
		s2sp->set_uniform((char *)"normal_matrix", normal_matrix);
		s2sp->set_uniform((char *)"light_pos", glm::vec4(eye, 1.0f));

		udpate_cube_and_lines();
	}
}

//------------------------------------------------------------------------------
void GfxBase::load_files()
{
	sdata = new JdsVtkScalar();
	sgfx = new JdsVtkScalarGfx(sdata);
	
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
	data_root = string("d:/vtk_con");
#else
	data_root = string("/data/vtk_con");
#endif

	sdata->load_vtk((char *)(data_root + "/q.0000100.vtk").c_str());
	sdata->load_scalar_lazy((char *)(data_root + "/q.0000100.lazy").c_str(),
		sdata->point_count);
	sdata->find_bounds();
	sdata->print_bounds();
	sdata->convert_scalars_to_vectors();
	sdata->calc_pos_deltas();
	sdata->convert_deltas_to_cubes();
	sdata->calc_cube_verts();
	sdata->calc_border_points(2);

	sdata->find_nearest_index(picker, picker_ind);

	ppos = sdata->p[picker_ind.x][picker_ind.y][picker_ind.z];
	pvel = sdata->v[picker_ind.x][picker_ind.y][picker_ind.z];

	sgfx->gen_wing_vao();
	sgfx->gen_hh_vao();

	sl = new SeedLine();
	slgfx = new SeedLineGfx(sl);
	slgfx->update();

	dfw = new dragonfly_wing((char *)data_root.c_str());
	dfw->load(100);
}

//------------------------------------------------------------------------------
void GfxBase::udpate_cube_and_lines()
{
	lines_data[0] = ppos.x;
	lines_data[1] = ppos.y;
	lines_data[2] = sdata->all_min.z;

	lines_data[3] = ppos.x;
	lines_data[4] = ppos.y;
	lines_data[5] = sdata->all_max.z;

	lines_data[6] = ppos.x;
	lines_data[7] = sdata->all_min.y;
	lines_data[8] = ppos.z;

	lines_data[9] = ppos.x;
	lines_data[10] = sdata->all_max.y;
	lines_data[11] = ppos.z;

	lines_data[12] = sdata->all_min.x;
	lines_data[13] = ppos.y;
	lines_data[14] = ppos.z;

	lines_data[15] = sdata->all_max.x;
	lines_data[16] = ppos.y;
	lines_data[17] = ppos.z;

	sgfx->get_single_cube_verts(picker_ind, cube_data);

	glBindBuffer(GL_ARRAY_BUFFER, lines_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 18, lines_data,
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 72, cube_data,
		GL_DYNAMIC_DRAW);
}

//------------------------------------------------------------------------------
void GfxBase::print_cube_verts()
{
	for(unsigned int i = 0; i < 24; i++)
	{
		printf("cube vert [%d] = (%f, %f, %f)\n", i, cube_data[i * 3],
			cube_data[i * 3 + 1], cube_data[i * 3 + 2]);
	}
}

