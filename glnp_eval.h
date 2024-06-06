#ifndef GLNP_EVAL__
#define GLNP_EVAL__

#include "emp-tool/emp-tool.h"
using namespace emp;
#include <iostream>




class CircuitExecution_GLNP_eva { public:
#ifndef THREADING
	static CircuitExecution_GLNP_eva * circ_exec;
#else
	static __thread CircuitExecution_GLNP_Eval * circ_exec;
#endif

	virtual void GLNP_and_gate_eva(block& w_i, block& w_j, const bool lambda_i, const bool lambda_j, block &w_l, bool &lambda_l) = 0;
	virtual void GLNP_xor_gate_eva(block& w_i, block& w_j, const bool lambda_i, const bool lambda_j, block &w_l, bool &lambda_l) = 0;
	virtual void GLNP_not_gate_eva(const block&w_i, const bool &lambda_i, block &w_l, bool &lambda_l) = 0;

	virtual ~CircuitExecution_GLNP_eva (){}
};




inline void GLNP_AND_Eval(block w_i, block w_j, bool lambda_i, bool lambda_j, block &w_l, bool &lambda_l, const block *table, MITCCRAES<4> *mitccrh) {
	block k[2] = {w_i, w_j};
	block temp;
	int index = 2 * lambda_i + lambda_j;
	block blks[2] = {makeBlock(0,index), makeBlock(0,index)};
	mitccrh->Compute_AES<2,1>(k, blks);
	if(index == 0)
	{
		temp = blks[0] ^ blks[1];
	}
	else
	{
		temp = blks[0] ^ blks[1] ^ table[index-1];
	}
	w_l = get_first_127(temp);
	lambda_l = getLSB(temp);
}


inline void GLNP_GRR2_AND_Eval(block w_i, block w_j, bool lambda_i, bool lambda_j, block &w_l, bool &lambda_l, const block *table, const bool *t, MITCCRAES<4> *mitccrh) {
    block K;
    block k[2] = {w_i, w_j};
    int index = 2 * int(lambda_i) + int(lambda_j);
    block blks[2] = {makeBlock(0,index), makeBlock(0,index)};
    mitccrh->Compute_AES<2,1>(k, blks);
    K = get_first_127(blks[0] ^ blks[1]);
    bool m = getLSB(blks[0] ^ blks[1]);
    if(index == 0)
        w_l = K;
    else if(index == 1)
        w_l = K ^ table[0];
    else if(index == 2)
        w_l = K ^ table[1];
    else
        w_l = K ^ table[0] ^ table[1];
    lambda_l = m ^ t[index];
}


inline void GLNP_XOR_Eval(block wi, block wj, bool lambda_i, bool lambda_j, block &wl, bool &lambda_l, const block *table, MITCCRAES<4> *mitccrh)
{
	block k[2] = {wi, wj};
    block blks[2] = {makeBlock(0,lambda_i), makeBlock(0,lambda_j)};
	mitccrh->Compute_AES<2,1>(k,blks);

	if(lambda_j == 0)
		wl = blks[0] ^ blks[1];
	else
		wl = blks[0] ^ blks[1] ^ table[0];
}



template<typename T>
class GLNPEva:public CircuitExecution_GLNP_eva {
public:
	T * io;
	MITCCRAES<4> mitccrh;
	block k[2];
	GLNPEva(T * io) :io(io) {
	}
	// GLNP_AND_Eval(block w_i, block w_j, bool lambda_i, bool lambda_j, block &w_l, bool &lambda_l, const block *table, MITCCRH<4> *mitccrh)
	void GLNP_and_gate_eva(block& w_i, block& w_j, const bool lambda_i, const bool lambda_j, block& w_l, bool& lambda_l) override {
		block table[3];
		io->recv_block(table, 3);
		GLNP_AND_Eval(w_i,w_j,lambda_i,lambda_j,w_l,lambda_l,table, &mitccrh);
	}

	void GLNP_xor_gate_eva(block& w_i, block& w_j, bool lambda_i, bool lambda_j, block& w_l, bool& lambda_l) override {
		block table[1];
		io->recv_block(table, 1);
		GLNP_XOR_Eval(w_i, w_j, lambda_i, lambda_j, w_l, lambda_l, table, &mitccrh);
	}


	void GLNP_not_gate_eva(const block&w_i, const bool &lambda_i, block &w_l, bool &lambda_l) override {
		w_l = w_i;
		lambda_l = lambda_i;
	}
};






#endif
