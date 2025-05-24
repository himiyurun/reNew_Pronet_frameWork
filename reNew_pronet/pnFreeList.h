#pragma once
#include "pronet_memory.h"

namespace pronet {
#ifndef _PN_FREE_LIST_LEVEL
#define _PN_FREE_LIST_LEVEL	(6)
#endif

	class pnFreeList
	{
		using fl_type_shared = poolObject_shared_ptr<pnFreeList, _PN_FREE_LIST_LEVEL>;
		using fl_shared_t_ptr = std::shared_ptr<Pool_Object<pnFreeList>>;
		using fl_type_weak = std::weak_ptr<Pool_Object<pnFreeList>>;
	private:
		size_t index_;					//	�v�[����̎n�_
		size_t bufSiz_;					//	�^�O�̃T�C�Y
		bool used_stat_;				//	�t���[���X�g�̎g�p����Ă��邩�̏��
		fl_type_shared nextTag_;		//	�ʒu�I�Ɏ��̃t���[���X�g�̃|�C���^
		fl_type_weak prevTag_;		//	�ʒu�I�ɑO�̃t���[���X�g�̃|�C���^
		fl_type_weak nextLink_;		//	TLSF��Ŏ��̃����N�̃|�C���^
		fl_type_weak prevLink_;	//	TLSF��őO�̃����N�̃|�C���^
	public:
		//	�R���X�g���N�^
		//	pool_index : �v�[����̎n�_
		//	tag_size : �^�O�̑傫��
		//	TLSF��̃����N���Ă��鎟�̃|�C���^
		pnFreeList(size_t _pool_index = 0, size_t _tag_size = 0, bool _is_used = false
			, fl_type_shared _next_link = nullptr, fl_type_weak _prev_link = fl_type_weak()
			, fl_type_weak _next_llist = fl_type_weak(), fl_type_weak _prev_llist = fl_type_weak())
			: index_(_pool_index)
			, bufSiz_(_tag_size)
			, used_stat_(_is_used)
			, nextTag_(_next_link)
			, prevTag_(_prev_link)
			, nextLink_(_next_llist)
			, prevLink_(_prev_llist)
		{
		}

		//	�R�s�[�R���X�g���N�^
		pnFreeList(const pnFreeList& _o) noexcept
			: index_(_o.index_)
			, bufSiz_(_o.bufSiz_)
			, used_stat_(_o.used_stat_)
			, nextTag_(_o.nextTag_)
			, prevTag_(_o.prevTag_)
			, nextLink_(_o.nextLink_)
			, prevLink_(_o.prevLink_)
		{
		}

		~pnFreeList() = default;

		void init(size_t _pool_index, size_t _tag_size, bool _is_used
			, fl_type_shared _next_link = fl_type_shared(), fl_type_weak _prev_link = fl_type_weak()
			, fl_type_weak _next_llist = fl_type_weak(), fl_type_weak _prev_llist = fl_type_weak());

		void reset();
		
		//	�^�O�𕪊�����i�I�u�W�F�N�g�v�[���ɂĊǗ��j
		//	this : �^�O�̃X�}�[�g�|�C���^
		//	pool : �V���Ƀt���[���X�g�̃��������m�ۂ���v�[���̃|�C���^
		[[nodiscard]] fl_type_shared split(fl_type_shared& _this, size_t _af_size, ObjectPool<pnFreeList, _PN_FREE_LIST_LEVEL>& _pool);
		
		//	�ׂ荇���^�O����������
		//	rtag : �ׂ̃^�O�i�E�ׁj
		//	pool : �ׂ̃^�O�̏�������v�[��
		void marge(fl_type_shared& _this, fl_type_shared _rtag);

		//	�^�O�̕����I�ȃ����N��ݒ肷��
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void attachTag(fl_type_shared& _this, fl_type_shared& _next, fl_type_shared& _prev);

		//	TLSF��̃^�O�̃����N��ݒ肷��
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void attachLink(fl_type_shared _this, fl_type_shared _next, fl_type_shared _prev);
		
		//	�^�O�̕����I�ȃ����N����������
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void detachTag();

		//	TLSF��̃^�O�̃����N����������
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void detachLink();

		//	���X�g�̎g�p�󋵂�ݒ肷��
		//	_stat : �������̎g�p�󋵁Afalse �̎��ɕs�g�p�Atrue �̎��Ɏg�p���ɂȂ�
		void setUsed(bool _status);

		void setNextTag(fl_type_shared _tag) {
			nextTag_ = _tag;
		}
		void setPrevTag(fl_type_weak _tag) {
			prevTag_ = _tag;
		}
		void setNextLink(fl_type_weak _tag) {
			nextLink_ = _tag;
		}
		void setPrevLink(fl_type_weak _tag) {
			prevLink_ = _tag;
		}
		void resize(size_t _size) {
			bufSiz_ = _size;
		}

		//	���X�g���g�p�����ǂ����Ԃ�
		[[nodiscard]] explicit operator bool() { return used_stat_; }
		[[nodiscard]] bool is_used() const { return used_stat_; }
		//	�����I�ɗאڂ��Ă��鎟�̃^�O�̃|�C���^��Ԃ�
		[[nodiscard]] const fl_type_shared nextTag() const { return nextTag_; }
		//	�����I�ɗאڂ��Ă���O�̃^�O�̃|�C���^��Ԃ�
		[[nodiscard]] const fl_type_weak prevTag() const { return prevTag_; }
		//	�����N���X�g�̎��̃|�C���^��Ԃ�
		[[nodiscard]] const fl_type_weak nextLink() const { return nextLink_; }
		//	�����N���X�g�̑O�̃|�C���^��Ԃ�
		[[nodiscard]] const fl_type_weak prevLink() const { return prevLink_; }
		//	�^�O�̎n�_�̃v�[�����̃C���f�b�N�X��Ԃ�
		[[nodiscard]] size_t index() const { return index_; }
		//	�^�O�̃T�C�Y��Ԃ�
		[[nodiscard]] size_t size() const { return bufSiz_; }
	};
}