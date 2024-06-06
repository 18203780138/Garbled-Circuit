#ifndef MITCCRAES__
#define MITCCRAES__
#include "emp-tool/emp-tool.h"
using namespace emp;
/*
 * [REF] Implementation of "Better Concrete Security for Half-Gates Garbling (in the Multi-Instance Setting)"
 * https://eprint.iacr.org/2019/1168.pdf
 */


template<int AESBatchSize = 8>
class MITCCRAES { public:
	AES_KEY scheduled_key[AESBatchSize];
	block keys[AESBatchSize];

	template<int K>
	void renew_AES_ks(block* k) {
		for(int i = 0; i < K; ++i)
			keys[i] = k[i];
		AES_opt_key_schedule<K>(keys, scheduled_key);
	}

	template<int K, int H>
	void Compute_AES(block * k, block * blks) {
		renew_AES_ks<K>(k);
		ParaEnc<K,H>(blks, scheduled_key);
	}
};



#endif