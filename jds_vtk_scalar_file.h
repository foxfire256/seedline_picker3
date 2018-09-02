#ifndef jds_vtk_scalar_file_H
#define jds_vtk_scalar_file_H

#include "jds_opengl.hpp"

#include <vector>
using std::vector;

class jds_vtk_scalar_file
{
public:
	GLbyte *scalar;
	vector<vector<vector<GLint> > > w;

	jds_vtk_scalar_file();

	void load_scalar_lazy(char *f, unsigned int count);
	void write_scalar_lazy(char *f, unsigned int count);
};

#endif // jds_vtk_scalar_file_H
