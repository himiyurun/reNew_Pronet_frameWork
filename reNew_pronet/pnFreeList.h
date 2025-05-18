#pragma once
#include "pronet_memory.h"

namespace pronet {
#ifndef _PN_FREE_LIST_LEVEL
#define _PN_FREE_LIST_LEVEL	(6)
#endif

	class pnFreeList
	{
		using free_list_type = poolObject_shared_ptr<pnFreeList, _PN_FREE_LIST_LEVEL>;
	private:
		size_t index_;					//	�v�[����̎n�_
		size_t bufSiz_;					//	�^�O�̃T�C�Y
		bool used_stat_;				//	�t���[���X�g�̎g�p����Ă��邩�̏��
		free_list_type nextTag_;		//	�ʒu�I�Ɏ��̃t���[���X�g�̃|�C���^
		free_list_type prevTag_;		//	�ʒu�I�ɑO�̃t���[���X�g�̃|�C���^
		free_list_type neLinkList_;		//	TLSF��Ŏ��̃����N�̃|�C���^
		free_list_type preLinkList_;	//	TLSF��őO�̃����N�̃|�C���^
	public:
		//	�R���X�g���N�^
		//	pool_index : �v�[����̎n�_
		//	tag_size : �^�O�̑傫��
		//	TLSF��̃����N���Ă��鎟�̃|�C���^
		pnFreeList(size_t _pool_index = 0, size_t _tag_size = 0, bool _is_used = false
			, free_list_type _next_link = nullptr, free_list_type _prev_link = nullptr
			, free_list_type _next_llist = nullptr, free_list_type _prev_llist = nullptr)
			: index_(_pool_index)
			, bufSiz_(_tag_size)
			, used_stat_(_is_used)
			, nextTag_(_next_link)
			, prevTag_(_prev_link)
			, neLinkList_(_next_llist)
			, preLinkList_(_prev_llist)
		{
		}

		//	�R�s�[�R���X�g���N�^
		pnFreeList(pnFreeList& _o)
			: index_(_o.index_)
			, bufSiz_(_o.bufSiz_)
			, used_stat_(_o.used_stat_)
			, nextTag_(_o.nextTag_)
			, prevTag_(_o.prevTag_)
			, neLinkList_(_o.neLinkList_)
			, preLinkList_(_o.preLinkList_)
		{
		}

		~pnFreeList() = default;

		void init(size_t _pool_index, size_t _tag_size, bool _is_used
			, free_list_type _next_link, free_list_type _prev_link
			, free_list_type _next_llist, free_list_type _prev_llist);

		void reset();
		
		//	�^�O�𕪊�����i�I�u�W�F�N�g�v�[���ɂĊǗ��j
		//	this_size : ������̃T�C�Y
		//	pool : �V���Ƀt���[���X�g�̃��������m�ۂ���v�[���̃|�C���^
		free_list_type split(free_list_type _this, size_t _this_size, ObjectPool<pnFreeList, _PN_FREE_LIST_LEVEL>& _pool);
		
		//	�ׂ荇���^�O����������
		//	rtag : �ׂ̃^�O�i�E�ׁj
		//	pool : �ׂ̃^�O�̏�������v�[��
		void marge(free_list_type _this, free_list_type _rtag);

		//	�^�O�̕����I�ȃ����N��ݒ肷��
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void attachTag(free_list_type _this, free_list_type _next, free_list_type _prev);

		//	TLSF��̃^�O�̃����N��ݒ肷��
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void attachLink(free_list_type _this, free_list_type _next, free_list_type _prev);
		
		//	�^�O�̕����I�ȃ����N����������
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void detachTag();

		//	TLSF��̃^�O�̃����N����������
		//	next : ���̃^�O
		//	prev : �O�̃^�O
		void detachLink();

	private:
		void setNextTag(free_list_type _tag) {
			nextTag_ = _tag;
		}
		void setPrevTag(free_list_type _tag) {
			prevTag_ = _tag;
		}
		void setNextLink(free_list_type _tag) {
			neLinkList_ = _tag;
		}
		void setPrevLink(free_list_type _tag) {
			preLinkList_ = _tag;
		}
	public:

		//	���X�g���g�p�����ǂ����Ԃ�
		[[nodiscard]] explicit operator bool() { return used_stat_; }
		[[nodiscard]] bool is_used() const { return used_stat_; }
		//	�����I�ɗאڂ��Ă��鎟�̃^�O�̃|�C���^��Ԃ�
		[[nodiscard]] const free_list_type nextTag() const { return nextTag_; }
		//	�����I�ɗאڂ��Ă���O�̃^�O�̃|�C���^��Ԃ�
		[[nodiscard]] const free_list_type prevTag() const { return prevTag_; }
		//	�����N���X�g�̎��̃|�C���^��Ԃ�
		[[nodiscard]] const free_list_type nextLink() const { return neLinkList_; }
		//	�����N���X�g�̑O�̃|�C���^��Ԃ�
		[[nodiscard]] const free_list_type prevLink() const { return preLinkList_; }
		//	�^�O�̎n�_�̃v�[�����̃C���f�b�N�X��Ԃ�
		[[nodiscard]] size_t index() const { return index_; }
		//	�^�O�̃T�C�Y��Ԃ�
		[[nodiscard]] size_t size() const { return bufSiz_; }
	};
}