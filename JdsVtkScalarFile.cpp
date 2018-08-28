#include "JdsVtkScalarFile.h"

#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sstream>
using std::istringstream;
#include <string>
using std::string;

JdsVtkScalarFile::JdsVtkScalarFile()
{
}




//------------------------------------------------------------------------------
// I couldn't get Paraview to like my VTK file with scalars so I just wrote the
// raw data to three separate files, this loads them
void JdsVtkScalarFile::load_scalar_lazy(char *file, unsigned int count)
{
	string filename(file);
	string fname = filename + ".scalars";
	ifstream stream(fname.c_str(), std::ios::in | std::ios::binary);
	// check to see if the file open worked
	if(!stream)
	{
		cerr << "Error opening file: " << fname << endl;
		return;
	}
	scalar = new GLbyte[(unsigned int)count];
	stream.read((char *)scalar, count * sizeof(GLbyte));
	stream.close();
}


//------------------------------------------------------------------------------
// I couldn't get Paraview to like my VTK file with scalars so I just wrote the
// raw data to three separate files, this writes them
void JdsVtkScalarFile::write_scalar_lazy(char *f, unsigned int count)
{
	string filename(f);
	string fname = filename + ".scalars";
	ofstream out(fname.c_str(), std::ios::out | std::ios::binary);
	// check to see if the file open worked
	if(!out)
	{
		cerr << "Error opening file: " << fname << endl;
		return;
	}
	out.write((char *)scalar, count * sizeof(GLbyte));
	out.close();
}
