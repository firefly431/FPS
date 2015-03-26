#ifndef _INCLUDED_VEC3_H
#define _INCLUDED_VEC3_H

struct vec4 {
	double x, y, z, w;

	vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
};

#endif