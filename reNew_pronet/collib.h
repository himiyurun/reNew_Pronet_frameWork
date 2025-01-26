#pragma once
#include <iostream>

struct Collusion_Point {
	float pos[2];
	Collusion_Point(float _xpos, float _ypos)
		: pos{ _xpos,_ypos }
	{}
};

struct Collusion_Quad {
	float pos[2];
	float size[2];
	Collusion_Quad(float _xpos, float _ypos, float _xsiz, float _ysiz)
		: pos{ _xpos, _ypos }
		, size{ _xsiz, _ysiz }
	{}
};

struct Collusion_Circle {
	float pos[2];
	float rad;
	Collusion_Circle(float _xpos, float _ypos, float _rad)
		: pos{ _xpos, _ypos }
		, rad(_rad)
	{}
};

bool pnObjectCollusion(const Collusion_Point& _original, const float _original_world_position[2], const Collusion_Quad& _object, const float _object_world_position[2]);
bool pnObjectCollusion(const Collusion_Point& _original, const float _original_world_position[2], const Collusion_Circle& _object, const float _object_world_position[2]);
bool pnObjectCollusion(const Collusion_Quad& _original, const float _original_world_position[2], const Collusion_Quad& _object, const float _object_world_position[2]);
bool pnObjectCollusion(const Collusion_Quad& _original, const float _original_world_position[2], const Collusion_Point& _object, const float _object_world_position[2]);
bool pnObjectCollusion(const Collusion_Quad& _original, const float _original_world_position[2], const Collusion_Circle& _object, const float _object_world_position[2]);
bool pnObjectCollusion(const Collusion_Circle& _original, const float _original_world_position[2], const Collusion_Point& _object, const float _object_world_position[2]);
bool pnObjectCollusion(const Collusion_Circle& _original, const float _original_world_position[2], const Collusion_Circle& _object, const float _object_world_position[2]);