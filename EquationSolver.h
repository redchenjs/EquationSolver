/*
 * EquationSolver.h
 *
 *  Created on: 2022-07-30 18:20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef __EQUATIONSOLVER__
#define __EQUATIONSOLVER__

#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

class EquationSolver
{
private:
    bool debug = false;
    long double C[7][7] = { 0.0 };

    void zero_mat(int n, long double C[7][7]);

    bool pivot_mat(int k, int n, int64_t C[7][7]);
    bool pivot_mat(int k, int n, long double C[7][7]);

    void print_mat(const char *str, int n, long double C[7][7]);
    void print_mat(char idx, int k, int n, int64_t C[7][7]);
    void print_mat(char idx, int k, int n, long double C[7][7]);
    void print_mat(char idx, int k, int m, int n, int64_t C[7][7]);
    void print_mat(char idx, int k, int m, int n, long double C[7][7]);

    void print_res(int n, long double C[7][7]);
    void print_res(int n, long double C[7][7], int scale);

public:
    void set_debug(bool val);

    void load_data(const int64_t i64EqualCoeff[7][7], int iParaNum, int scale);
    void save_data(double dAffinePara[6], int iParaNum, int scale);

    void method_gja(int n);
    void method_gja2(int n, int q, int scale);
    void method_dfa(int n, int scale);
#ifdef __GNUC__
    void method_dfa128(int n, int scale);
#endif
    void method_dfa2(int n, int scale);
    void method_dfa2s(int n, int scale);
    void method_dfa3(int n, int scale);
    void method_dfa3s(int n, int scale);
    void method_dfa4a(int n, int scale);
    void method_dfa4b(int n, int scale);
    void method_cra(int n, int scale);
};

#endif // __EQUATIONSOLVER__
