#ifndef JDSVTKSCALARFILE_H
#define JDSVTKSCALARFILE_H

#include "jds_opengl.hpp"

#include <vector>
using std::vector;

class JdsVtkScalarFile
{
public:
	GLbyte *scalar;
	vector<vector<vector<GLint> > > w;

	JdsVtkScalarFile();

	void load_scalar_lazy(char *f, unsigned int count);
	void write_scalar_lazy(char *f, unsigned int count);
};

#endif // JDSVTKSCALARFILE_H
