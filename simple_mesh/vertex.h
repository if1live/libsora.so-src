// Ŭnicode please 
#pragma once

#include <cassert>

namespace haruna {;

const float kPi = static_cast<float>(3.14159265358979323846);
const float kPiOver2 = kPi / 2.0f;
const float kPiOver4 = kPi / 4.0f;
const float kTwoPi = kPi * 2;

class Vec3 {
public:
	Vec3() : x(0), y(0), z(0) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	float x, y, z;

	float& operator [](int i)
	{
		switch(i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			assert(!"do not reach");
			return x;
		}
	}
};

class Vec2 {
public:
	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}
	float x, y;

	float& operator [](int i)
	{
		switch(i) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			assert(!"do not reach");
			return x;
		}
	}
};

class Vertex_1P1N1UV {
public:
	Vec3 p;
	Vec3 n;
	Vec2 uv;
};

class Vertex_1P {
public:
	Vec3 p;
};

}	// namespace haruna