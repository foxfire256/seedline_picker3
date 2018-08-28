#ifndef DRAGONFLY_WING_H
#define DRAGONFLY_WING_H

#define GLM_FORCE_ONLY_XYZW
#include "jds_opengl.hpp"

#include <string>

class dragonfly_wing
{
public:
	dragonfly_wing();
	dragonfly_wing(char *data_root_in);
	~dragonfly_wing();

	void load(int i);
	void set_data_root(std::string s){data_root = s;}
	void draw();

private:
	std::string data_root;
	float *vert, *norm;
	unsigned int *ind;
	unsigned int vert_count, ind_count;
	GLuint vao, vbo_vert, vbo_norm, vbo_ind;
};

#endif // DRAGONFLY_WING_H
