#ifndef JDSVTKFILE_H
#define JDSVTKFILE_H

#include "jds_opengl.hpp"

#define GLM_FORCE_ONLY_XYZW
#include <glm/glm.hpp>
using glm::uvec3;
using glm::vec3;
#include <vector>
using std::vector;

class JdsVtkFile
{
public:
	GLuint point_count;
	// array dimensions
	uvec3 dim;
	// raw data
	GLfloat *point, *vel;

	// p[x][y][z]
	vector<vector<vector<vec3> > > p;
	vector<vector<vector<vec3> > > v;

	JdsVtkFile();

	void load_vtk(char *f);
	void convert_to_vectors();

private:
	void change_endianess();
	void byte_swap4(void *b);
};

#endif // JDSVTKFILE_H
