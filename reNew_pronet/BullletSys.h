#pragma once
#include <glbs/py_db.hpp>
#include "BulletManager.h"
class BulletSys : public BulletManager 
{
	size_t bullet_;
	size_t pyObj_;

	const char* exec_func_;
	const char* gen_func_;
	const char* upd_func_;
public:
	BulletSys(const char* _py_name = nullptr);
	~BulletSys();

	void init_bs(const char* _py_name);

	void reset_bs();
};

