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

#define COMP_MAX_BITS     61

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

    uint8_t M_BITS = (*_M == 0) ? 0 : logb(*_M);
    uint8_t D_BITS = (*_D == 0) ? 0 : logb(*_D);
    uint8_t L_BITS = (*_L == 0) ? 0 : logb(*_L);
    uint8_t C_BITS = (*_C == 0) ? 0 : logb(*_C);

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
        if (M_BITS >= D_BITS) {
            *_M >>= DIFF_BITS;
        } else {
            *_D >>= DIFF_BITS;
        }

        if (L_BITS >= C_BITS) {
            *_L >>= DIFF_BITS;
        } else {
            *_C >>= DIFF_BITS;
        }

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

void EquationSolver::print_mat(const char *str, int n, const int64_t T[7][7])
{
    if (debug) {
        printf("------------------------------- %-6s ----------------------------- n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%7ld.0\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7ld.0", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(const char *str, int n, const double T[7][7])
{
    if (debug) {
        printf("------------------------------- %-6s ----------------------------- n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1lf\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1lf", T[p][n]);
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
                printf(LOG_COLOR_I "%7ld.0\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7ld.0", T[p][n]);
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
                printf(LOG_COLOR_I "%9.1lf\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1lf", T[p][n]);
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
                printf(LOG_COLOR_E "%7ld.0\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7ld.0", T[p][n]);
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
                printf(LOG_COLOR_E "%9.1lf\t", T[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1lf", T[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_res(int n, const double T[7][7])
{
    if (debug) {
        printf("------------------------------- RESULT -----------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf(LOG_COLOR_I "x%d = %9.1lf", i, T[i][n] / T[i][i]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::method_gja(int n)
{
    bool zero = false;
    double T[7][7] = { 0.0 };
    double D[7][7] = { 0.0 };

    load_mat(n, T);
    print_mat("GJA", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        double _M = T[k][k];

        for (int i = 0; i < n; i++) {
            double _L = T[i][k];

            if (k == i) {
                // make T[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j] / _M;
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = T[i][j] - (_L / _M) * T[k][j];
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

        int64_t _M = F[k][k];

        for (int i = 0; i < n; i++) {
            int64_t _L = F[i][k] << q;

            if (k == i) {
                // make T[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (F[k][j] << q) / _M;
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = F[i][j] - (((_L / _M) * F[k][j]) >> q);
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

void EquationSolver::method_dfa(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA", n, T);

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
    print_mat("DFA2", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t _M = T[k][k];
        int64_t TM = abs(_M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t _L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (_M * T[i][j] - _L * T[k][j]);
                    D[i][j] = D[i][j] >> B;
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

void EquationSolver::method_dfa2s(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA2S", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t _M = T[k][k];
        int64_t TM = abs(_M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t _L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (_M * T[i][j] - _L * T[k][j]);
                    D[i][j] = (_M < 0) ? (-D[i][j]) >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                T[i][j] = (double)D[i][j];
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
    print_mat("DFA3", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t _M = T[k][k];
        int64_t TM = abs(_M);
        int64_t HM = abs(_M);

        while (TM >>= 1) {
            B++;
        }

        if (B != 0) {
            HM >>= B - 1;

            if (HM & 0x01) {
                B++;
            }
        }

        for (int i = 0; i < n; i++) {
            int64_t _L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (_M * T[i][j] - _L * T[k][j]);
                    D[i][j] = D[i][j] >> B;
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

void EquationSolver::method_dfa3s(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA3S", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t _M = T[k][k];
        int64_t TM = abs(_M);
        int64_t HM = abs(_M);

        while (TM >>= 1) {
            B++;
        }

        if (B != 0) {
            HM >>= B - 1;

            if (HM & 0x01) {
                B++;
            }
        }

        for (int i = 0; i < n; i++) {
            int64_t _L = T[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = T[k][j];
                }
            } else {
                // make T[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (_M * T[i][j] - _L * T[k][j]);
                    D[i][j] = (_M < 0) ? (-D[i][j]) >> B : D[i][j] >> B;
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
    print_mat("DFA4", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        uint8_t B = 0;
        int64_t M = T[k][k];
        int64_t TM = abs(M);
        int64_t HM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        if (B >= 1) {
            HM >>= B - 1;

            if (HM & 0x01) {
                B++;
            }
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

void EquationSolver::method_dfa4s(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA4S", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        uint8_t B = 0;
        int64_t M = T[k][k];
        int64_t TM = abs(M);
        int64_t HM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        if (B >= 1) {
            HM >>= B - 1;

            if (HM & 0x01) {
                B++;
            }
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
    print_mat("DFA5", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        uint8_t B = 0;
        int64_t M = T[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
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

void EquationSolver::method_dfa5s(int n)
{
    bool zero = false;
    int64_t T[7][7] = { 0 };
    int64_t D[7][7] = { 0 };

    load_mat(n, T);
    print_mat("DFA5S", n, T);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, T)) {
            zero = true;
            break;
        }

        uint8_t B = 0;
        int64_t M = T[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
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

void EquationSolver::load_data(const int64_t i64EqualCoeff[7][7], int iParaNum)
{
    for (int row = 0; row < iParaNum; row++) {
        for (int i = 0; i < iParaNum + 1; i++) {
            C[row][i] = (double)i64EqualCoeff[row + 1][i];
        }
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
        if (C[i][i] == 0) {
            for (int i = 0; i < iParaNum; i++) {
                dAffinePara[i] = 0;
            }

            return;
        }

        int64_t dividend  = (int64_t)C[i][iParaNum];
        int64_t divisor   = (int64_t)C[i][i];
        int64_t divisor_f = (divisor < 0) ? -(-divisor >> frac) : divisor >> frac;
        int64_t quotient = 0;

        int64_t *_M = &dividend;
        int     *_D = &frac;
        int64_t *_L = &divisor;
        int     *_C = &frac;

        uint8_t M_BITS = (*_M == 0) ? 0 : logb(*_M);
        uint8_t D_BITS = (*_D == 0) ? 0 : logb(*_D);
        uint8_t L_BITS = (*_L == 0) ? 0 : logb(*_L);
        uint8_t C_BITS = (*_C == 0) ? 0 : logb(*_C);

        int16_t MD_BITS = M_BITS + D_BITS;
        int16_t LC_BITS = L_BITS - C_BITS;

        if (MD_BITS > COMP_MAX_BITS) {
            if (LC_BITS >= 1) {
                quotient = dividend / divisor_f;
            } else {
                quotient = dividend;
            }
        } else {
            quotient = (dividend << frac) / divisor;
        }

        dAffinePara[i] = quotient / pow(2.0, frac);
    }
}

void EquationSolver::print_data(double dAffinePara[6], int iParaNum)
{
    for (int i = 0; i < iParaNum; i++) {
        printf(LOG_COLOR_W "x%d = %9.1lf\n", i, dAffinePara[i]);
    }
}
