#include "pnFreeList.h"

using namespace pronet;
inline void pnFreeList::init(size_t _pool_index, size_t _tag_size, bool _is_used, free_list_type _next_link, free_list_type _prev_link, free_list_type _next_llist, free_list_type _prev_llist)
{
	index_ = _pool_index;
	bufSiz_ = _tag_size;
	used_stat_ = _is_used;
	nextTag_ = _next_link;
	prevTag_ = _prev_link;
	neLinkList_ = _next_llist;
	preLinkList_ = _prev_llist;
}

inline void pnFreeList::reset()
{
	index_ = 0;
	bufSiz_ = 0;
	used_stat_ = 0;
	nextTag_ = nullptr;
	prevTag_ = nullptr;
	neLinkList_ = nullptr;
	preLinkList_ = nullptr;
}

inline pnFreeList::free_list_type pnFreeList::split(free_list_type _this, size_t _this_size, ObjectPool<pnFreeList, _PN_FREE_LIST_LEVEL>& _pool)
{
	if (_this_size > bufSiz_) {
		throw std::logic_error("Tag size is too small : pnFreeList.split(size_t)");
	}
	if (used_stat_) {
		throw std::logic_error("Used tag can't split : pnFreeList.split(size_t)");
	}
	free_list_type rtag(&_pool);
	rtag->init(index_ + bufSiz_, bufSiz_ - _this_size, false, nextTag_, _this, nullptr, nullptr);
	if (nextTag_)
		nextTag_->setPrevTag(rtag);
	nextTag_ = rtag;
	detachLink();

	bufSiz_ = _this_size;
	return rtag;
}


inline void pnFreeList::marge(free_list_type _this, free_list_type _rtag)
{
	if (_rtag.get() != nextTag_.get())
		throw std::logic_error("rtag is not same NextTag : pnFreeList.marge(Pool_Object<pnFreeList<_Ty>>*, ObjectPool<pnFreeList<_Ty>, _lv>&)");
	if (_rtag->is_used())
		throw std::logic_error("rtag is already used : pnFreeList.marge(Pool_Object<pnFreeList<_Ty>>*, ObjectPool<pnFreeList<_Ty>, _lv>&)");

	bufSiz_ += _rtag->size();
	nextTag_ = _rtag->nextTag();
	nextTag_->setPrevTag(_this);
	_rtag->detachLink();
	detachLink();
	_rtag.reset();
}

inline void pnFreeList::attachTag(free_list_type _this, free_list_type _next, free_list_type _prev)
{
	if (_next) {
		_next->setPrevTag(_this);
		_this->setNextTag(_next);
	}
	if (_prev) {
		_prev->setNextTag(_this);
		_this->setPrevTag(_prev);
	}
}

inline void pnFreeList::attachLink(free_list_type _this, free_list_type _next, free_list_type _prev)
{
	if (_next) {
		_next->setPrevLink(_this);
		_this->setNextLink(_next);
	}
	if (_prev) {
		_prev->setNextLink(_this);
		_this->setPrevLink(_prev);
	}
}

inline void pnFreeList::detachTag()
{
	if (nextTag_) {
		nextTag_->setPrevTag(prevTag_);
	}
	if (prevTag_) {
		prevTag_->setNextTag(nextTag_);
	}
	nextTag_ = nullptr;
	prevTag_ = nullptr;
}

inline void pnFreeList::detachLink()
{
	if (neLinkList_) {
		neLinkList_->setPrevLink(preLinkList_);
	}
	if (preLinkList_) {
		preLinkList_->setNextLink(neLinkList_);
	}
	neLinkList_ = nullptr;
	preLinkList_ = nullptr;
}