#include "bullet_db.hpp"

GLBS_API void printBulletParamData(const data_base::BULLET_PARAM_DATA& _str)
{
	std::cout << "coef : " << _str.coef << std::endl;
	std::cout << "area : " << _str.area << std::endl;
	std::cout << "interval : " << _str.interval << std::endl;
	std::cout << "count : " << _str.count << std::endl;
	std::cout << "rad : " << _str.rad << std::endl;
	std::cout << "speed : " << _str.speed << std::endl;
}

void test(float _pos[2])
{
	std::cout << "pos : " << _pos[0] << ", " << _pos[1] << std::endl;
}
