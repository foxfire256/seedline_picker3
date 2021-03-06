#ifndef jds_vtk_scalar_H
#define jds_vtk_scalar_H

#include "jds_opengl.hpp"

#define GLM_FORCE_ONLY_XYZW
#include <glm/glm.hpp>
using glm::vec3;
using glm::uvec3;
#include <vector>
using std::vector;

#include "jds_vtk_file.h"
#include "jds_vtk_scalar_file.h"

//------------------------------------------------------------------------------
// this was designed for ease of use at the expense of lots of memory
class jds_vtk_scalar : public jds_vtk_file, public jds_vtk_scalar_file
{
public:
	// data bounds
	vec3 all_min, all_max, all_center, wing_min, wing_max, wing_center;

	// position and wing in 3d
	// w[x][y][z]

	vector<vector<vector<GLint> > > w;

	vector<vector<vector<vec3> > > p_delta_upper, p_delta_lower;
	vector<vector<vector<vector<vec3> > > > cv; // cube verts
	unsigned int cv_per_point;
	vector<uvec3> border_ind;

	// meant to be drawn as lines
	GLfloat *cube_vert_array;
	unsigned int cube_vert_count;
	GLfloat *hedge_hog_array, hedge_hog_scale;
	unsigned int hedge_hog_count;
	vector<vec3> hhv;

	vector<unsigned int> cube_indicies;

	jds_vtk_scalar();
	~jds_vtk_scalar();

	void find_bounds();
	void print_bounds();

	void calc_pos_deltas();
	void convert_deltas_to_cubes();
	void calc_cube_verts();
	void calc_border_points(unsigned int border_w);
	void convert_scalars_to_vectors();
	void find_nearest_index(vec3 loc, uvec3 &ind);

	void print_debug_file();
};

#endif // JDS_LAZY_DATAFILE_H
