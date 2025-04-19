#pragma once			
#include "pronet_memory.h"
#include "Collision_4tree.h"

namespace pronet {
	/*
	* ���E���܂����I�u�W�F�N�g�̏���
	* �J�X�^���X�}�[�g�|�C���^�̎g�p�c4�������̃��X�g���쐬������������ʂ̑����A�X�}�[�g�|�C���^�����������Ӗ�
	* ���P��
	* �E�X�g���N�`�����X�}�[�g�|�C���^�ŊǗ������ɂ��Ȃ��Ă��邪�I�u�W�F�N�g�̈������ǂ����悤���P��̃I�u�W�F�N�g�v�[���̎���
	* 
	* �X�g���N�`���[
	* �E�I�u�W�F�N�g�E�E�X�}�[�g�|�C���^
	* �E�V�F�[�_�E�E�X�}�[�g�|�C���^
	* �E�e�N�X�`���E�E�C���f�b�N�X
	* �E�p�����E�E��
	* 
	* ����
	* �E���[�h���ɃX�}�[�g�|�C���^�𐶐�
	* �E�Q�Ƃ���B���I�u�W�F�N�g�͐����͈͂��O��邽�ߍ폜�������������̂͊m�ۂ��ꂽ�܂�
	*/

	constexpr size_t strLv = 6;

	static size_t frame_counter = 0;

	enum CHANK_DIR {
		PRONET_CHANCK_NATIVE,
		PRONET_CHANCK_UP,
		PRONET_CHANCK_DOWN,
		PRONET_CHANCK_RIGHT,
		PRONET_CHANCK_LEFT,
		PRONET_CHANCK_UP_RIGHT,
		PRONET_CHANCK_UP_LEFT,
		PRONET_CHANCK_DOWN_RIGHT,
		PRONET_CHANCK_DOWN_LEFT,
		PRONET_CHANCK_DIRECTION_SIZE
	};

	struct ChanckObjectSizeDirectionInfo {
		size_t native;		//	�`�����N�����ɑ�����I�u�W�F�N�g�̐�
		size_t up;			//	��̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t down;		//	���̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t right;		//	�E�̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t left;		//	���̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t up_right;	//	�E��̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t up_left;		//	����̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t down_right;	//	�E���̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
		size_t down_left;	//	�����̋��E�Ƃ��Ԃ�I�u�W�F�N�g�̐�
	};

	void updateFrameCounter();

	size_t getFrameCount();

	template<std::size_t VBOLV, std::size_t SHDLV>
	class Chanck_2D
	{
		pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>> structures;
		std::array<size_t, PRONET_CHANCK_DIRECTION_SIZE> obj_size;
		Collision_4tree<Structure2v<VBOLV, SHDLV>*> cells;

		using str_sp = pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>>;

	public:
		Chanck_2D();

		~Chanck_2D();

		void init(str_sp& strs, ChanckObjectSizeDirectionInfo* info);

		void reset();

		void draw() const;

		bool Intersect(const Player& _ply) const;

	private:

		void init_chanck_direction_object_size(ChanckObjectSizeDirectionInfo* info);

		void reset_chanck_direction_object_size();
	};

	template<std::size_t VBOLV, std::size_t SHDLV>
	pronet::Chanck_2D<VBOLV, SHDLV>::Chanck_2D()
		: obj_size()
	{
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	pronet::Chanck_2D<VBOLV, SHDLV>::~Chanck_2D()
	{
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::init(str_sp& strs, ChanckObjectSizeDirectionInfo* info)
	{
		structures = strs;
		init_chanck_direction_object_size(info);
		//	�Z���̏����������������[�N�����m���ň����N�����\����
		cells.init(4, -5.f, 5.f, 10.f, 10.f);
		for (auto& a : structures) {
			cells.rigist(a.get());
		}
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::reset()
	{
		structures.reset();
		reset_chanck_direction_object_size();
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::draw() const
	{
		for (auto& a : structures) {
			a->use();
			updateGameObjectUniformParam(a->parameter());
			a->draw(getFrameCount());
		}
		cells.debug_draw();
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	inline bool Chanck_2D<VBOLV, SHDLV>::Intersect(const Player& _ply) const
	{
		return cells.intersect(_ply);
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::init_chanck_direction_object_size(ChanckObjectSizeDirectionInfo* info)
	{
		obj_size[PRONET_CHANCK_NATIVE] = info->native;
		obj_size[PRONET_CHANCK_UP] = info->up;
		obj_size[PRONET_CHANCK_DOWN] = info->down;
		obj_size[PRONET_CHANCK_RIGHT] = info->right;
		obj_size[PRONET_CHANCK_LEFT] = info->left;
		obj_size[PRONET_CHANCK_UP_RIGHT] = info->up_right;
		obj_size[PRONET_CHANCK_UP_LEFT] = info->up_left;
		obj_size[PRONET_CHANCK_DOWN_RIGHT] = info->down_right;
		obj_size[PRONET_CHANCK_DOWN_LEFT] = info->down_left;
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::reset_chanck_direction_object_size()
	{
		obj_size[PRONET_CHANCK_NATIVE] = 0;
		obj_size[PRONET_CHANCK_UP] = 0;
		obj_size[PRONET_CHANCK_DOWN] = 0;
		obj_size[PRONET_CHANCK_RIGHT] = 0;
		obj_size[PRONET_CHANCK_LEFT] = 0;
		obj_size[PRONET_CHANCK_UP_RIGHT] = 0;
		obj_size[PRONET_CHANCK_UP_LEFT] = 0;
		obj_size[PRONET_CHANCK_DOWN_RIGHT] = 0;
		obj_size[PRONET_CHANCK_DOWN_LEFT] = 0;
	}
}

