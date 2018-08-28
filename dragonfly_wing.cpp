#include "dragonfly_wing.h"

using std::string;
#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
#pragma warning(disable : 4996) // crt is secure, disable the warning
#define snprintf _snprintf_s
#endif

//------------------------------------------------------------------------------
dragonfly_wing::dragonfly_wing()
{
#ifdef _MSC_VER
	data_root = string("c:\\vtk_con");
#else
	data_root = string("/data/vtk_con");
#endif
}

//------------------------------------------------------------------------------
dragonfly_wing::dragonfly_wing(char *data_root_in)
{
	data_root = string(data_root_in);
}

//------------------------------------------------------------------------------
dragonfly_wing::~dragonfly_wing()
{
	glDeleteBuffers(1, &vbo_vert);
	glDeleteBuffers(1, &vbo_norm);
	glDeleteBuffers(1, &vbo_ind);
	glDeleteVertexArrays(1, &vao);
}

//------------------------------------------------------------------------------
void dragonfly_wing::load(int i)
{
	char vfile[128], nfile[128], ifile[128];

	snprintf(vfile, 128, "%s/wing_raw/q.0000%d.wing.vert", data_root.c_str(),i);
	snprintf(nfile, 128, "%s/wing_raw/q.0000%d.wing.norm", data_root.c_str(),i);
	snprintf(ifile, 128, "%s/wing_raw/q.0000%d.wing.ind", data_root.c_str(),i);

	FILE *v_f, *i_f, *n_f;
	v_f = fopen(vfile, "rb");
	n_f = fopen(nfile, "rb");
	i_f = fopen(ifile, "rb");

	if(v_f == NULL || n_f == NULL || i_f == NULL)
	{
		printf("At least one of these failed to load!\n");
		printf("%s\n", vfile);
		printf("%s\n", nfile);
		printf("%s\n", ifile);
		fflush(stdout);
	}

	unsigned int vsize, nsize, isize;

	fseek(v_f, 0, SEEK_END);
	vsize = ftell(v_f);
	rewind(v_f);

	fseek(n_f, 0, SEEK_END);
	nsize = ftell(n_f);
	rewind(n_f);

	fseek(i_f, 0, SEEK_END);
	isize = ftell(i_f);
	rewind(i_f);

	vert_count = vsize / 3 / sizeof(float);
	vert = new float[vsize / sizeof(float)];
	norm = new float[nsize / sizeof(float)];
	ind_count = isize / sizeof(int);
	ind = new unsigned int[ind_count];

	fread(vert, sizeof(float), vert_count * 3, v_f);
	fread(norm, sizeof(float), vert_count * 3, n_f);
	fread(ind, sizeof(int), ind_count, i_f);

	fclose(v_f);
	fclose(n_f);
	fclose(i_f);

	// OpenGL Init()
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo_vert);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vert_count * 3, vert,
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

	glGenBuffers(1, &vbo_norm);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vert_count * 3, norm,
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);

	glGenBuffers(1, &vbo_ind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ind_count, ind,
		GL_STATIC_DRAW);

	glFlush();
	glFinish();

	delete [] vert;
	delete [] norm;
	delete [] ind;

	jds::print_opengl_error();
}

//------------------------------------------------------------------------------
void dragonfly_wing::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, ind_count, GL_UNSIGNED_INT, 0);
}
