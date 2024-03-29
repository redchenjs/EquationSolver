/*
 * EquationSolver.cpp
 *
 *  Created on: 2022-07-30 18:20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "EquationSolver.h"

using namespace std;

#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)

#define COMP_MAX_BITS     44
#define COMP_DIV_BITS     64

void EquationSolver::set_debug(bool val)
{
    debug = val;
}

void EquationSolver::zero_mat(int n)
{
    for (int p = 0; p < n; p++) {
        C[p][p] = 1.0;
        C[p][n] = 0.0;
    }
}

void EquationSolver::scale_mat(int64_t *_M, int64_t *_D, int64_t *_L, int64_t *_C, uint8_t *_B)
{
    uint8_t DIFF_BITS = 0;

    uint8_t M_BITS = (*_M == 0) ? 0 : (uint8_t)logb(*_M);
    uint8_t D_BITS = (*_D == 0) ? 0 : (uint8_t)logb(*_D);
    uint8_t L_BITS = (*_L == 0) ? 0 : (uint8_t)logb(*_L);
    uint8_t C_BITS = (*_C == 0) ? 0 : (uint8_t)logb(*_C);

    uint8_t MD_BITS = M_BITS + D_BITS;
    uint8_t LC_BITS = L_BITS + C_BITS;

    if (MD_BITS >= LC_BITS) {
        if (MD_BITS > COMP_MAX_BITS) {
            DIFF_BITS = MD_BITS - COMP_MAX_BITS;
        }
    } else {
        if (LC_BITS > COMP_MAX_BITS) {
            DIFF_BITS = LC_BITS - COMP_MAX_BITS;
        }
    }

    if (DIFF_BITS >= 1) {
        *_M = *_M >> DIFF_BITS;
        *_L = *_L >> DIFF_BITS;

        *_B -= DIFF_BITS;
    }
}

void EquationSolver::load_mat(int n, int64_t T[7][7])
{
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            T[p][q] = (int64_t)C[p][q];
        }
    }
}

void EquationSolver::load_mat(int n, double T[7][7])
{
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            T[p][q] = (double)C[p][q];
        }
    }
}

void EquationSolver::load_mat(int n, float T[7][7])
{
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            T[p][q] = (float)C[p][q];
        }
    }
}

void EquationSolver::save_mat(int n, const int64_t T[7][7])
{
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            C[p][q] = (double)T[p][q];
        }
    }
}

void EquationSolver::save_mat(int n, const double T[7][7])
{
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            C[p][q] = (double)T[p][q];
        }
    }
}

void EquationSolver::save_mat(int n, const float T[7][7])
{
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            C[p][q] = (double)T[p][q];
        }
    }
}

bool EquationSolver::pivot_mat(int k, int n, int64_t T[7][7])
{
    // find column max
    int m = k;
    int64_t t = abs(T[k][k]);

    for (int i = k + 1; i < n; i++) {
        if (abs(T[i][k]) > t) {
            t = abs(T[i][k]);
            m = i;
        }
    }

    // swap rows k and m
    if (m != k) {
        print_mat('A', k, m, n, T);

        for (int j = 0; j < n + 1; j++) {
            T[6][j] = T[k][j];
            T[k][j] = T[m][j];
            T[m][j] = T[6][j];
        }

        print_mat('B', k, m, n, T);
    }

    if (T[k][k] == 0) {
        return false;
    }

    return true;
}

bool EquationSolver::pivot_mat(int k, int n, double T[7][7])
{
    // find column max
    int m = k;
    double t = abs(T[k][k]);

    for (int i = k + 1; i < n; i++) {
        if (abs(T[i][k]) > t) {
            t = abs(T[i][k]);
            m = i;
        }
    }

    // swap rows k and m
    if (m != k) {
        print_mat('A', k, m, n, T);

        for (int j = 0; j < n + 1; j++) {
            T[6][j] = T[k][j];
            T[k][j] = T[m][j];
            T[m][j] = T[6][j];
        }

        print_mat('B', k, m, n, T);
    }

    if (T[k][k] == 0.) {
        return false;
    }

    return true;
}

bool EquationSolver::pivot_mat(int k, int n, float T[7][7])
{
    // find column max
    int m = k;
    float t = abs(T[k][k]);

    for (int i = k + 1; i < n; i++) {
        if (abs(T[i][k]) > t) {
            t = abs(T[i][k]);
            m = i;
        }
    }

    // swap rows k and m
    if (m != k) {
        print_mat('A', k, m, n, T);

        for (int j = 0; j < n + 1; j++) {
            T[6][j] = T[k][j];
            T[k][j] = T[m][j];
            T[m][j] = T[6][j];
        }

        print_mat('B', k, m, n, T);
    }

    if (T[k][k] == 0.) {
        return false;
    }

    return true;
}

void EquationSolver::print_mat(const char *str, int n, const int64_t T[7][7])
{
    if (debug) {
        printf("------------------------------- %-5s ------------------------------ n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%7" PRId64 ".0\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7" PRId64 ".0", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(const char *str, int n, const double T[7][7])
{
    if (debug) {
        printf("------------------------------- %-5s ------------------------------ n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1f\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1f", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(const char *str, int n, const float T[7][7])
{
    if (debug) {
        printf("------------------------------- %-5s ------------------------------ n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1f\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1f", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int n, const int64_t T[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------------- n = %d k = %d\n", idx, n, k);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%7" PRId64 ".0\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7" PRId64 ".0", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int n, const double T[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------------- n = %d k = %d\n", idx, n, k);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1f\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1f", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int n, const float T[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------------- n = %d k = %d\n", idx, n, k);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1f\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1f", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int m, int n, const int64_t T[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------- n = %d k = %d m = %d\n", idx, n, k, m);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_E "%7" PRId64 ".0\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7" PRId64 ".0", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int m, int n, const double T[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------- n = %d k = %d m = %d\n", idx, n, k, m);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_E "%9.1f\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1f", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int m, int n, const float T[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------- n = %d k = %d m = %d\n", idx, n, k, m);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_E "%9.1f\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1f", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_res(int n, const double T[7][7])
{
    if (debug) {
        printf("------------------------------- RESULT -----------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf(LOG_COLOR_I "x%d = %9.1f", i, T[i][n] / T[i][i]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::method_gem(int n)
{
    bool zero = false;
    double T[7][7] = { 0.0 };
    double D[7][7] = { 0.0 };

    load_mat(n, T);
    print_mat(" GEM ", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        double M = T[k][k];

        for (int i = k; i < n; i++) {
            double L = T[i][k];

            if (k == i) {
                // make T[i][k] one
                for (int j = k; j < n + 1; j++) {
                    D[k][j] = T[k][j] / M;
                }
            } else {
                // make T[i][k] zero
                for (int j = k; j < n + 1; j++) {
                    double _M = M;
                    double _D = T[i][j];
                    double _L = L;
                    double _C = T[k][j];

                    D[i][j] = _D - (_L / _M) * _C;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_gja(int n)
{
    bool zero = false;
    double T[7][7] = { 0.0 };
    double D[7][7] = { 0.0 };

    load_mat(n, T);
    print_mat(" GJA ", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        double M = T[k][k];

        for (int i = 0; i < n; i++) {
            double L = T[i][k];

            if (k == i) {
                // make T[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j] / M;
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    double _M = M;
                    double _D = T[i][j];
                    double _L = L;
                    double _C = T[k][j];

                    D[i][j] = _D - (_L / _M) * _C;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_gja2(int n, int q)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };
    int64_t F[7][7] = { 0 };

    load_mat(n, T);
    print_mat("GJA-2", n, T);

    // Shift
    for (int row = 0; row < n; row++) {
        for (int i = 0; i < n + 1; i++) {
            F[row][i] = T[row][i] << q;
        }
    }

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, F)) {
            zero = true;
            break;
        }

        int64_t M = F[k][k];

        for (int i = 0; i < n; i++) {
            int64_t L = F[i][k] << q;

            if (k == i) {
                // make T[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (F[k][j] << q) / M;
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    int64_t _M = M;
                    int64_t _D = F[i][j];
                    int64_t _L = L;
                    int64_t _C = F[k][j];

                    D[i][j] = _D - (((_L / _M) * _C) >> q);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                F[i][j] = D[i][j];
            }
        }

        print_mat('F', k, n, F);
    }

    // Shift back
    for (int row = 0; row < n; row++) {
        for (int i = 0; i < n + 1; i++) {
            T[row][i] = F[row][i] >> q;
        }
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_gja3(int n)
{
    bool zero = false;
    float T[7][7] = { 0.0 };
    float D[7][7] = { 0.0 };

    load_mat(n, T);
    print_mat("GJA-3", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        float M = T[k][k];

        for (int i = 0; i < n; i++) {
            float L = T[i][k];

            if (k == i) {
                // make T[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j] / M;
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    float _M = M;
                    float _D = T[i][j];
                    float _L = L;
                    float _C = T[k][j];

                    D[i][j] = _D - (_L / _M) * _C;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_dfa(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat(" DFA ", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int64_t M = T[k][k];

        for (int i = 0; i < n; i++) {
            int64_t L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    int64_t _M = M;
                    int64_t _D = T[i][j];
                    int64_t _L = L;
                    int64_t _C = T[k][j];

                    D[i][j] = (_M * _D - _L * _C);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_dfa2(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA-2", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int64_t M = T[k][k];
        uint8_t B = (M == 0) ? 0 : (uint8_t)logb(M);

        for (int i = 0; i < n; i++) {
            int64_t L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    int64_t _M = M;
                    int64_t _D = T[i][j];
                    int64_t _L = L;
                    int64_t _C = T[k][j];
                    uint8_t _B = B;

                    D[i][j] = (_M * _D - _L * _C);
                    D[i][j] = (D[i][j] < 0) ? -(-D[i][j] >> _B) : D[i][j] >> _B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_dfa3(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA-3", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int64_t M = T[k][k];
        uint8_t B = (M == 0) ? 0 : (uint8_t)logb(M);

        int64_t TM = abs(M) >> (B - 1);
        if ((B >= 1) && (TM & 0x01)) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    int64_t _M = M;
                    int64_t _D = T[i][j];
                    int64_t _L = L;
                    int64_t _C = T[k][j];
                    uint8_t _B = B;

                    D[i][j] = (_M * _D - _L * _C);
                    D[i][j] = (D[i][j] < 0) ? -(-D[i][j] >> _B) : D[i][j] >> _B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_dfa4(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA-4", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int64_t M = T[k][k];
        uint8_t B = (M == 0) ? 0 : (uint8_t)logb(M);

        int64_t TM = abs(M) >> (B - 1);
        if ((B >= 1) && (TM & 0x01)) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    int64_t _M = M;
                    int64_t _D = T[i][j];
                    int64_t _L = L;
                    int64_t _C = T[k][j];
                    uint8_t _B = B;

                    scale_mat(&_M, &_D, &_L, &_C, &_B);

                    D[i][j] = (_M * _D - _L * _C);
                    D[i][j] = (D[i][j] < 0) ? -(-D[i][j] >> _B) : D[i][j] >> _B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::method_dfa5(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA-5", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int64_t M = T[k][k];
        uint8_t B = (M == 0) ? 0 : (uint8_t)logb(M);

        for (int i = 0; i < n; i++) {
            int64_t L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    int64_t _M = M;
                    int64_t _D = T[i][j];
                    int64_t _L = L;
                    int64_t _C = T[k][j];
                    uint8_t _B = B;

                    scale_mat(&_M, &_D, &_L, &_C, &_B);

                    D[i][j] = (_M * _D - _L * _C) >> _B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                T[i][j] = D[i][j];
            }
        }

        print_mat('T', k, n, T);
    }

    save_mat(n, T);

    if (zero) {
        zero_mat(n);
    }

    print_res(n, C);
}

void EquationSolver::load_data(const int64_t i64EqualCoeff[7][7], int iParaNum)
{
    for (int row = 0; row < iParaNum; row++) {
        for (int i = 0; i < iParaNum + 1; i++) {
            C[row][i] = (double)i64EqualCoeff[row + 1][i];
        }
    }
}

void EquationSolver::save_data_gem(double dAffinePara[6], int iParaNum)
{
    dAffinePara[iParaNum - 1] = C[iParaNum - 1][iParaNum] / C[iParaNum - 1][iParaNum - 1];

    for (int i = iParaNum - 2; i >= 0; i--) {
        double temp = 0;

        for (int j = i + 1; j < iParaNum; j++) {
            temp += C[i][j] * dAffinePara[j];
        }

        dAffinePara[i] = (C[i][iParaNum] - temp) / C[i][i];
    }
}

void EquationSolver::save_data(double dAffinePara[6], int iParaNum)
{
    for (int i = 0; i < iParaNum; i++) {
        if (C[i][i] == 0) {
            for (int i = 0; i < iParaNum; i++) {
                dAffinePara[i] = 0;
            }

            return;
        }

        dAffinePara[i] = C[i][iParaNum] / C[i][i];
    }
}

void EquationSolver::save_data(double dAffinePara[6], int iParaNum, int frac)
{
    for (int i = 0; i < iParaNum; i++) {
        int64_t dividend  = (int64_t)C[i][iParaNum];
        int64_t divisor   = (int64_t)C[i][i];
        int64_t divisor_f = divisor >> frac;
        int64_t quotient = 0;

        int64_t _D = dividend;
        int     _F = frac;

        uint8_t D_BITS = (_D == 0) ? 0 : (uint8_t)logb(_D);
        uint8_t F_BITS = (_F == 0) ? 0 : (uint8_t)logb(_F);

        int16_t DF_BITS = D_BITS + F_BITS;

        if (DF_BITS >= COMP_DIV_BITS) {
            if (divisor_f) {
                quotient = dividend / divisor_f;
            } else {
                for (int i = 0; i < iParaNum; i++) {
                    dAffinePara[i] = 0;
                }

                return;
            }
        } else {
            if (divisor) {
                quotient = (dividend << frac) / divisor;
            } else {
                for (int i = 0; i < iParaNum; i++) {
                    dAffinePara[i] = 0;
                }

                return;
            }
        }

        dAffinePara[i] = quotient / pow(2.0, frac);
    }
}

void EquationSolver::print_data(double dAffinePara[6], int iParaNum)
{
    for (int i = 0; i < iParaNum; i++) {
        printf(LOG_COLOR_W "x%d = %9.1f\n", i, dAffinePara[i]);
    }
}
