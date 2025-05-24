#include "tlsf.h"

using namespace pronet;	

tlsf_set::tlsf_set(uint8_t _div_size, size_t _pool_size)
	: fli_(0)
	, bufSiz_(sizeAlignment(_pool_size))
	, bitdiv_(_div_size), minsiz_(1ULL << _div_size)
	, bitmsk_((1ULL << _div_size) - 1)
{
	if (_pool_size < _div_size) {
		throw std::runtime_error("PoolSize is too small : tlsf_set.tlsf_set");
	}
	resize_bmp(_pool_size);
	fl_store_type_shared fl(&flPool_);
	//	ブロックサイズ、前後のリンクリスト、オブジェクトプールのオブジェクト情報（返却時に必要なため埋め込む）
	fl->init(0, bufSiz_, false);
	rigist(fl);
	first_list = fl.s_ptr();
	final_list = fl;
}

tlsf_set::~tlsf_set()
{
}

bool pronet::tlsf_set::get(size_t _size, obj_type& _obj)
{
	size_t index(0);
	uint8_t fli(0), sli(0);
	size_t size = Alignment(_size);
	fl_store_type_shared tag = locate(size, &fli, &sli, &index);
	if (!tag) {
		resize(bufSiz_ + size * 2);
		tag = locate(size, &fli, &sli, &index);
		if (!tag)
			return false;
	}
	std::cout << "tag size : " << tag->size() << std::endl;
	print_bmp();
	unrigist(tag, fli, sli, index);
	if (tag->size() - size > minsiz_) {
		fl_store_type_shared rtag = tag->split(tag, size, flPool_);
		rigist(rtag);
		if (!rtag->nextTag())
			final_list = rtag;
	}
	tag->setUsed(true);
	_obj = tag;
	return true;
}

void pronet::tlsf_set::back(obj_type& _obj)
{
	size_t index(0);
	uint8_t fli(0), sli(0);
	fl_store_type_shared obj = _obj;
	if (!obj) {
		throw std::runtime_error("Pool Object is null!! : tlsf_set.back(obj_type)");
	}
	obj->setUsed(false);
	calcTlsfIndex(fli, sli, obj->size());
	index = calcIndex(fli, sli);
	fl_store_type_shared nextt = obj->nextTag();
	if (nextt) {
		if (!nextt->is_used()) {
			unrigist(nextt);
			obj->marge(obj, nextt);
		}
	}
	fl_store_type_shared prevt = obj->prevTag().lock();
	if(prevt){
		if (!prevt->is_used()) {
			unrigist(prevt);
			prevt->marge(prevt, obj);
			obj = prevt;
		}
	}
	rigist(obj);
	resize(compress());
}

void tlsf_set::resize(size_t _size)
{
	size_t new_size = sizeAlignment(_size);
	resize_bmp(new_size);
	fl_store_type_shared fl;
	std::cout << "tlsf resize : " << new_size << ", presize : " << bufSiz_ << std::endl;

	if (new_size > bufSiz_) {
		fl.realloc(&flPool_);
		if (!fl) {
			throw std::bad_alloc();
		}

		if (final_list->is_used()) {
			fl->init(bufSiz_, new_size - bufSiz_, false, nullptr, final_list.s_ptr());
			final_list->setNextTag(fl);
			rigist(fl);
			printFlMap();
			final_list = fl;
			std::cout << "last used!!!!!!!!!!!!!!!!!!!" << std::endl;
		}
		else {
			final_list->detachLink();
			unrigist(final_list);
			final_list->resize(final_list->size() + new_size - bufSiz_);
			rigist(final_list);
			std::cout << "last not used!!!!!!!!!!!!!!!!!!!" << std::endl;
		}
	}
	else if (new_size < bufSiz_) {
		size_t index(0);
		size_t total(0);
		fl = final_list;
		index = fl->index();
		total = fl->size();
		std::cout << "total : " << total << ", bufSize : " << bufSiz_ << ", new_size : " << new_size; std::cout << ", fl index : " << fl->index() << std::endl;
		while (bufSiz_ - (total) > new_size) {
			fl = fl->prevTag().lock();
			index = fl->index();
			total += fl->size();
			if (!fl->prevTag().lock())
				throw std::logic_error("out of range!! : tlsf_set.resize(size_t)");
		}
		size_t split_siz(total - (bufSiz_ - new_size));
		if (split_siz > 0) {
			std::cout << "total : " << total << ", bufSize : " << bufSiz_ << ", new_size : " << new_size << ", split_siz : " << split_siz; std::cout << ", fl index : " << fl->index() << std::endl;
			unrigist(fl);
			fl->split(fl, split_siz, flPool_);
			rigist(fl);
		}
		else {
			fl = fl->prevTag().lock();
		}

		fl_store_type_shared nowfl = fl;
		fl_store_type_shared lastfl = fl;
		fl = fl->nextTag();
		while (fl) {
			std::cout << "tag size : " << fl->size() << std::endl;
			unrigist(fl);
			lastfl = fl;
			fl = fl->nextTag();
			lastfl->detachTag();
		}
		final_list = nowfl;
		printFlMap();
		nowfl->setNextTag(nullptr);
		std::cout << "final size : " << final_list->size() << std::endl;
		std::cout << "compressed bmp" << std::endl;
		print_bmp();
		resize_bmp(new_size);
	}
	else {
		std::cerr << "Log : Pool resize same size : tlsf_set.resize(size_t)" << std::endl;
	}
	bufSiz_ = new_size;
}

size_t tlsf_set::compress()
{
	fl_store_type_shared fl = final_list;
	while (!fl->is_used()) {
		fl = fl->prevTag().lock();
		if (!fl) {
			return bufSiz_;
		}
	}
	return fl->index() + fl->size();
}

void tlsf_set::calcTlsfIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const
{
	unsigned long index;
	_BitScanReverse(&index, _size);
	_fli = (uint8_t)index;
	calcSLI(_sli, _fli, _size);
}

void tlsf_set::print_bmp() const
{
	std::cout << "fli_level : " << (unsigned)fliLv_ << std::endl;
	std::cout << "FLI : ";
	_bit_print(fli_);
	for (uint8_t i = 0; i < fliLv_; i++) {
		for (size_t j = 0; j < minsiz_; j++) {
			if (sli_[i * minsiz_ + j])
				std::cout << "1";
			else
				std::cout << "0";
		}
		std::cout << '\n';
	}
}

void pronet::tlsf_set::printFlMap() const
{
	size_t tag_ct(0);
	fl_store_type_shared buf(first_list.lock());
	while (buf) {
		std::cout << "tag " << tag_ct << " : " << buf->index() << " : " << buf->size() << " : " << buf->is_used() << std::endl;
		tag_ct++;
		buf = buf->nextTag();
	}
}

size_t pronet::tlsf_set::Alignment(size_t _n) const
{
	size_t size = sizeAlignment(_n);
	if (size < minsiz_)
		return minsiz_;
	else
		return size;
}

void pronet::tlsf_set::rigist(fl_store_type_shared& _obj)
{
	uint8_t fli(0), sli(0);
	size_t size(_obj->size());
	if (size >= minsiz_) {
		calcTlsfIndex(fli, sli, size);
	}
	else {
		fli = bitdiv_;
	}

	if (!_bit_get_status(fli, UNSIGNED_INT_64, fli))
		_bit_write_one_area(&fli_, UNSIGNED_INT_64, fli, 1);

	size_t index(calcIndex(fli, sli));
	if (!sli_[index]) {
		sli_.write_Bit_1(index, 1);
		std::cout << index << " write" << std::endl;
	}
	else {
		std::cout << "already" << std::endl;
	}

	//	フリーリストを割り当てる、前後のリンクを引数に取る
	_obj->attachLink(_obj, data_[index].lock(), nullptr);
	if (!_obj->nextTag())
		final_list = _obj;
	if (!_obj->prevTag().lock())
		first_list = _obj.s_ptr();

	data_[index] = _obj.s_ptr();
}

void tlsf_set::unrigist(fl_store_type_shared& _obj)
{
	size_t index(0);
	uint8_t fli(0), sli(0);
	calcTlsfIndex(fli, sli, _obj->size());
	index = calcIndex(fli, sli);
	delink(_obj, index);
	del_bmp(fli, sli, index);
}

void pronet::tlsf_set::unrigist(fl_store_type_shared& _obj, uint8_t _fli, uint8_t _sli, size_t _index)
{
	delink(_obj, _index);
	del_bmp(_fli, _sli, _index);
}

tlsf_set::fl_store_type_shared tlsf_set::locate(size_t _size, uint8_t* const _fli, uint8_t* const _sli, size_t* const _index)
{
	uint8_t fli(0), sli(0);
	if (!search(fli, sli, _size)) {
		std::cerr << "Log : Can't Found free block!!" << std::endl;
		return nullptr;
	}
	std::cout << "locate fli : " << (unsigned)fli << ", locate sli : " << (unsigned)sli << std::endl;
	size_t index(calcIndex(fli, sli));
	fl_store_type_shared obj = data_[index].lock();
	std::cout << "size : " << final_list->size() << std::endl;

	if (_fli)
		*_fli = fli;
	if (_sli)
		*_sli = sli;
	if (_index)
		*_index = index;
	return obj;
}

bool tlsf_set::search(uint8_t& _fli, uint8_t& _sli, size_t _size) const
{
	uint8_t f(0), s(0);
	calcTlsfIndex(f, s, _size);
	align_idx(f, s);
	size_t fli(f), sli(s);
	if (_bit_get_status(fli_, UNSIGNED_INT_64, fli)) {
		std::cout << "1fli : " << fli << ", 1sli : " << sli << std::endl;
		if (sli_.find_one_from_unit(calcIndex(fli, sli), &sli)) {
			_fli = sli >> bitdiv_;
			_sli = sli & bitmsk_;
			return true;
		}
	}
	sli = 0;

	if (!_bit_find_one_from(fli_, UNSIGNED_INT_64, ++fli, &fli)) {
		std::cout << "fli : " << fli << ", sli : " << sli << std::endl;
		std::cerr << "Log : Memory Block is Full!!" << std::endl;
		return false;
	}
	if (!sli_.find_one_from_unit(calcIndex(fli, sli), &sli)) {
		print_bmp();
		std::cout << "fli : " << fli << ", sli : " << sli << ", index : " << calcIndex(fli, sli) << std::endl;
		throw std::logic_error("Not working second level index in TLSFrigist system!! : tlsf_set.search(uint8_t, uint8_t, size_t)");
	}
	std::cout << "rfli : " << fli << ", rsli : " << (sli & bitmsk_) << std::endl;
	_fli = fli;
	_sli = sli & bitmsk_;
	return true;
}

bool tlsf_set::getStat(uint8_t _fli, uint8_t _sli) const
{
	size_t index((_fli << bitdiv_) + _sli);
	return sli_[index];
}

void tlsf_set::resize_bmp(size_t _size)
{
	size_t lsb = 0;
	if (!pronet::_bit_find_one_from_reverse(_size, sizeof(size_t) * 0x08, 0, &lsb)) {
		throw std::runtime_error("pool_size is too small : tlsf_set");
	}
	fliLv_ = ++lsb;
	data_.resize((lsb) << bitdiv_);
	std::cout << "lsb : " << lsb << ", bit_div : " << (unsigned)bitdiv_ << std::endl;
	sli_.resize(((((1UL << bitdiv_) * (lsb)) + 0x3f) >> BITCOUNT_OF_64));
}

void tlsf_set::align_idx(uint8_t& _fli, uint8_t& _sli) const
{
	_sli++;
	if (_sli > bitmsk_) {
		_sli &= bitmsk_;
		_fli++;
	}
}

void tlsf_set::del_bmp(uint8_t _fli, uint8_t _sli, size_t _index)
{
	std::cout << "fli : " << (unsigned)_fli << ", sli : " << (unsigned)_sli << ", div : " << minsiz_ << std::endl;
	if (!data_[_index].lock()) {
		sli_.write_Bit_0(((uint64_t)_fli << bitdiv_) + _sli, 1);
		if (!sli_.extract_area((uint64_t)_fli << bitdiv_, minsiz_)) {
			_bit_write_zero_area(&fli_, UNSIGNED_INT_64, _fli, 1);
		}
	}
}

void tlsf_set::delink(fl_store_type_shared _obj, size_t _index)
{
	data_[_index] = _obj->nextLink();
	//	自分の現在の位置を引数に取る
	_obj->detachLink();
}

void tlsf_set::calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const
{
	uint8_t shift(0);
	shift = _fli - bitdiv_;
	_sli = (_size & ~(1 << _fli)) >> (shift);
}

size_t tlsf_set::sizeAlignment(size_t _n) const
{
	return (_n + 3) & ~(3);
}