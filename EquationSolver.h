/*
 * EquationSolver.h
 *
 *  Created on: 2022-07-30 18:20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef __EQUATION_SOLVER__
#define __EQUATION_SOLVER__

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cinttypes>
#include <algorithm>

class EquationSolver
{
private:
    bool debug = false;
    double C[7][7] = { 0.0 };

    void zero_mat(int n);
    void scale_mat(int64_t *_M, int64_t *_D, int64_t *_L, int64_t *_C, uint8_t *_B);

    void load_mat(int n, int64_t T[7][7]);
    void load_mat(int n, double T[7][7]);

    void save_mat(int n, const int64_t T[7][7]);
    void save_mat(int n, const double T[7][7]);

    bool pivot_mat(int k, int n, int64_t T[7][7]);
    bool pivot_mat(int k, int n, double T[7][7]);

    void print_mat(const char *str, int n, const int64_t T[7][7]);
    void print_mat(const char *str, int n, const double T[7][7]);
    void print_mat(char idx, int k, int n, const int64_t T[7][7]);
    void print_mat(char idx, int k, int n, const double T[7][7]);
    void print_mat(char idx, int k, int m, int n, const int64_t T[7][7]);
    void print_mat(char idx, int k, int m, int n, const double T[7][7]);

    void print_res(int n, const double T[7][7]);

public:
    void set_debug(bool val);

    void load_data(const int64_t i64EqualCoeff[7][7], int iParaNum);

    void save_data(double dAffinePara[6], int iParaNum);
    void save_data(double dAffinePara[6], int iParaNum, int frac);

    void print_data(double dAffinePara[6], int iParaNum);

    void method_gja(int n);
    void method_gja2(int n, int q);
    void method_dfa(int n);
    void method_dfa2(int n);
    void method_dfa3(int n);
    void method_dfa4(int n);
    void method_dfa5(int n);
};

#endif // __EQUATION_SOLVER__
