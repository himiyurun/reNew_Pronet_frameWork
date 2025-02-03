#pragma once
#include "loader.h"
#include "Player.h"
#include "glfw_Window.h"

static const size_t strLv = 6;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト
template<std::size_t VBOLV, std::size_t SHDLV>
class PronetManager : public glfw_Window {
	GLint dimentionSize;
	pronet::loader<VBOLV, SHDLV> loader_object;
	pronet::Player player;
	std::array<pronet::Chanck_2D<VBOLV, SHDLV>, CHANCK_LOAD_SIZE> chanck;

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
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::process()
{
	static GLfloat velocity(0.05f);
	static GLfloat last_pos[2];
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

	if (chanck[0].Intersect(player.getColInfoQuad(), player.position())) {
		/*
		player.param.position[0] = last_pos[0];
		player.param.position[1] = last_pos[1];
		*/
		player.param.intersect = true;
	}
	else {
		player.param.intersect = false;
	}

	pronet::updateApplicationUniformParam(&param);
	pronet::updatePlayer2vUniformParam(player.parameter());
	pronet::updateFrameCounter();
	chanck[0].draw();
	player.draw();
}