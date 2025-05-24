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
		size_t index_;					//	プール上の始点
		size_t bufSiz_;					//	タグのサイズ
		bool used_stat_;				//	フリーリストの使用されているかの情報
		fl_type_shared nextTag_;		//	位置的に次のフリーリストのポインタ
		fl_type_weak prevTag_;		//	位置的に前のフリーリストのポインタ
		fl_type_weak nextLink_;		//	TLSF上で次のリンクのポインタ
		fl_type_weak prevLink_;	//	TLSF上で前のリンクのポインタ
	public:
		//	コンストラクタ
		//	pool_index : プール上の始点
		//	tag_size : タグの大きさ
		//	TLSF上のリンクしている次のポインタ
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

		//	コピーコンストラクタ
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
		
		//	タグを分割する（オブジェクトプールにて管理）
		//	this : タグのスマートポインタ
		//	pool : 新たにフリーリストのメモリを確保するプールのポインタ
		[[nodiscard]] fl_type_shared split(fl_type_shared& _this, size_t _af_size, ObjectPool<pnFreeList, _PN_FREE_LIST_LEVEL>& _pool);
		
		//	隣り合うタグを結合する
		//	rtag : 隣のタグ（右隣）
		//	pool : 隣のタグの所属するプール
		void marge(fl_type_shared& _this, fl_type_shared _rtag);

		//	タグの物理的なリンクを設定する
		//	next : 次のタグ
		//	prev : 前のタグ
		void attachTag(fl_type_shared& _this, fl_type_shared& _next, fl_type_shared& _prev);

		//	TLSF上のタグのリンクを設定する
		//	next : 次のタグ
		//	prev : 前のタグ
		void attachLink(fl_type_shared _this, fl_type_shared _next, fl_type_shared _prev);
		
		//	タグの物理的なリンクを解除する
		//	next : 次のタグ
		//	prev : 前のタグ
		void detachTag();

		//	TLSF上のタグのリンクを解除する
		//	next : 次のタグ
		//	prev : 前のタグ
		void detachLink();

		//	リストの使用状況を設定する
		//	_stat : メモリの使用状況、false の時に不使用、true の時に使用中になる
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

		//	リストが使用中かどうか返す
		[[nodiscard]] explicit operator bool() { return used_stat_; }
		[[nodiscard]] bool is_used() const { return used_stat_; }
		//	物理的に隣接している次のタグのポインタを返す
		[[nodiscard]] const fl_type_shared nextTag() const { return nextTag_; }
		//	物理的に隣接している前のタグのポインタを返す
		[[nodiscard]] const fl_type_weak prevTag() const { return prevTag_; }
		//	リンクリストの次のポインタを返す
		[[nodiscard]] const fl_type_weak nextLink() const { return nextLink_; }
		//	リンクリストの前のポインタを返す
		[[nodiscard]] const fl_type_weak prevLink() const { return prevLink_; }
		//	タグの始点のプール内のインデックスを返す
		[[nodiscard]] size_t index() const { return index_; }
		//	タグのサイズを返す
		[[nodiscard]] size_t size() const { return bufSiz_; }
	};
}