#pragma once
#include <vector>
#include <new>

#include "BoundaryTag.h"

namespace pronet {

	class TLSFmemory {
		std::vector<uint8_t> pool;
		std::vector<BoundaryTagBegin*> freelist;

		std::vector<uint16_t> parititionSLI;
		uint64_t parititionFLI;

		uint32_t bufSize;
		uint8_t minSize;

		uint8_t divBit;
		uint8_t divSize;

	public:

		//	コンストラクタ
		//	n : プールのメモリのビット数
		//	divsize : SLIの分割数
		TLSFmemory(uint32_t n, uint8_t divsize);

		~TLSFmemory();

		//	メモリのアロケーションを行う
		//	size : アロケーションするサイズ
		void* allocate(uint32_t size);

		BoundaryTagBegin* searchFreeBlock(uint8_t fli, uint8_t sli) const;

		//	メモリを開放する
		//	p : 解放したいメモリブロックの先頭ポインタ
		void deallocate(void* p);

		bool margeNextfreeBlock(BoundaryTagBegin* begin, BoundaryTagBegin* next);
		bool margePrevfreeBlock(BoundaryTagBegin* prev, BoundaryTagBegin* &begin);

		//	フリーリストにタグを割り当てる
		//	begin : 割り当てたいタグのポインタ
		//	size : 割り当てたいサイズ
		void rigist(BoundaryTagBegin* begin, uint32_t size);

		//	フリーリストからタグを取り外す
		//	begin : 割り当てたいタグのポインタ
		//	size : 割り当てたいサイズ
		void unrigist(BoundaryTagBegin* begin, uint32_t size);

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
		void calcTlsfIndex(uint8_t* fli, uint8_t* sli, uint32_t size) const;

		void calcSLI(uint8_t* sli, uint8_t fli, uint32_t size) const;

		uint32_t sizeAlignment(uint32_t size) const { return (size + 0x03) & ~(0x03); }

		//	ビットの描画を行う
		//	num : 描画を行いたい変数
		//	size : ビットのサイズ
		void printBit(uint64_t num, uint8_t size) const;

		//	フリーリストの中身が入っているかを描画する
		void printFreelistStatus() {
			for (int i = 0; i < freelist.size(); i++) {
				if (i % divSize == 0) {
					std::cout << std::endl;
					std::cout << "num" << i / divSize << " : ";
				}
				
				if (freelist[i]) {
					std::cout << "1";
				}
				else {
					std::cout << "0";
				}
			}
			std::cout << std::endl << std::endl;
		}

		//	パリティションビットの中身を描画
		void printParititionBit();

		//	FLIとSLIをもとに1次元配列のインデックスに変換
		[[nodiscard]] uint32_t calcIndex(uint8_t fli, uint8_t sli) const { return (fli << divBit) + sli; }

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
		[[nodiscard]] uint32_t size() const { return bufSize; }
	};

}