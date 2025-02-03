#include "collib.h"

bool pnObjectCollision(const Collusion_Point& _original, const float _original_world_position[2], const Collusion_Quad& _object, const float _object_world_position[2])
{
	return false;
}

bool pnObjectCollision(const Collusion_Point& _original, const float _original_world_position[2], const Collusion_Circle& _object, const float _object_world_position[2])
{
	return false;
}

bool pnObjectCollision(const Collusion_Quad& _original, const float _original_world_position[2], const Collusion_Quad& _object, const float _object_world_position[2])
{
	if (_original_world_position[0] + _original.pos[0] + _original.size[0] > _object_world_position[0] + _object.pos[0] && 
		_original_world_position[0] + _original.pos[0] < _object_world_position[0] + _object.pos[0] + _object.size[0] &&
		_original_world_position[1] + _original.pos[1] < _object_world_position[1] + _object.pos[1] + _object.size[1] &&
		_original_world_position[1] + _original.pos[1] + _original.size[1] > _object_world_position[1] + _object.pos[1])
	{
		return true;
	}
	return false;
}

bool pnObjectCollision(const Collusion_Quad& _original, const float _original_world_position[2], const Collusion_Point& _object, const float _object_world_position[2])
{
	return false;
}

bool pnObjectCollision(const Collusion_Quad& _original, const float _original_world_position[2], const Collusion_Circle& _object, const float _object_world_position[2])
{
	return false;
}

bool pnObjectCollision(const Collusion_Circle& _original, const float _original_world_position[2], const Collusion_Point& _object, const float _object_world_position[2])
{
	return false;
}

bool pnObjectCollision(const Collusion_Circle& _original, const float _original_world_position[2], const Collusion_Circle& _object, const float _object_world_position[2])
{
	return false;
}
