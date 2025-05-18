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
		size_t index_;					//	プール上の始点
		size_t bufSiz_;					//	タグのサイズ
		bool used_stat_;				//	フリーリストの使用されているかの情報
		free_list_type nextTag_;		//	位置的に次のフリーリストのポインタ
		free_list_type prevTag_;		//	位置的に前のフリーリストのポインタ
		free_list_type neLinkList_;		//	TLSF上で次のリンクのポインタ
		free_list_type preLinkList_;	//	TLSF上で前のリンクのポインタ
	public:
		//	コンストラクタ
		//	pool_index : プール上の始点
		//	tag_size : タグの大きさ
		//	TLSF上のリンクしている次のポインタ
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

		//	コピーコンストラクタ
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
		
		//	タグを分割する（オブジェクトプールにて管理）
		//	this_size : 分割後のサイズ
		//	pool : 新たにフリーリストのメモリを確保するプールのポインタ
		free_list_type split(free_list_type _this, size_t _this_size, ObjectPool<pnFreeList, _PN_FREE_LIST_LEVEL>& _pool);
		
		//	隣り合うタグを結合する
		//	rtag : 隣のタグ（右隣）
		//	pool : 隣のタグの所属するプール
		void marge(free_list_type _this, free_list_type _rtag);

		//	タグの物理的なリンクを設定する
		//	next : 次のタグ
		//	prev : 前のタグ
		void attachTag(free_list_type _this, free_list_type _next, free_list_type _prev);

		//	TLSF上のタグのリンクを設定する
		//	next : 次のタグ
		//	prev : 前のタグ
		void attachLink(free_list_type _this, free_list_type _next, free_list_type _prev);
		
		//	タグの物理的なリンクを解除する
		//	next : 次のタグ
		//	prev : 前のタグ
		void detachTag();

		//	TLSF上のタグのリンクを解除する
		//	next : 次のタグ
		//	prev : 前のタグ
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

		//	リストが使用中かどうか返す
		[[nodiscard]] explicit operator bool() { return used_stat_; }
		[[nodiscard]] bool is_used() const { return used_stat_; }
		//	物理的に隣接している次のタグのポインタを返す
		[[nodiscard]] const free_list_type nextTag() const { return nextTag_; }
		//	物理的に隣接している前のタグのポインタを返す
		[[nodiscard]] const free_list_type prevTag() const { return prevTag_; }
		//	リンクリストの次のポインタを返す
		[[nodiscard]] const free_list_type nextLink() const { return neLinkList_; }
		//	リンクリストの前のポインタを返す
		[[nodiscard]] const free_list_type prevLink() const { return preLinkList_; }
		//	タグの始点のプール内のインデックスを返す
		[[nodiscard]] size_t index() const { return index_; }
		//	タグのサイズを返す
		[[nodiscard]] size_t size() const { return bufSiz_; }
	};
}