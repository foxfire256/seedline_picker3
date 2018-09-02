#ifndef seed_line_H
#define seed_line_H

#include <vector>
using std::vector;
#define GLM_FORCE_ONLY_XYZW
#include <glm/glm.hpp>
using glm::vec3;
using glm::uvec3;

class seed_line
{
public:
	seed_line();
	~seed_line();

	void add(vec3 p, uvec3 i);
	void save();
	void save(const char *filename);
	void load(const char *filename);
	void calc_pos_from_ind(vector<vector<vector<vec3> > > &p);
	void clear();
	void calc_length();
	float get_length(){return length;}
	vec3 find_pos(float parm);

	vector<vec3> pos;
	vector<uvec3> ind;

private:
	float length;
	vector<float> para;
};

#endif // seed_line_H
