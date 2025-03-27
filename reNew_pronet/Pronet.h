#pragma once
#include "loader.h"
#include "Player.h"
#include "PyBullet.h"
#include "glfw_Window.h"

static const size_t strLv = 6;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト
template<std::size_t VBOLV, std::size_t SHDLV>
class PronetManager : public glfw_Window {
	GLint dimentionSize;
	pronet::loader<VBOLV, SHDLV> loader_object;
	pronet::Player player;
	std::array<pronet::Chanck_2D<VBOLV, SHDLV>, CHANCK_LOAD_SIZE> chanck;

	pronet::ObjectPool<Object, 6> bulletobj;
	pronet::ObjectPool<Shader, 6> bulletshd;
	pronet::ObjectPool<Texture, 6> bullettex;
	PyBullet sample;
public:
	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo *windowInfo, const char* loadfilelist_name);

	//	デストラクタ
	~PronetManager();

	void load();

	//	ループ内で実行する処理
	virtual void process();
};

template<std::size_t VBOLV, std::size_t SHDLV>
PronetManager<VBOLV, SHDLV>::PronetManager(glfw_windowCreateInfo* windowInfo, const char* loadfilelist_name)
	: glfw_Window(windowInfo)
	, loader_object(&dimentionSize, loadfilelist_name)
	, bulletobj(2), bulletshd(2), bullettex(2)
{
	pronet::initUniformBlock();
}

template<std::size_t VBOLV, std::size_t SHDLV>
PronetManager<VBOLV, SHDLV>::~PronetManager()
{
	chanck[0].reset();
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::load()
{
	pronet::Player2vCreateInfo info;
	loader_object.load_playerinfo("player.fi", &info);
	player.init(dimentionSize, &info);
	loader_object.load_chanck(0, 0, chanck[0]);

	PyBulletCreateInfo bulletInfo;
	bulletInfo.info_.id_ = "sample";
	bulletInfo.info_.coef_ = 12;
	bulletInfo.info_.interval_ = 5;
	bulletInfo.info_.rad_ = 0.f;
	bulletInfo.info_.speed_ = 0.07f;
	bulletInfo.exec_func_id_ = "execute";
	bulletInfo.gen_func_id_ = "generate";
	bulletInfo.update_func_id_ = "update";
	pronet::poolObject_shared_ptr<Object, 6> obj(&bulletobj);
	pronet::poolObject_shared_ptr<Shader, 6> shd(&bulletshd);
	pronet::poolObject_shared_ptr<Texture, 6> tex(&bullettex);

	obj->init(2, 4, bulletAngleVertex, 6, bulletAngleIndex, bulletAngleUv);
	shd->init("bullet.vert", "bullet.frag");
	tex->init("pic/enemypre.bmp");
	sample.Initialize("main.py", bulletInfo, obj, shd, tex, "pysrc");
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::process()
{
	static GLfloat velocity(0.05f);
	static GLfloat last_pos[2];
	static glm::vec2 bullet_pos(0.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	last_pos[0] = player.param.position[0];
	last_pos[1] = player.param.position[1];
	if (glfwGetKey(window, GLFW_KEY_W)) {
		player.param.position[1] += velocity;
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) {
		player.param.position[1] -= velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		player.param.position[0] -= velocity;
	}
	else if (glfwGetKey(window, GLFW_KEY_D)) {
		player.param.position[0] += velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_F3)) {
		if (swapStat())
			setSwapEventsStatus(false);
		else
			setSwapEventsStatus(true);
	}

	if (chanck[0].Intersect(player)) {
		player.param.position[0] = last_pos[0];
		player.param.position[1] = last_pos[1];
	}

	pronet::updateApplicationUniformParam(&param);
	pronet::updatePlayer2vUniformParam(player.parameter());
	pronet::updateFrameCounter();
	bullet_pos.x = player.position()[0];
	bullet_pos.y = player.position()[1];
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		sample.run(bullet_pos, true);
	}
	else {
		//sample.run(bullet_pos, false);
	}
	chanck[0].draw();
	player.draw();
}