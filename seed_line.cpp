#include "SeedLine.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <ctime>
#include <cstdio>

#ifdef _MSC_VER
#pragma warning(disable : 4996) // crt is secure, disable the warning
#define snprintf _snprintf_s
#endif

//------------------------------------------------------------------------------
SeedLine::SeedLine()
{
}

//------------------------------------------------------------------------------
SeedLine::~SeedLine()
{
}

//------------------------------------------------------------------------------
// parm is from 0.0f to 1.0f, this is the parametric position to find
// returns the interpolated position
// calc_length() must have been run after the last point was added and before
// is run
vec3 SeedLine::find_pos(float parm)
{
	unsigned int n;

	// oops!
	if(parm < 0 || parm > 1.0f) return vec3(0.0f);

	for(n = 1; n < para.size() - 1; n++)
	{
		if(para[n - 1] <= parm && para[n] >= parm)
			break;
	}

	float rat = (parm - para[n - 1]) / (para[n] - para[n - 1]);
	vec3 interp = pos[n -1] + (rat * (pos[n] - pos[n - 1]));

	return interp;
}

//------------------------------------------------------------------------------
void SeedLine::calc_length()
{
	length = 0.0f;
	para.clear();

	para.resize(pos.size());
	if(para.size() > 0)
	{
		para[0] = 0.0f;
	}

	for(unsigned int n = 1; n < pos.size(); n++)
	{
		length += glm::distance(pos[n -1], pos[n]);
		para[n] = length;
	}
}

//------------------------------------------------------------------------------
void SeedLine::add(vec3 p, uvec3 i)
{
	pos.push_back(p);
	ind.push_back(i);
}

//------------------------------------------------------------------------------
void SeedLine::clear()
{
	pos.clear();
	ind.clear();
}

//------------------------------------------------------------------------------
// p is the VTK position data
// ind needs to be already loaded for this to work
void SeedLine::calc_pos_from_ind(vector<vector<vector<vec3> > > &p)
{
	for(unsigned int n = 0; n < ind.size(); n++)
	{
		pos.push_back(p[ind[n].x][ind[n].y][ind[n].z]);
	}
}

//------------------------------------------------------------------------------
void SeedLine::save(const char *filename)
{
	unsigned int *i, size = (unsigned int)ind.size() * 3;

	if(size == 0) return;

	i = new unsigned int[size];

	for(unsigned int n = 0; n < ind.size(); n++)
	{
		i[n * 3] = ind[n].x;
		i[n * 3 + 1] = ind[n].y;
		i[n * 3 + 2] = ind[n].z;
	}

	ofstream out(filename, std::ios::out | std::ios::binary);

	out.write((char *)i, sizeof(unsigned int) * size);

	out.close();

	cout << "Saved seedline: " << filename << endl;

	delete [] i;
}

//------------------------------------------------------------------------------
void SeedLine::save()
{
	time_t rawtime;
	struct tm *timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	unsigned int *i, size = (unsigned int)ind.size() * 3;

	if(size == 0) return;

	char ftime[64];

	strftime(ftime, 64, "%Y.%m.%d_%H.%M.%S", timeinfo);

	char filename[128];

	snprintf(filename, 128, "seedline_%s.raw", ftime);

	i = new unsigned int[size];

	for(unsigned int n = 0; n < ind.size(); n++)
	{
		i[n * 3] = ind[n].x;
		i[n * 3 + 1] = ind[n].y;
		i[n * 3 + 2] = ind[n].z;
	}

	ofstream out(filename, std::ios::out | std::ios::binary);

	out.write((char *)i, sizeof(unsigned int) * size);

	out.close();

	cout << "Saved seedline: " << filename << endl;

	delete [] i;
}

//------------------------------------------------------------------------------
void SeedLine::load(const char *filename)
{
	unsigned int *i, size;

	ifstream in(filename, std::ios::in | std::ios::binary);

	in.seekg (0, std::ios::end);
	size = (unsigned int)in.tellg() / sizeof(unsigned int);
	in.seekg (0, std::ios::beg);

	i = new unsigned int[size];

	in.read((char *)i, size * sizeof(unsigned int));

	in.close();

	for(unsigned int n = 0; n < size / 3; n++)
	{
		ind.push_back(uvec3(i[n * 3], i[n * 3 + 1], i[n * 3 + 2]));
	}

	delete [] i;
}
