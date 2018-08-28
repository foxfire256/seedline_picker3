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

#include "JdsVtkScalar.h"

//------------------------------------------------------------------------------
JdsVtkScalar::JdsVtkScalar()
{
	point_count = 0;
	dim = uvec3(0);

	hedge_hog_scale = 0.2f;

	point = NULL;
	scalar = NULL;
}

//------------------------------------------------------------------------------
JdsVtkScalar::~JdsVtkScalar()
{
	point_count = 0;
	dim = uvec3(0);

	if(point) delete [] point;
	if(scalar) delete [] scalar;
	if(vel) delete [] vel;
}

//------------------------------------------------------------------------------
void JdsVtkScalar::print_debug_file()
{
	FILE *f = fopen("debug.txt", "w");

	for(unsigned int i = 0; i < point_count; i++)
	{
		fprintf(f, "%d, %f, %f, %f, %d,\n", i, point[i * 3], point[i * 3 + 1],
				point[i * 3 + 2], scalar[i]);
	}

	fclose(f);
}

//------------------------------------------------------------------------------
// this assumes a rectangular, structured grid
// this is a 3d bisection method
void JdsVtkScalar::find_nearest_index(vec3 loc, uvec3 &ind)
{
	uvec3 i, delta;
	int i_done = 0, j_done = 0, k_done = 0;

	if(loc.x > all_max.x || loc.y > all_max.y || loc.z > all_max.z ||
		loc.x < all_min.x || loc.y < all_min.y || loc.z < all_min.z)
	{
		ind = uvec3(0);
		return;
	}

	i.x = delta.x = dim.x / 2;
	i.y = delta.y = dim.y / 2;
	i.z = delta.z = dim.z / 2;

	while(1)
	{
		if(!i_done)
		{
			// find the new index
			delta.x = delta.x / 2 + delta.x % 2;
			if(loc.x > p[i.x][i.y][i.z].x)
			{
				i.x += delta.x;
			}
			else
			{
				i.x -= delta.x;
			}

			// see if we are within the bounds of a point
			if(loc.x > p_delta_lower[i.x][i.y][i.z].x &&
				loc.x <= p_delta_upper[i.x][i.y][i.z].x)
			{
				i_done = 1;
			}
		}
		if(!j_done)
		{
			// find the new index
			delta.y = delta.y / 2 + delta.y % 2;
			if(loc.y > p[i.x][i.y][i.z].y)
			{
				i.y += delta.y;
			}
			else
			{
				i.y -= delta.y;
			}

			// see if we are within the bounds of a point
			if(loc.y > p_delta_lower[i.x][i.y][i.z].y &&
				loc.y <= p_delta_upper[i.x][i.y][i.z].y)
			{
				j_done = 1;
			}
		}
		if(!k_done)
		{
			// find the new index
			delta.z = delta.z / 2 + delta.z % 2;
			if(loc.z > p[i.x][i.y][i.z].z)
			{
				i.z += delta.z;
			}
			else
			{
				i.z -= delta.z;
			}

			// see if we are within the bounds of a point
			if(loc.z > p_delta_lower[i.x][i.y][i.z].z &&
				loc.z <= p_delta_upper[i.x][i.y][i.z].z)
			{
				k_done = 1;
			}
		}

		if(i_done && j_done && k_done)
			break;
	}

	ind = i;
}

//------------------------------------------------------------------------------
// finds the points in the flow that are border_w from the wing
// TODO: maybe tweak the check boundaries part of this a bit
void JdsVtkScalar::calc_border_points(unsigned int border_w)
{
	vector<vector<vector<GLbyte> > > bx, by, bz;

	// allocate memory
	bx.resize(dim.x);
	by.resize(dim.x);
	bz.resize(dim.x);
	for(unsigned int i = 0; i < dim.x; i++)
	{
		bx[i].resize(dim.y);
		by[i].resize(dim.y);
		bz[i].resize(dim.y);
		for(unsigned int j = 0; j < dim.y; j++)
		{
			bx[i][j].resize(dim.z);
			by[i][j].resize(dim.z);
			bz[i][j].resize(dim.z);
		}
	}

	// initialize
	for(unsigned int i = 0; i < dim.x; i++)
	{
		for(unsigned int j = 0; j < dim.y; j++)
		{
			for(unsigned int k = 0; k < dim.z; k++)
			{
				bx[i][j][k] = 0;
				by[i][j][k] = 0;
				bz[i][j][k] = 0;
			}
		}
	}

	// check boundaries
	for(unsigned int i = border_w; i < dim.x - border_w; i++)
	{
		for(unsigned int j = border_w; j < dim.y - border_w; j++)
		{
			for(unsigned int k = border_w; k < dim.z - border_w; k++)
			{
				for(int n = border_w; n >= 0; n--)
				{
					if((w[i - n][j][k] || w[i + n][j][k] ||
						w[i][j - n][k] || w[i][j + n][k] ||
						w[i][j][k - n] || w[i][j][k + n] ||
						/*
						w[i - n][j - n][k - n] ||
						w[i + n][j + n][k + n] ||
						w[i + n][j - n][k - n] ||
						w[i - n][j + n][k - n] ||
						w[i - n][j - n][k + n] ||
						w[i + n][j + n][k - n] ||
						w[i - n][j + n][k + n] ||
						w[i + n][j - n][k + n] ||
							*/
						w[i - n][j - n][k] ||
						w[i + n][j + n][k] ||
						w[i + n][j - n][k] ||
						w[i - n][j + n][k] ||

						w[i - n][j][k - n] ||
						w[i + n][j][k + n] ||
						w[i + n][j][k - n] ||
						w[i - n][j][k + n] ||

						w[i][j - n][k - n] ||
						w[i][j + n][k + n] ||
						w[i][j + n][k - n] ||
						w[i][j - n][k + n]
						) && w[i][j][k] != 1)
					{
						bx[i][j][k] = 1;
					}
				}

				if(bx[i][j][k] || by[i][j][k] || bz[i][j][k])
				{
					border_ind.push_back(uvec3(i, j, k));
				}
			}
		}
	}

	// allocate memory for hedge hog
	hedge_hog_count = border_ind.size();
	hedge_hog_array = new GLfloat[hedge_hog_count * 3 * 2];
	hhv.resize(hedge_hog_count * 2);

	// set hedge hog vertex values
	for(unsigned int i = 0; i < border_ind.size(); i++)
	{
		hedge_hog_array[i * 3] =
			p[border_ind[i].x][border_ind[i].y][border_ind[i].z].x;
		hedge_hog_array[i * 3 + 1] =
			p[border_ind[i].x][border_ind[i].y][border_ind[i].z].y;
		hedge_hog_array[i * 3 + 2] =
			p[border_ind[i].x][border_ind[i].y][border_ind[i].z].z;

		hhv[i * 2] = p[border_ind[i].x][border_ind[i].y][border_ind[i].z];

		hedge_hog_array[i * 3 + 3] = hedge_hog_array[i * 3] +
			v[border_ind[i].x][border_ind[i].y][border_ind[i].z].x *
			hedge_hog_scale;
		hedge_hog_array[i * 3 + 4] = hedge_hog_array[i * 3 + 1] +
			v[border_ind[i].x][border_ind[i].y][border_ind[i].z].y *
			hedge_hog_scale;
		hedge_hog_array[i * 3 + 5] = hedge_hog_array[i * 3 + 2] +
			v[border_ind[i].x][border_ind[i].y][border_ind[i].z].z *
			hedge_hog_scale;

		hhv[i * 2 + 1] = hhv[i * 2] +
			v[border_ind[i].x][border_ind[i].y][border_ind[i].z] *
			hedge_hog_scale;
	}

	cout << "Done calc_border_points()" << border_ind.size() << "\n";
	fflush(stdout);
}

//------------------------------------------------------------------------------
// generates a vertex array for all cubes in the wing
// the vertices are meant to be drawn as lines
void JdsVtkScalar::calc_cube_verts()
{
	unsigned int wing_point_count = 0, verts_per_point = 12 * 2;

	// count wing points
	for(unsigned int i = 0; i < point_count; i++)
	{
		if(scalar[i])
			wing_point_count++;
	}

	// TODO: check to make sure wing_point_count * verts_per_point !> UINT_MAX
	cube_vert_count = wing_point_count * verts_per_point;
	cout << "cube_vert_count = " << cube_vert_count << endl;
	cube_vert_array = new GLfloat[cube_vert_count * 3];

	unsigned int i = 0, j = 0, k = 0, n = 0;
	for(k = 0; k < dim.z; k++)
	{
		for(j = 0; j < dim.y; j++)
		{
			for(i = 0; i < dim.x; i++)
			{
				// wing point
				if(w[i][j][k] == 1)
				{
					for(unsigned int m = 0; m < cube_indicies.size(); m++)
					{
						cube_vert_array[n] = cv[i][j][k][cube_indicies[m]].x;
						n++;
						cube_vert_array[n] = cv[i][j][k][cube_indicies[m]].y;
						n++;
						cube_vert_array[n] = cv[i][j][k][cube_indicies[m]].z;
						n++;
					}
				}
			}
			i = 0;
		}
		j = 0;
	}

	cout << "Done calc_cube_verts()\n";
	fflush(stdout);
}

//------------------------------------------------------------------------------
void JdsVtkScalar::convert_deltas_to_cubes()
{
	// allocate cube verts memory
	cv_per_point = 8;
	cv.resize(dim.x);
	for(unsigned int i = 0; i < cv.size(); i++)
	{
		cv[i].resize(dim.y);
		for(unsigned int j = 0; j < cv[i].size(); j++)
		{
			cv[i][j].resize(dim.z);
			for(unsigned int k = 0; k < cv[i][j].size(); k++)
			{
				cv[i][j][k].resize(cv_per_point);
			}
		}
	}

	// calculate cubes
	unsigned int i = 0, j = 0, k = 0, n = 0;
	for(k = 0; k < dim.z; k++)
	{
		for(j = 0; j < dim.y; j++)
		{
			for(i = 0; i < dim.x; i++)
			{
				n = 0;
				cv[i][j][k][n] = p_delta_lower[i][j][k];
				n++;
				cv[i][j][k][n] = cv[i][j][k][0];
				cv[i][j][k][n].x = p_delta_upper[i][j][k].x;
				n++;
				cv[i][j][k][n] = cv[i][j][k][0];
				cv[i][j][k][n].x = p_delta_upper[i][j][k].x;
				cv[i][j][k][n].y = p_delta_upper[i][j][k].y;
				n++;
				cv[i][j][k][n] = cv[i][j][k][0];
				cv[i][j][k][n].y = p_delta_upper[i][j][k].y;

				n++;
				cv[i][j][k][n] = cv[i][j][k][0];
				cv[i][j][k][n].z = p_delta_upper[i][j][k].z;
				n++;
				cv[i][j][k][n] = cv[i][j][k][4];
				cv[i][j][k][n].x = p_delta_upper[i][j][k].x;
				n++;
				cv[i][j][k][n] = cv[i][j][k][4];
				cv[i][j][k][n].x = p_delta_upper[i][j][k].x;
				cv[i][j][k][n].y = p_delta_upper[i][j][k].y;
				n++;
				cv[i][j][k][n] = cv[i][j][k][4];
				cv[i][j][k][n].y = p_delta_upper[i][j][k].y;
			}
			i = 0;
		}
		j = 0;
	}

	// TODO: free p_delta_upper and lower here
	/*
	for(unsigned int i = 0; i < dim.x; i++)
	{
		for(unsigned int j = 0; j < dim.y; j++)
		{
			p_delta_upper[i][j].clear();
		}
		p_delta_upper[i].clear();
	}
	*/
	//p_delta_upper.clear();

	// indicies for crude drawing
	// all front
	cube_indicies.push_back(0);
	cube_indicies.push_back(1);

	cube_indicies.push_back(1);
	cube_indicies.push_back(2);

	cube_indicies.push_back(2);
	cube_indicies.push_back(3);

	cube_indicies.push_back(3);
	cube_indicies.push_back(0);

	// all back
	cube_indicies.push_back(4);
	cube_indicies.push_back(5);

	cube_indicies.push_back(5);
	cube_indicies.push_back(6);

	cube_indicies.push_back(6);
	cube_indicies.push_back(7);

	cube_indicies.push_back(7);
	cube_indicies.push_back(4);

	// interconnect
	cube_indicies.push_back(0);
	cube_indicies.push_back(4);

	cube_indicies.push_back(1);
	cube_indicies.push_back(5);

	cube_indicies.push_back(2);
	cube_indicies.push_back(6);

	cube_indicies.push_back(3);
	cube_indicies.push_back(7);

	cout << "Done convert_deltas_to_cubes()\n";
	fflush(stdout);
}

//------------------------------------------------------------------------------
// finds the position deltas in each dimension for each point
// this will be used for drawing boxes at each point
void JdsVtkScalar::calc_pos_deltas()
{
	// allocate vector memory
	// vector[x][y][z]
	p_delta_upper.resize(dim.x);
	for(unsigned int i = 0; i < p_delta_upper.size(); i++)
	{
		p_delta_upper[i].resize(dim.y);
		for(unsigned int j = 0; j < p_delta_upper[i].size(); j++)
		{
			p_delta_upper[i][j].resize(dim.z);
		}
	}
	p_delta_lower.resize(dim.x);
	for(unsigned int i = 0; i < p_delta_lower.size(); i++)
	{
		p_delta_lower[i].resize(dim.y);
		for(unsigned int j = 0; j < p_delta_lower[i].size(); j++)
		{
			p_delta_lower[i][j].resize(dim.z);
		}
	}

	unsigned int i = 0, j = 0, k = 0;
	// x direction first
	for(k = 0; k < dim.z; k++)
	{
		for(j = 0; j < dim.y; j++)
		{
			for(i = 0; i < dim.x; i++)
			{
				// left edge
				if(i == 0)
				{
					p_delta_upper[i][j][k].x = p[i][j][k].x +
						(p[i + 1][j][k].x - p[i][j][k].x) / 2.0f;

					p_delta_lower[i][j][k].x = p[i][j][k].x -
						(p[i + 1][j][k].x - p[i][j][k].x) / 2.0f;
				}
				// right edge
				else if(i == dim.x - 1)
				{
					p_delta_upper[i][j][k].x = p[i][j][k].x +
						(p[i][j][k].x - p[i - 1][j][k].x) / 2.0f;

					p_delta_lower[i][j][k].x = p[i][j][k].x -
						(p[i][j][k].x - p[i - 1][j][k].x) / 2.0f;
				}
				// interior
				else
				{
					p_delta_upper[i][j][k].x = p[i][j][k].x +
						(p[i + 1][j][k].x - p[i][j][k].x) / 2.0f;

					p_delta_lower[i][j][k].x = p[i][j][k].x -
						(p[i][j][k].x - p[i - 1][j][k].x) / 2.0f;
				}
			}
			i = 0;
		}
		j = 0;
	}

	// y direction
	for(k = 0; k < dim.z; k++)
	{
		for(j = 0; j < dim.y; j++)
		{
			for(i = 0; i < dim.x; i++)
			{
				// left edge
				if(j == 0)
				{
					p_delta_upper[i][j][k].y = p[i][j][k].y +
						(p[i][j + 1][k].y - p[i][j][k].y) / 2.0f;

					p_delta_lower[i][j][k].y = p[i][j][k].y -
						(p[i][j + 1][k].y - p[i][j][k].y) / 2.0f;
				}
				// right edge
				else if(j == dim.y - 1)
				{
					p_delta_upper[i][j][k].y = p[i][j][k].y +
						(p[i][j][k].y - p[i][j - 1][k].y) / 2.0f;

					p_delta_lower[i][j][k].y = p[i][j][k].y -
						(p[i][j][k].y - p[i][j - 1][k].y) / 2.0f;
				}
				// interior
				else
				{
					p_delta_upper[i][j][k].y = p[i][j][k].y +
						(p[i][j + 1][k].y - p[i][j][k].y) / 2.0f;

					p_delta_lower[i][j][k].y = p[i][j][k].y -
						(p[i][j][k].y - p[i][j - 1][k].y) / 2.0f;
				}
			}
			i = 0;
		}
		j = 0;
	}

	// z direction
	for(k = 0; k < dim.z; k++)
	{
		for(j = 0; j < dim.y; j++)
		{
			for(i = 0; i < dim.x; i++)
			{
				// left edge
				if(k == 0)
				{
					p_delta_upper[i][j][k].z = p[i][j][k].z +
						(p[i][j][k + 1].z - p[i][j][k].z) / 2.0f;

					p_delta_lower[i][j][k].z = p[i][j][k].z -
						(p[i][j][k + 1].z - p[i][j][k].z) / 2.0f;
				}
				// right edge
				else if(k == dim.z - 1)
				{
					p_delta_upper[i][j][k].z = p[i][j][k].z +
						(p[i][j][k].z - p[i][j][k - 1].z) / 2.0f;

					p_delta_lower[i][j][k].z = p[i][j][k].z -
						(p[i][j][k].z - p[i][j][k - 1].z) / 2.0f;
				}
				// interior
				else
				{
					p_delta_upper[i][j][k].z = p[i][j][k].z +
						(p[i][j][k + 1].z - p[i][j][k].z) / 2.0f;

					p_delta_lower[i][j][k].z = p[i][j][k].z -
						(p[i][j][k].z - p[i][j][k - 1].z) / 2.0f;
				}
			}
			i = 0;
		}
		j = 0;
	}

	cout << "Done finding deltas\n";
	fflush(stdout);
}

//------------------------------------------------------------------------------
// converts raw data arrays to 3D vectors of type glm::vec3
void JdsVtkScalar::convert_scalars_to_vectors()
{
	JdsVtkFile::convert_to_vectors();

	// allocate vector memory
	// vector[x][y][z]
	w.resize(dim.x);
	for(unsigned int i = 0; i < dim.x; i++)
	{
		w[i].resize(dim.y);
		for(unsigned int j = 0; j < dim.y; j++)
		{
			w[i][j].resize(dim.z);
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
				w[i][j][k] = scalar[n];
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
// finds the bounds of the entire data set and just the wing
void JdsVtkScalar::find_bounds()
{
	all_min = vec3((GLfloat)FLT_MAX);
	wing_min = vec3((GLfloat)FLT_MAX);
	all_max = vec3((GLfloat)-FLT_MAX);
	wing_max = vec3((GLfloat)-FLT_MAX);

	for(unsigned int i = 0; i < point_count; i++)
	{
		if(point[i * 3] < all_min.x)
			all_min.x = point[i * 3];
		if(point[i * 3 + 1] < all_min.y)
			all_min.y = point[i * 3 + 1];
		if(point[i * 3 + 2] < all_min.z)
			all_min.z = point[i * 3 + 2];

		if(point[i * 3] > all_max.x)
			all_max.x = point[i * 3];
		if(point[i * 3 + 1] > all_max.y)
			all_max.y = point[i * 3 + 1];
		if(point[i * 3 + 2] > all_max.z)
			all_max.z = point[i * 3 + 2];

		if(scalar[i])
		{
			if(point[i * 3] < wing_min.x)
				wing_min.x = point[i * 3];
			if(point[i * 3 + 1] < wing_min.y)
				wing_min.y = point[i * 3 + 1];
			if(point[i * 3 + 2] < wing_min.z)
				wing_min.z = point[i * 3 + 2];

			if(point[i * 3] > wing_max.x)
				wing_max.x = point[i * 3];
			if(point[i * 3 + 1] > wing_max.y)
				wing_max.y = point[i * 3 + 1];
			if(point[i * 3 + 2] > wing_max.z)
				wing_max.z = point[i * 3 + 2];
		}
	}

	all_center.x = (all_max.x - all_min.x) / 2.0f + all_min.x;
	all_center.y = (all_max.y - all_min.y) / 2.0f + all_min.y;
	all_center.z = (all_max.z - all_min.z) / 2.0f + all_min.z;

	wing_center.x = (wing_max.x - wing_min.x) / 2.0f + wing_min.x;
	wing_center.y = (wing_max.y - wing_min.y) / 2.0f + wing_min.y;
	wing_center.z = (wing_max.z - wing_min.z) / 2.0f + wing_min.z;

	cout << "Done loading and finding bounds\n";
	fflush(stdout);
}

//------------------------------------------------------------------------------
void JdsVtkScalar::print_bounds()
{
	printf("all min (%f, %f, %f)\n", all_min.x, all_min.y, all_min.z);
	printf("all max (%f, %f, %f)\n", all_max.x, all_max.y, all_max.z);
	printf("all center (%f, %f, %f)\n", all_center.x, all_center.y,
		all_center.z);

	printf("wing min (%f, %f, %f)\n", wing_min.x, wing_min.y, wing_min.z);
	printf("wing max (%f, %f, %f)\n", wing_max.x, wing_max.y, wing_max.z);
	printf("wing center (%f, %f, %f)\n", wing_center.x, wing_center.y,
		wing_center.z);

	fflush(stdout);
}





