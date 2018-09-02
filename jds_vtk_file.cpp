#include "JdsVtkFile.h"

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

//------------------------------------------------------------------------------
JdsVtkFile::JdsVtkFile()
{
}

//------------------------------------------------------------------------------
// converts raw data arrays to 3D vectors of type glm::vec3
void JdsVtkFile::convert_to_vectors()
{
	// allocate vector memory
	// vector[x][y][z]
	p.resize(dim.x);
	v.resize(dim.x);
	for(unsigned int i = 0; i < dim.x; i++)
	{
		p[i].resize(dim.y);
		v[i].resize(dim.y);
		for(unsigned int j = 0; j < dim.y; j++)
		{
			p[i][j].resize(dim.z);
			v[i][j].resize(dim.z);
		}
	}

	unsigned int n = 0, i = 0, j = 0, k = 0;
	// load the data to the vectors
	for(k = 0; k < dim.z; k++)
	{
		for(j = 0; j < dim.y; j++)
		{
			for(i = 0; i < dim.x; i++)
			{
				p[i][j][k].x = point[n * 3];
				p[i][j][k].y = point[n * 3 + 1];
				p[i][j][k].z = point[n * 3 + 2];
				v[i][j][k].x = vel[n * 3];
				v[i][j][k].y = vel[n * 3 + 1];
				v[i][j][k].z = vel[n * 3 + 2];
				n++;
			}
			i = 0;
		}
		j = 0;
	}

	cout << "Done converting to vectors\n";
	fflush(stdout);
}

//------------------------------------------------------------------------------
// VTK binary file loader
void JdsVtkFile::load_vtk(char *f)
{
	point_count = 0;

	// try to open the file
	ifstream stream(f, std::ios::in | std::ios::binary);

	// check to see if the file open worked
	if(!stream)
	{
		cerr << "Error opening VTK file: " << f << endl;
		return;
	}

	// start by pulling lines off the file
	string line, token;
	getline(stream, line); //# vtk DataFile Version 2.0
	getline(stream, line); //vtk output
	getline(stream, line); //BINARY
	if(line != "BINARY")
	{
		cerr << "ERROR! this isn't a binary VTK file: " << f << endl;
		return;
	}
	getline(stream, line); //DATASET STRUCTURED_GRID
	getline(stream, line); //DIMENSIONS 200 176 216
	// turns the string into an input stream
	istringstream line_stream(line);
	// pull chars until space
	line_stream >> token;
	if(token != "DIMENSIONS")
	{
		cerr << "ERROR! Couldn't find 'DIMENSIONS' in VTK file: " << f << endl;
		return;
	}
	line_stream >> token;
	dim.x = (unsigned int)atoi(token.c_str());
	line_stream >> token;
	dim.y = (unsigned int)atoi(token.c_str());
	line_stream >> token;
	dim.z = (unsigned int)atoi(token.c_str());
	getline(stream, line); //POINTS 7603200 float
	line_stream.clear();
	line_stream.str(line);
	line_stream >> token;
	line_stream >> token;
	point_count = (GLint)atoi(token.c_str());

	// didn't find anything
	if(point_count == 0)
	{
		cerr << "ERROR! Couldn't find 'POINTS' in VTK file: " << f << endl;
		return;
	}
	line_stream >> token;

	point = new GLfloat[point_count * 3];
	vel = new GLfloat[point_count * 3];
	stream.read((char *)point, sizeof(GLfloat) * point_count * 3);

	getline(stream, line); //
	getline(stream, line); //POINT_DATA 7603200
	getline(stream, line); //VECTORS vectors float

	stream.read((char *)vel, sizeof(GLfloat) * point_count * 3);

	stream.close();

	change_endianess();
}

//------------------------------------------------------------------------------
// VTK binary files are big endian by default
void JdsVtkFile::change_endianess()
{
	for(unsigned int i = 0; i < point_count * 3; i++)
	{
		byte_swap4(&(point[i]));
		byte_swap4(&(vel[i]));
	}
}

//------------------------------------------------------------------------------
// swaps the bytes in a 4 byte block of memory starting at b
void JdsVtkFile::byte_swap4(void *b)
{
	char b1[4], b2[4];

	memcpy(b1, b, 4);

	b2[0] = b1[3];
	b2[1] = b1[2];
	b2[2] = b1[1];
	b2[3] = b1[0];

	memcpy(b, b2, 4);
}
