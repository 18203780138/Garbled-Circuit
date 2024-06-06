#include "emp-tool/emp-tool.h"
#include "glnp_gen.h"
#include "glnp_eval.h"
#include <iostream>
using namespace std;
using namespace emp;

int main(void) {
	// sender
	block data[4], table[3], wl_0, wl_1;
	MITCCRAES<4> mi_gen;
	MITCCRAES<8> mi_gen_half;
	PRG prg;
	bool t[4];
	// receiver
	block data1[2];
	MITCCRAES<4> mi_eva;
	MITCCRAES<8> mi_eva_half;
	block w_l;

	bool pl, lambda_l;
	cout << "Correctness ... ";

	block ret1;

	for(int ii = 0; ii < 2; ++ii) {
		for(int jj = 0; jj < 2; ++jj) {
			for(int pi = 0; pi < 2; ++pi) {
				for(int pj = 0; pj < 2; ++pj){
					prg.random_block(data, 4);
					if(ii == 1) data1[0] = data[1]; else data1[0] = data[0];
					if(jj == 1) data1[1] = data[3]; else data1[1] = data[2];
					GLNP_AND_garble(data[0], data[1], data[2], data[3],pi, pj,wl_0, wl_1, pl, table, &mi_gen, &prg);
					GLNP_AND_Eval(data1[0], data1[1], ii^pi, jj^pj, w_l, lambda_l, table, &mi_eva);
					if(ii == 1 && jj == 1)
						ret1 = wl_1;
					else
						ret1 = wl_0;
					// cout << "wl_0" << wl_0 << endl;
					// cout << "wl_1" << wl_1 << endl;
					// cout << "wl" << w_l << endl;
					// cout << "ret1" << ret1 << endl;
					if(cmpBlock(&w_l, &ret1, 1) == false) {cout << "wrong" << endl; abort();}
				}
			}
		}
	}
	cout << "GLNP AND gate check\n";




	for(int ii = 0; ii < 2; ++ii) {
		for(int jj = 0; jj < 2; ++jj) {
			for(int pi = 0; pi < 2; ++pi) {
				for(int pj = 0; pj < 2; ++pj){
					prg.random_block(data, 4);
					if(ii == 1) data1[0] = data[1]; else data1[0] = data[0];
					if(jj == 1) data1[1] = data[3]; else data1[1] = data[2];
					GLNP_GRR2_AND_garble(data[0], data[1], data[2], data[3],pi, pj,wl_0, wl_1, pl, table, t, &mi_gen, &prg);
					GLNP_GRR2_AND_Eval(data1[0], data1[1], ii^pi, jj^pj, w_l, lambda_l, table, t, &mi_eva);
					if(ii == 1 && jj == 1)
						ret1 = wl_1;
					else
						ret1 = wl_0;
					// cout << "wl_0: " << wl_0 << endl;
					// cout << "wl_1: " << wl_1 << endl;
					// cout << "wl: " << w_l << endl;
					// cout << "ret1: " << ret1 << endl;
					if(cmpBlock(&w_l, &ret1, 1) == false) {cout << "wrong" << endl; abort();}
				}
			}
		}
	}
	cout << "GLNP AND gate check\n";





	for(int ii = 0; ii < 2; ++ii) {
		for(int jj = 0; jj < 2; ++jj) {
			for(int pi = 0; pi < 2; ++pi) {
				for(int pj = 0; pj < 2; ++pj){
					prg.random_block(data, 4);
					if(ii == 1) data1[0] = data[1]; else data1[0] = data[0];
					if(jj == 1) data1[1] = data[3]; else data1[1] = data[2];
					GLNP_XOR_garble(data[0], data[1], data[2], data[3], pi, pj, wl_0, wl_1, pl, table, &mi_gen);
					GLNP_XOR_Eval(data1[0], data1[1], pi^ii, pj^jj, w_l, lambda_l, table, &mi_eva);
					if((ii ^ jj) == 1)
					{
						ret1 = wl_1;
					}
					else
					{
						ret1 = wl_0;
					}
					if(cmpBlock(&w_l, &ret1, 1) == false) {cout << "wrong" << endl; abort();}
				}
			}
		}
	}
	cout << "GLNP XOR gate check\n";

	return 0;
}
