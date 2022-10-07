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

void EquationSolver::set_debug(bool val)
{
    debug = val;
}

void EquationSolver::zero_mat(int n, long double C[7][7])
{
    for (int p = 0; p < n; p++) {
        C[p][p] = 1.0;
        C[p][n] = 0.0;
    }
}

bool EquationSolver::pivot_mat(int k, int n, int64_t C[7][7])
{
    // find column max
    int m = k;
    int64_t t = abs(C[k][k]);

    for (int i = k + 1; i < n; i++) {
        if (abs(C[i][k]) > t) {
            t = abs(C[i][k]);
            m = i;
        }
    }

    // swap rows k and m
    if (m != k) {
        print_mat('A', k, m, n, C);

        for (int j = 0; j < n + 1; j++) {
            C[6][j] = C[k][j];
            C[k][j] = C[m][j];
            C[m][j] = C[6][j];
        }

        print_mat('B', k, m, n, C);
    }

    if (C[k][k] == 0) {
        return false;
    }

    return true;
}

bool EquationSolver::pivot_mat(int k, int n, long double C[7][7])
{
    // find column max
    int m = k;
    long double t = abs(C[k][k]);

    for (int i = k + 1; i < n; i++) {
        if (abs(C[i][k]) > t) {
            t = abs(C[i][k]);
            m = i;
        }
    }

    // swap rows k and m
    if (m != k) {
        print_mat('A', k, m, n, C);

        for (int j = 0; j < n + 1; j++) {
            C[6][j] = C[k][j];
            C[k][j] = C[m][j];
            C[m][j] = C[6][j];
        }

        print_mat('B', k, m, n, C);
    }

    if (C[k][k] == 0.) {
        return false;
    }

    return true;
}

void EquationSolver::print_mat(const char *str, int n, long double C[7][7])
{
    if (debug) {
        printf("------------------------------- %-7s ---------------------------- n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1Lf\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1Lf", C[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int n, int64_t C[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------------- n = %d k = %d\n", idx, n, k);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%7ld.0\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7ld.0", C[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int n, long double C[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------------- n = %d k = %d\n", idx, n, k);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1Lf\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1Lf", C[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int m, int n, int64_t C[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------- n = %d k = %d m = %d\n", idx, n, k, m);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_E "%7ld.0\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%7ld.0", C[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_mat(char idx, int k, int m, int n, long double C[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------- n = %d k = %d m = %d\n", idx, n, k, m);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_E "%9.1Lf\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1Lf", C[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_res(int n, long double C[7][7])
{
    if (debug) {
        printf("------------------------------- RESULT -----------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf(LOG_COLOR_I "x%d = %9.1Lf", i, C[i][n] / C[i][i]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_res(int n, long double C[7][7], int scale)
{
    if (debug) {
        printf("------------------------------- RESULT-%d ---------------------------------\n", scale);
        for (int i = 0; i < n; i++) {
            printf(LOG_COLOR_I "x%d = %9.1Lf", i, C[i][n] / C[i][i] / pow(2.0, scale));
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::method_gja(int n)
{
    bool zero = false;
    long double D[7][7] = { 0.0 };

    print_mat("GJA", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        long double M = C[k][k];

        for (int i = 0; i < n; i++) {
            long double L = C[i][k];

            if (k == i) {
                // make C[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j] / M;
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = C[i][j] - (L / M) * C[k][j];
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C);
}

void EquationSolver::method_gja2(int n, int q, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };
    int64_t F[7][7] = { 0 };

    print_mat("GJA-2", n, C);

    // Shift
    for (int row = 0; row < n; row++) {
        for (int i = 0; i < n + 1; i++) {
            F[row][i] = (int64_t)(C[row][i] * pow(2.0, q));
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
                // make C[i][k] one
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (F[k][j] << q) / M;
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = F[i][j] - (((L / M) * F[k][j]) >> q);
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
            C[row][i] = (long double)(F[row][i] / pow(2.0, q));
        }
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int64_t M = (int64_t)C[k][k];

        for (int i = 0; i < n; i++) {
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(M * C[i][j] - L * C[k][j]);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

#ifdef __GNUC__
void EquationSolver::method_dfa128(int n, int scale)
{
    bool zero = false;
    __int128_t D[7][7] = { 0 };

    print_mat("DFA-128", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        __int128_t M = (__int128_t)C[k][k];

        for (int i = 0; i < n; i++) {
            __int128_t L = (__int128_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (__int128_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (__int128_t)(M * C[i][j] - L * C[k][j]);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}
#endif

void EquationSolver::method_dfa2(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA2", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t M = (int64_t)C[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(M * C[i][j] - L * C[k][j]);
                    D[i][j] = D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa2s(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA2-S", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t M = (int64_t)C[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(M * C[i][j] - L * C[k][j]);
                    D[i][j] = M < 0 ? -D[i][j] >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa3(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA3", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t M = (int64_t)C[k][k];
        int64_t TM = abs(M);
        int64_t HM = abs(M);

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
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(M * C[i][j] - L * C[k][j]);
                    D[i][j] = D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa3s(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA3-S", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t M = (int64_t)C[k][k];
        int64_t TM = abs(M);
        int64_t HM = abs(M);

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
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(M * C[i][j] - L * C[k][j]);
                    D[i][j] = M < 0 ? -D[i][j] >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa4a(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA4-A", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t M = (int64_t)C[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(C[i][j] - (L >> B) * C[k][j]);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa4b(int n, int scale)
{
    bool zero = false;
    int64_t D[7][7] = { 0 };

    print_mat("DFA4-B", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        int B = 0;
        int64_t M = (int64_t)C[k][k];
        int64_t TM = abs(M);
        int64_t HM = abs(M);

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
            int64_t L = (int64_t)C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = (int64_t)C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = (int64_t)(C[i][j] - (L >> B) * C[k][j]);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (long double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_cra(int n, int scale)
{
    int64_t D[13] = { 0 };
    int64_t D_I[6][7] = { 0 };

    print_mat("CRA", n, C);

    if (n == 4) {
        for (int i = 5; i >= 2; i--) {
            for (int j = 6; j >= 2; j--) {
                C[i][j] = C[i - 2][j - 2];
            }
        }
    }

    // D
    D[0] = (int64_t)(C[0][0] * C[1][1] - C[0][1] * C[1][0]);
    D[1] = (int64_t)(C[0][2] * C[1][3] - C[0][3] * C[1][2]);
    D[2] = (int64_t)(C[0][4] * C[1][5] - C[0][5] * C[1][4]);
    D[3] = (int64_t)(C[2][0] * C[3][1] - C[2][1] * C[3][0]);
    D[4] = (int64_t)(C[2][2] * C[3][3] - C[2][3] * C[3][2]);
    D[5] = (int64_t)(C[2][4] * C[3][5] - C[2][5] * C[3][4]);
    D[6] = (int64_t)(C[4][0] * C[5][1] - C[4][1] * C[5][0]);
    D[7] = (int64_t)(C[4][2] * C[5][3] - C[4][3] * C[5][2]);
    D[8] = (int64_t)(C[4][4] * C[5][5] - C[4][5] * C[5][4]);

    D[9]  = D[1] * D[5] - D[2] * D[4];
    D[10] = D[1] * D[8] - D[2] * D[7];
    D[11] = D[4] * D[8] - D[5] * D[7];

    D[12] = D[0] * D[11] - D[3] * D[10] + D[6] * D[9];

    // D_0
    D_I[0][0] = (int64_t)(C[0][6] * C[1][1] - C[0][1] * C[1][6]);
    D_I[0][1] = (int64_t)(C[2][6] * C[3][1] - C[2][1] * C[3][6]);
    D_I[0][2] = (int64_t)(C[4][6] * C[5][1] - C[4][1] * C[5][6]);

    D_I[0][6] = D_I[0][0] * D[11] - D_I[0][1] * D[10] + D_I[0][2] * D[9];

    // D_1
    D_I[1][0] = (int64_t)(C[0][0] * C[1][6] - C[0][6] * C[1][0]);
    D_I[1][1] = (int64_t)(C[2][0] * C[3][6] - C[2][6] * C[3][0]);
    D_I[1][2] = (int64_t)(C[4][0] * C[5][6] - C[4][6] * C[5][0]);

    D_I[1][6] = D_I[1][0] * D[11] - D_I[1][1] * D[10] + D_I[1][2] * D[9];

    // D_2
    D_I[2][0] = (int64_t)(C[0][6] * C[1][3] - C[0][3] * C[1][6]);
    D_I[2][1] = (int64_t)(C[2][6] * C[3][3] - C[2][3] * C[3][6]);
    D_I[2][2] = (int64_t)(C[4][6] * C[5][3] - C[4][3] * C[5][6]);

    D_I[2][3] = D_I[2][0] * D[5] - D[2] * D_I[2][1];
    D_I[2][4] = D_I[2][0] * D[8] - D[2] * D_I[2][2];
    D_I[2][5] = D_I[2][1] * D[8] - D[5] * D_I[2][2];

    D_I[2][6] = D_I[2][3] * D[11] - D_I[2][4] * D[10] + D_I[2][5] * D[9];

    // D_3
    D_I[3][0] = (int64_t)(C[0][2] * C[1][6] - C[0][6] * C[1][2]);
    D_I[3][1] = (int64_t)(C[2][2] * C[3][6] - C[2][6] * C[3][2]);
    D_I[3][2] = (int64_t)(C[4][2] * C[5][6] - C[4][6] * C[5][2]);

    D_I[3][3] = D_I[3][0] * D[5] - D[2] * D_I[3][1];
    D_I[3][4] = D_I[3][0] * D[8] - D[2] * D_I[3][2];
    D_I[3][5] = D_I[3][1] * D[8] - D[5] * D_I[3][2];

    D_I[3][6] = D_I[3][3] * D[11] - D_I[3][4] * D[10] + D_I[3][5] * D[9];

    // D_4
    D_I[4][0] = (int64_t)(C[0][6] * C[1][5] - C[0][5] * C[1][6]);
    D_I[4][1] = (int64_t)(C[2][6] * C[3][5] - C[2][5] * C[3][6]);
    D_I[4][2] = (int64_t)(C[4][6] * C[5][5] - C[4][5] * C[5][6]);

    D_I[4][3] = D[1] * D_I[4][1] - D_I[4][0] * D[4];
    D_I[4][4] = D[2] * D_I[4][2] - D_I[4][0] * D[7];
    D_I[4][5] = D[4] * D_I[4][2] - D_I[4][1] * D[7];

    D_I[4][6] = D[0] * D_I[4][3] - D[3] * D_I[4][4] + D[6] * D_I[4][5];

    // D_5
    D_I[5][0] = (int64_t)(C[0][4] * C[1][6] - C[0][6] * C[1][4]);
    D_I[5][1] = (int64_t)(C[2][4] * C[3][6] - C[2][6] * C[3][4]);
    D_I[5][2] = (int64_t)(C[4][4] * C[5][6] - C[4][6] * C[5][4]);

    D_I[5][3] = D[1] * D_I[5][1] - D_I[5][0] * D[4];
    D_I[5][4] = D[2] * D_I[5][2] - D_I[5][0] * D[7];
    D_I[5][5] = D[4] * D_I[5][2] - D_I[5][1] * D[7];

    D_I[5][6] = D[0] * D_I[5][3] - D[3] * D_I[5][4] + D[6] * D_I[5][5];

    memset(C, 0x00, sizeof(C));

    if (n == 6) {
        C[0][0] = (long double)D[12];
        C[1][1] = (long double)D[12];
        C[2][2] = (long double)D[12];
        C[3][3] = (long double)D[12];
        C[4][4] = (long double)D[12];
        C[5][5] = (long double)D[12];

        C[0][6] = (long double)D_I[0][6];
        C[1][6] = (long double)D_I[1][6];
        C[2][6] = (long double)D_I[2][6];
        C[3][6] = (long double)D_I[3][6];
        C[4][6] = (long double)D_I[4][6];
        C[5][6] = (long double)D_I[5][6];
    } else {
        C[0][0] = (long double)D[11];
        C[1][1] = (long double)D[11];
        C[2][2] = (long double)D[11];
        C[3][3] = (long double)D[11];

        C[0][4] = (long double)D_I[2][5];
        C[1][4] = (long double)D_I[3][5];
        C[2][4] = (long double)D_I[4][5];
        C[3][4] = (long double)D_I[5][5];
    }

    print_mat("CRA_O", n, C);
    print_res(n, C, scale);
}

void EquationSolver::load_data(const int64_t i64EqualCoeff[7][7], int iParaNum, int scale)
{
    for (int row = 0; row < iParaNum; row++) {
        for (int i = 0; i < iParaNum + 1; i++) {
            if (i == iParaNum) {
                C[row][i] = (long double)(i64EqualCoeff[row + 1][i] << scale);
            } else {
                C[row][i] = (long double)i64EqualCoeff[row + 1][i];
            }
        }
    }
}

void EquationSolver::save_data(double dAffinePara[6], int iParaNum, int scale)
{
    for (int i = 0; i < iParaNum; i++) {
        dAffinePara[i] = C[i][iParaNum] / C[i][i] / pow(2.0, scale);
    }
}
