#ifndef GLNP_GEN__
#define GLNP_GEN__


#include "emp-tool/emp-tool.h"
#include "mitccraes.h"
using namespace emp;
#include <iostream>



block get_first_127(__m128i input) {
    block mask = makeBlock(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE); // 创建一个掩码，最后一位为0，其他位为1
    return _mm_and_si128(input, mask); // 使用掩码与结果进行位与操作
}


block union_127_1(block blk, bool bit)
{
  return _mm_or_si128(blk, makeBlock(0b00,bit));
  /* data */
};



int number = 0;
/*
 * The half-gate garbling scheme, with improved hashing
 * [REF] Implementation of "Two Halves Make a Whole"
 * https://eprint.iacr.org/2014/756.pdf
 */


class CircuitExecution_GLNP_gen { public:
#ifndef THREADING
	static CircuitExecution_GLNP_gen * circ_exec;
#else
	static __thread CircuitExecution_GLNP_Garble * circ_exec;
#endif

	virtual void GLNP_and_gate_gen(block& wi_pi, block& wi_pi_bar, block& wj_pj, block& wj_pj_bar, const bool& pi, const bool& pj, block& w_l_0, block& w_l_1, bool& pl) = 0;
	virtual void GLNP_xor_gate_gen(block& wi_pi, block& wi_pi_bar, block& wj_pj, block& wj_pj_bar, const bool& pi, const bool& pj, block& w_l_0, block& w_l_1, bool& pl) = 0;
	virtual void GLNP_not_gate_gen(const block& wi_pi, const block& wi_pi_bar, const bool& pi, block& w_l_0, block& w_l_1, bool& pl) = 0;

	virtual ~CircuitExecution_GLNP_gen (){ }
};


inline void GLNP_AND_garble(block wi_0, block wi_1, block wj_0, block wj_1, bool pi, bool pj, block& w_l_0, block& w_l_1, bool& pl, block *table, MITCCRAES<4> *mitccrh, PRG *prg) {
	block k[4];
	block K;
	block k_l[2], K_l[2];
	block temp[1];
	bool pi_bar = pi ^ 1, pj_bar = pj^1;
    k[pi] = wi_0;
    k[1^pi] = wi_1;
    k[2+pj] = wj_0;
    k[2+(pj^1)] = wj_1;
    // k[0] = wi_pi, k[1] = wi_pi_bar, k[2] = wi_pj, k[3] = wi_pj_bar
	block blks[8] = {makeBlock(0b0,0b00),makeBlock(0b0,0b01),makeBlock(0b00,0b10),makeBlock(0b00,0b11),makeBlock(0b00,0b00),makeBlock(0b00,0b10),makeBlock(0b00,0b01),makeBlock(0b00,0b11)};
	int index = 2 * (int) pi + (int) pj;
	// blks[0] = AES(wi_pi, 00)
	// blks[1] = AES(wi_pi, 01)
	// blks[2] = AES(wi_pi_bar, 10)
	// blks[3] = AES(wi_pi_bar, 11)
	// blks[4] = AES(wi_pj, 00)
	// blks[5] = AES(wi_pj, 10)
	// blks[6] = AES(wi_pj_bar, 01)
	// blks[7] = AES(wi_pi_bar, 11)
    mitccrh->Compute_AES<4,2>(k, blks);
	K = blks[0] ^ blks[4];
	prg->random_block(temp, 1);
	if(index == 3)
	{
		k_l[0] = get_first_127(temp[0]);
		k_l[1] = get_first_127(K);
		pl = 1 ^ getLSB(K);
		K_l[0] = union_127_1(k_l[0], pl);
		K_l[1] = K;
	}
	else
	{
		k_l[0] = get_first_127(K);
		k_l[1] = get_first_127(temp[0]);
		pl = getLSB(K);
		K_l[0] = K;
		K_l[1] = union_127_1(k_l[1], pl ^ 1);
	}
	w_l_0 = k_l[0];
	w_l_1 = k_l[1];

	table[0] = blks[1] ^ blks[6] ^ K_l[pi & pj_bar];
	table[1] = blks[2] ^ blks[5] ^ K_l[pi_bar & pj];
	table[2] = blks[3] ^ blks[7] ^ K_l[pi_bar & pj_bar];
}	



inline void GLNP_GRR2_AND_garble(block wi_0, block wi_1, block wj_0, block wj_1, bool pi, bool pj, block& w_l_0, block& w_l_1, bool& pl, block *table, bool *t, MITCCRAES<4> *mitccrh, PRG *prg) {
	block k[4];
	block K[4];
	block k_l[2], K_l[2];
	block temp[1];
    bool m[4];
    k[pi] = wi_0;
    k[1^pi] = wi_1;
    k[2+pj] = wj_0;
    k[2+(pj^1)] = wj_1;
    // k[0] = wi_pi, k[1] = wi_pi_bar, k[2] = wi_pj, k[3] = wi_pj_bar
	block blks[8] = {makeBlock(0b0,0b00),makeBlock(0b0,0b01),makeBlock(0b00,0b10),makeBlock(0b00,0b11),makeBlock(0b00,0b00),makeBlock(0b00,0b10),makeBlock(0b00,0b01),makeBlock(0b00,0b11)};
	int s = 2 * (int) (pi^1) + (int) (pj^1);
	// blks[0] = AES(wi_pi, 00)
	// blks[1] = AES(wi_pi, 01)
	// blks[2] = AES(wi_pi_bar, 10)
	// blks[3] = AES(wi_pi_bar, 11)
	// blks[4] = AES(wi_pj, 00)
	// blks[5] = AES(wi_pj, 10)
	// blks[6] = AES(wi_pj_bar, 01)
	// blks[7] = AES(wi_pi_bar, 11)
    mitccrh->Compute_AES<4,2>(k, blks);


    K[0] = blks[0] ^ blks[4];
    K[1] = blks[1] ^ blks[6];
    K[2] = blks[2] ^ blks[5];
    K[3] = blks[3] ^ blks[7];
    for(int i = 0; i < 4; i++)
    {
        m[i] = getLSB(K[i]);
        K[i] = get_first_127(K[i]);
    }

    k_l[bool(s)] = K[1]^K[2]^K[3];
	k_l[bool(s) ^ 1] = K[0];
    w_l_0 = k_l[0];
    w_l_1 = k_l[1];
    if(s == 0)
    {
        table[0] = K[2] ^ K[3];
	    table[1] = K[1] ^ K[3];
    }
    else if(s == 1)
    {
        table[0] = K[2] ^ K[3];
	    table[1] = K[0] ^ K[2];
    }
    else if(s == 2)
    {
        table[0] = K[0] ^ K[1];
	    table[1] = K[1] ^ K[3];
    }
    else
    {
        table[0] = K[0] ^ K[1];
	    table[1] = K[0] ^ K[2];
    }
	for(int i = 0; i < 4; i++)
    {
        t[i] = m[i] ^ pl;
    }
    t[s] = t[s] ^ 1;
}



inline void GLNP_XOR_garble(block wi_0, block wi_1, block wj_0, block wj_1, bool pi, bool pj, block& wl_0, block& wl_1, bool& pl, block *table, MITCCRAES<4> *mitccrh) {
	pl = pi ^ pj;
	block k[4] = {wi_0,wi_1,wj_0,wj_1};
    block blks[4];
    blks[0] = makeBlock(0,pi);
    blks[1] = makeBlock(0,pi^1);
    blks[2] = makeBlock(0,pj);
    blks[3] = makeBlock(0,pj^1);
    mitccrh->Compute_AES<4,1>(k, blks);
    table[0] = blks[0] ^ blks[1] ^ blks[2] ^ blks[3];
    if(pj == 0)
    {
        wl_0 = blks[0] ^ blks[2];
        wl_1 = blks[1] ^ blks[2];
    }
    else
    {
        wl_1 = blks[0] ^ blks[3];
        wl_0 = blks[1] ^ blks[3];
    }
}


template<typename T>
class GLNPGen:public CircuitExecution_GLNP_gen {
public:
	T * io;
	MITCCRAES<4> mitccrh;
	PRG prg;
	block k[4];
	GLNPGen(T * io) :io(io) {
	}
	void GLNP_and_gate_gen(block& wi_pi, block& wi_pi_bar, block& wj_pj, block& wj_pj_bar, const bool& pi, const bool& pj, block& w_l_0, block& w_l_1, bool& pl) override {
		block table[3];
		
		GLNP_AND_garble(wi_pi, wi_pi_bar, wj_pj, wj_pj_bar, pi, pj, w_l_0, w_l_1, pl, table, &mitccrh, &prg);
		io->send_block(table, 3);
	}

	void GLNP_xor_gate_gen(block& wi_pi, block& wi_pi_bar, block& wj_pj, block& wj_pj_bar, const bool& pi, const bool& pj, block& w_l_0, block& w_l_1, bool& pl) override {
		block table[1];
		GLNP_XOR_garble(wi_pi, wi_pi_bar, wj_pj, wj_pj_bar, pi, pj, w_l_0, w_l_1, pl, table, &mitccrh);
		io->send_block(table, 1);
	}

	void GLNP_not_gate_gen(const block& wi_pi, const block& wi_pi_bar, const bool& pi, block& w_l_0, block& w_l_1, bool& pl) override {
		w_l_0 = wi_pi;
		w_l_1 = wi_pi_bar;
		pl = pi^1;
	}
};
#endif