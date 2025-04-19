#ifndef TLSF_MEMORY_H_
#define TLSF_MEMORY_H_
#include <vector>
#include <memory>

#include "BoundaryTag.h"

namespace pronet {

	class PNMEM_API TLSFmemory {
		uint8_t* pool;
		BoundaryTagBegin** freelist;
		size_t fl_size;

		uint16_t* parititionSLI;
		size_t SLI_size;
		uint64_t parititionFLI;

		size_t bufSize;
		uint8_t minSize;

		uint8_t divBit;
		uint8_t divSize;

	public:

		//	コンストラクタ
		//	n : プールのメモリのビット数
		//	divsize : SLIの分割数
		TLSFmemory(size_t n = 7, uint8_t divsize = 4);

		~TLSFmemory();

		//	メモリのアロケーションを行う
		//	size : アロケーションするサイズ
		void* allocate(size_t size);

		BoundaryTagBegin* searchFreeBlock(uint8_t fli, uint8_t sli) const;

		//	メモリを開放する
		//	p : 解放したいメモリブロックの先頭ポインタ
		void deallocate(void* p);

		bool margeNextfreeBlock(BoundaryTagBegin* begin, BoundaryTagBegin* next);
		bool margePrevfreeBlock(BoundaryTagBegin* prev, BoundaryTagBegin* &begin);

		//	フリーリストにタグを割り当てる
		//	begin : 割り当てたいタグのポインタ
		//	size : 割り当てたいサイズ
		void rigist(BoundaryTagBegin* begin, size_t size);

		//	フリーリストからタグを取り外す
		//	begin : 割り当てたいタグのポインタ
		//	size : 割り当てたいサイズ
		void unrigist(BoundaryTagBegin* begin, size_t size);

		void writeParititionBit(uint8_t fli, uint8_t sli);

		void clearParititionBit(uint8_t fli, uint8_t sli);

		//	パリティションビットに書き込む
		//	fli : FLIの値
		//	sli : SLIの値
		void writeBit(uint8_t fli, uint8_t sli);

		//	パリティションビットをクリアする
		//	fli : FLIの値
		//	sli : SLIの値
		void clearBit(uint8_t fli, uint8_t sli);

		//	サイズからFLIとSLIを算出
		//	fli : FLIの値を格納するポインタ
		//	sli : SLIの値を格納するポインタ
		//	size : 算出したいサイズ
		void calcTlsfIndex(uint8_t* fli, uint8_t* sli, size_t size) const;

		void calcSLI(uint8_t* sli, uint8_t fli, size_t size) const;

		size_t sizeAlignment(size_t size) const { 
#ifdef _DEBUG
			std::cout << "original : " << size << ", alignment :" << ((size + 3) & ~(3)) << std::endl;
#endif
			return (size + 3) & ~(3); 
		}

		//	ビットの描画を行う
		//	num : 描画を行いたい変数
		//	size : ビットのサイズ
		void printBit(uint64_t num, uint8_t size) const;

		//	フリーリストの中身が入っているかを描画する
		void printFreelistStatus() const;

		//	パリティションビットの中身を描画
		void printParititionBit() const;

		void printMemoryLayout() const;

		//	メモリアロケーターのレイアウト

		//	FLIとSLIをもとに1次元配列のインデックスに変換
		[[nodiscard]] size_t calcIndex(uint8_t fli, uint8_t sli) const { return (fli << divBit) + sli; }

		//	最下位ビットを返す　0のときはfalseを返す
		//	num : 最下位ビットを求めたい変数
		[[nodiscard]] bool getLSB(uint64_t num, uint8_t* lsb) const;

		//	FLIの番号から使われているかを判定
		//	fli : FLIの番号
		[[nodiscard]] bool fli_used(uint8_t fli) const;
		//	SLIの番号から使われているかを判定
		//	sli : SLIの番号
		[[nodiscard]] bool sli_used(uint8_t fli, uint8_t sli) const;
		//	メモリプールのサイズ
		[[nodiscard]] size_t size() const { return bufSize; }
	};
}
#endif