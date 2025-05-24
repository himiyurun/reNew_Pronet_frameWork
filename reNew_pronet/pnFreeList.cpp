#include "pnFreeList.h"

using namespace pronet;
void pnFreeList::init(size_t _pool_index, size_t _tag_size, bool _is_used, fl_type_shared _next_link, fl_type_weak _prev_link, fl_type_weak _next_llist, fl_type_weak _prev_llist)
{
	index_ = _pool_index;
	bufSiz_ = _tag_size;
	used_stat_ = _is_used;
	nextTag_ = _next_link;
	prevTag_ = _prev_link;
	nextLink_ = _next_llist;
	prevLink_ = _prev_llist;
}

void pnFreeList::reset()
{
	index_ = 0;
	bufSiz_ = 0;
	used_stat_ = 0;
	nextTag_.reset();
	prevTag_.reset();
	nextLink_.reset();
	prevLink_.reset();
}

pnFreeList::fl_type_shared pnFreeList::split(fl_type_shared& _this, size_t _af_size, ObjectPool<pnFreeList, _PN_FREE_LIST_LEVEL>& _pool)
{
	if (_af_size > bufSiz_) {
		throw std::logic_error("Tag size is too small : pnFreeList.split(size_t)");
	}
	if (used_stat_) {
		throw std::logic_error("Used tag can't split : pnFreeList.split(size_t)");
	}
	fl_type_shared rtag(&_pool);
	rtag->init(index_ + bufSiz_, bufSiz_ - _af_size, false, nextTag_, _this.s_ptr());
	if (nextTag_)
		nextTag_->setPrevTag(rtag.s_ptr());
	nextTag_ = rtag;
	detachLink();

	bufSiz_ = _af_size;
	return rtag;
}


void pnFreeList::marge(fl_type_shared& _this, fl_type_shared _rtag)
{
	if (_rtag.get() != nextTag_.get())
		throw std::logic_error("rtag is not same NextTag : pnFreeList.marge(Pool_Object<pnFreeList<_Ty>>*, ObjectPool<pnFreeList<_Ty>, _lv>&)");
	if (_rtag->is_used())
		throw std::logic_error("rtag is already used : pnFreeList.marge(Pool_Object<pnFreeList<_Ty>>*, ObjectPool<pnFreeList<_Ty>, _lv>&)");

	bufSiz_ += _rtag->size();
	nextTag_ = _rtag->nextTag();
	if (nextTag_)
		nextTag_->setPrevTag(_this.s_ptr());
	_rtag->detachLink();
	detachLink();
	_rtag.reset();
}

void pnFreeList::attachTag(fl_type_shared& _this, fl_type_shared& _next, fl_type_shared& _prev)
{
	if (_next) {
		_next->setPrevTag(_this.s_ptr());
		_this->setNextTag(_next);
	}
	if (_prev) {
		_prev->setNextTag(_this);
		_this->setPrevTag(_prev.s_ptr());
	}
}

void pnFreeList::attachLink(fl_type_shared _this, fl_type_shared _next, fl_type_shared _prev)
{
	if (_next) {
		_next->setPrevLink(_this.s_ptr());
		_this->setNextLink(_next.s_ptr());
	}
	if (_prev) {
		_prev->setNextLink(_this.s_ptr());
		_this->setPrevLink(_prev.s_ptr());
	}
}

void pnFreeList::detachTag()
{
	if (nextTag_) {
		nextTag_->setPrevTag(prevTag_);
	}
	fl_type_shared prev(prevTag_.lock());
	if (prev) {
		prev->setNextTag(nextTag_);
	}
	nextTag_.reset();
	prevTag_.reset();
}

void pnFreeList::detachLink()
{
	fl_type_shared next(nextLink_.lock());
	if (next) {
		next->setPrevLink(prevLink_);
	}
	fl_type_shared prev(prevLink_.lock());
	if (prev) {
		prev->setNextLink(nextLink_);
	}
	nextLink_.reset();
	prevLink_.reset();
}

void pronet::pnFreeList::setUsed(bool _status)
{
	used_stat_ = _status;
}
