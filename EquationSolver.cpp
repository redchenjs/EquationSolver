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

void EquationSolver::zero_mat(int n, double C[7][7])
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

bool EquationSolver::pivot_mat(int k, int n, double C[7][7])
{
    // find column max
    int m = k;
    double t = abs(C[k][k]);

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

void EquationSolver::print_mat(const char *str, int n, double C[7][7])
{
    if (debug) {
        printf("------------------------------- %-6s ----------------------------- n = %d\n", str, n);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1lf\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1lf", C[p][n]);
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

void EquationSolver::print_mat(char idx, int k, int n, double C[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------------- n = %d k = %d\n", idx, n, k);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_I "%9.1lf\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1lf", C[p][n]);
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

void EquationSolver::print_mat(char idx, int k, int m, int n, double C[7][7])
{
    if (debug) {
        printf("------------------------------- MATRIX %c --------------- n = %d k = %d m = %d\n", idx, n, k, m);
        for (int p = 0; p < n; p++) {
            printf(LOG_COLOR_W "r%d ", p);
            printf(LOG_RESET_COLOR "|");
            for (int q = 0; q < n; q++) {
                printf(LOG_COLOR_E "%9.1lf\t", C[p][q]);
            }
            printf(LOG_RESET_COLOR "|");
            printf(LOG_COLOR_W "%9.1lf", C[p][n]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_res(int n, double C[7][7])
{
    if (debug) {
        printf("------------------------------- RESULT -----------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf(LOG_COLOR_I "x%d = %9.1lf", i, C[i][n] / C[i][i]);
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::print_res(int n, double C[7][7], int scale)
{
    if (debug) {
        printf("------------------------------- RESULT-%d ---------------------------------\n", scale);
        for (int i = 0; i < n; i++) {
            printf(LOG_COLOR_I "x%d = %9.1lf", i, C[i][n] / C[i][i] / pow(2.0, scale));
            printf(LOG_RESET_COLOR "\n");
        }
    }
}

void EquationSolver::method_gja(int n)
{
    bool zero = false;
    double D[7][7] = { 0.0 };

    print_mat("GJA", n, C);

    for (int k = 0; k < n; k++) {
        if (!pivot_mat(k, n, C)) {
            zero = true;
            break;
        }

        double M = C[k][k];

        for (int i = 0; i < n; i++) {
            double L = C[i][k];

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
            C[row][i] = (double)(F[row][i] / pow(2.0, q));
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
                C[i][j] = (double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

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
                C[i][j] = (double)D[i][j];
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
                    D[i][j] = (M < 0) ? (-D[i][j]) >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (double)D[i][j];
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
                C[i][j] = (double)D[i][j];
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
                    D[i][j] = (M < 0) ? (-D[i][j]) >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = (double)D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    if (zero) {
        zero_mat(n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::load_data(const int64_t i64EqualCoeff[7][7], int iParaNum, int scale)
{
    for (int row = 0; row < iParaNum; row++) {
        for (int i = 0; i < iParaNum + 1; i++) {
            if (i == iParaNum) {
                C[row][i] = (double)(i64EqualCoeff[row + 1][i] << scale);
            } else {
                C[row][i] = (double)i64EqualCoeff[row + 1][i];
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
