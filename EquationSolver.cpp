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

void EquationSolver::print_mat(const char *str, int n, double C[7][7])
{
#ifdef DEBUG
    printf("------------------------------- %-6s ---------------------------- n = %d\n", str, n);
    for (int p = 0; p < n; p++) {
        printf(LOG_COLOR_W "r%d ", p);
        printf(LOG_RESET_COLOR "|");
        for (int q = 0; q < n; q++) {
            printf(LOG_COLOR_I "%8.1f\t", C[p][q]);
        }
        printf(LOG_RESET_COLOR "|");
        printf(LOG_COLOR_W "%8.1f", C[p][n]);
        printf(LOG_RESET_COLOR "\n");
    }
#endif
}

void EquationSolver::print_mat(char idx, int k, int n, double C[7][7])
{
#ifdef DEBUG
    printf("------------------------------- MATRIX %c -------------------- n = %d k = %d\n", idx, n, k);
    for (int p = 0; p < n; p++) {
        printf(LOG_COLOR_W "r%d ", p);
        printf(LOG_RESET_COLOR "|");
        for (int q = 0; q < n; q++) {
            printf(LOG_COLOR_I "%8.1f\t", C[p][q]);
        }
        printf(LOG_RESET_COLOR "|");
        printf(LOG_COLOR_W "%8.1f", C[p][n]);
        printf(LOG_RESET_COLOR "\n");
    }
#endif
}

void EquationSolver::print_mat(char idx, int k, int m, int n, double C[7][7])
{
#ifdef DEBUG
    printf("------------------------------- MATRIX %c -------------- n = %d k = %d m = %d\n", idx, n, k, m);
    for (int p = 0; p < n; p++) {
        printf(LOG_COLOR_W "r%d ", p);
        printf(LOG_RESET_COLOR "|");
        for (int q = 0; q < n; q++) {
            printf(LOG_COLOR_E "%8.1f\t", C[p][q]);
        }
        printf(LOG_RESET_COLOR "|");
        printf(LOG_COLOR_W "%8.1f", C[p][n]);
        printf(LOG_RESET_COLOR "\n");
    }
#endif
}

void EquationSolver::print_res(int n, double C[7][7])
{
#ifdef DEBUG
    printf("------------------------------- RESULT ----------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(LOG_COLOR_I "x%d = %8.1f", i, C[i][n] / C[i][i]);
        printf(LOG_RESET_COLOR "\n");
    }
#endif
}

void EquationSolver::print_res(int n, double C[7][7], int scale)
{
#ifdef DEBUG
    printf("------------------------------- RESULT ----------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(LOG_COLOR_I "x%d = %8.1f", i, C[i][n] / C[i][i] / pow(2.0, scale));
        printf(LOG_RESET_COLOR "\n");
    }
#endif
}

void EquationSolver::method_gja(int n)
{
    double D[7][7] = { 0.0 };

    print_mat("GJA", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        double t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

    print_res(n, C);
}

void EquationSolver::method_dfa(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA", (n == 6), C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int64_t M = C[k][k];

        for (int i = 0; i < n; i++) {
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = M * C[i][j] - L * C[k][j];
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

    print_res(n, C, scale);
}

void EquationSolver::method_dfa2(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA2", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int B = 0;
        int64_t M = C[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = M * C[i][j] - L * C[k][j];
                    D[i][j] = D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa2s(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA2-S", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int B = 0;
        int64_t M = C[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = M * C[i][j] - L * C[k][j];
                    D[i][j] = M < 0 ? -D[i][j] >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa3(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA3", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int B = 0;
        int64_t M = C[k][k];
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
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = M * C[i][j] - L * C[k][j];
                    D[i][j] = D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa3s(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA3-S", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int B = 0;
        int64_t M = C[k][k];
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
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = M * C[i][j] - L * C[k][j];
                    D[i][j] = M < 0 ? -D[i][j] >> B : D[i][j] >> B;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa4a(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA4-A", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int B = 0;
        int64_t M = C[k][k];
        int64_t TM = abs(M);

        while (TM >>= 1) {
            B++;
        }

        for (int i = 0; i < n; i++) {
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = C[i][j] - (L >> B) * C[k][j];
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
    }

    print_res(n, C, scale);
}

void EquationSolver::method_dfa4b(int n, int scale)
{
    int64_t D[7][7] = { 0 };

    print_mat("DFA4-B", n, C);

    for (int k = 0; k < n; k++) {
        // find column max
        int m = k;
        int64_t t = fabs(C[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(C[i][k]) > t) {
                t = fabs(C[i][k]);
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

        int B = 0;
        int64_t M = C[k][k];
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
            int64_t L = C[i][k];

            if (k == i) {
                // row k is not modified
                for (int j = 0; j < n + 1; j++) {
                    D[k][j] = C[k][j];
                }
            } else {
                // make C[i][k] zero
                for (int j = 0; j < n + 1; j++) {
                    D[i][j] = C[i][j] - (L >> B) * C[k][j];
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 2; j++) {
                C[i][j] = D[i][j];
            }
        }

        print_mat('C', k, n, C);
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
    D[0] = C[0][0] * C[1][1] - C[0][1] * C[1][0];
    D[1] = C[0][2] * C[1][3] - C[0][3] * C[1][2];
    D[2] = C[0][4] * C[1][5] - C[0][5] * C[1][4];
    D[3] = C[2][0] * C[3][1] - C[2][1] * C[3][0];
    D[4] = C[2][2] * C[3][3] - C[2][3] * C[3][2];
    D[5] = C[2][4] * C[3][5] - C[2][5] * C[3][4];
    D[6] = C[4][0] * C[5][1] - C[4][1] * C[5][0];
    D[7] = C[4][2] * C[5][3] - C[4][3] * C[5][2];
    D[8] = C[4][4] * C[5][5] - C[4][5] * C[5][4];

    D[9]  = D[1] * D[5] - D[2] * D[4];
    D[10] = D[1] * D[8] - D[2] * D[7];
    D[11] = D[4] * D[8] - D[5] * D[7];

    D[12] = D[0] * D[11] - D[3] * D[10] + D[6] * D[9];

    // D_0
    D_I[0][0] = C[0][6] * C[1][1] - C[0][1] * C[1][6];
    D_I[0][1] = C[2][6] * C[3][1] - C[2][1] * C[3][6];
    D_I[0][2] = C[4][6] * C[5][1] - C[4][1] * C[5][6];

    D_I[0][6] = D_I[0][0] * D[11] - D_I[0][1] * D[10] + D_I[0][2] * D[9];

    // D_1
    D_I[1][0] = C[0][0] * C[1][6] - C[0][6] * C[1][0];
    D_I[1][1] = C[2][0] * C[3][6] - C[2][6] * C[3][0];
    D_I[1][2] = C[4][0] * C[5][6] - C[4][6] * C[5][0];

    D_I[1][6] = D_I[1][0] * D[11] - D_I[1][1] * D[10] + D_I[1][2] * D[9];

    // D_2
    D_I[2][0] = C[0][6] * C[1][3] - C[0][3] * C[1][6];
    D_I[2][1] = C[2][6] * C[3][3] - C[2][3] * C[3][6];
    D_I[2][2] = C[4][6] * C[5][3] - C[4][3] * C[5][6];

    D_I[2][3] = D_I[2][0] * D[5] - D[2] * D_I[2][1];
    D_I[2][4] = D_I[2][0] * D[8] - D[2] * D_I[2][2];
    D_I[2][5] = D_I[2][1] * D[8] - D[5] * D_I[2][2];

    D_I[2][6] = D_I[2][3] * D[11] - D_I[2][4] * D[10] + D_I[2][5] * D[9];

    // D_3
    D_I[3][0] = C[0][2] * C[1][6] - C[0][6] * C[1][2];
    D_I[3][1] = C[2][2] * C[3][6] - C[2][6] * C[3][2];
    D_I[3][2] = C[4][2] * C[5][6] - C[4][6] * C[5][2];

    D_I[3][3] = D_I[3][0] * D[5] - D[2] * D_I[3][1];
    D_I[3][4] = D_I[3][0] * D[8] - D[2] * D_I[3][2];
    D_I[3][5] = D_I[3][1] * D[8] - D[5] * D_I[3][2];

    D_I[3][6] = D_I[3][3] * D[11] - D_I[3][4] * D[10] + D_I[3][5] * D[9];

    // D_4
    D_I[4][0] = C[0][6] * C[1][5] - C[0][5] * C[1][6];
    D_I[4][1] = C[2][6] * C[3][5] - C[2][5] * C[3][6];
    D_I[4][2] = C[4][6] * C[5][5] - C[4][5] * C[5][6];

    D_I[4][3] = D[1] * D_I[4][1] - D_I[4][0] * D[4];
    D_I[4][4] = D[2] * D_I[4][2] - D_I[4][0] * D[7];
    D_I[4][5] = D[4] * D_I[4][2] - D_I[4][1] * D[7];

    D_I[4][6] = D[0] * D_I[4][3] - D[3] * D_I[4][4] + D[6] * D_I[4][5];

    // D_5
    D_I[5][0] = C[0][4] * C[1][6] - C[0][6] * C[1][4];
    D_I[5][1] = C[2][4] * C[3][6] - C[2][6] * C[3][4];
    D_I[5][2] = C[4][4] * C[5][6] - C[4][6] * C[5][4];

    D_I[5][3] = D[1] * D_I[5][1] - D_I[5][0] * D[4];
    D_I[5][4] = D[2] * D_I[5][2] - D_I[5][0] * D[7];
    D_I[5][5] = D[4] * D_I[5][2] - D_I[5][1] * D[7];

    D_I[5][6] = D[0] * D_I[5][3] - D[3] * D_I[5][4] + D[6] * D_I[5][5];

    memset(C, 0x00, sizeof(C));

    if (n == 6) {
        C[0][0] = D[12];
        C[1][1] = D[12];
        C[2][2] = D[12];
        C[3][3] = D[12];
        C[4][4] = D[12];
        C[5][5] = D[12];

        C[0][6] = D_I[0][6];
        C[1][6] = D_I[1][6];
        C[2][6] = D_I[2][6];
        C[3][6] = D_I[3][6];
        C[4][6] = D_I[4][6];
        C[5][6] = D_I[5][6];
    } else {
        C[0][0] = D[11];
        C[1][1] = D[11];
        C[2][2] = D[11];
        C[3][3] = D[11];

        C[0][4] = D_I[2][5];
        C[1][4] = D_I[3][5];
        C[2][4] = D_I[4][5];
        C[3][4] = D_I[5][5];
    }

    print_mat("CRA_O", n, C);
    print_res(n, C, scale);
}

void EquationSolver::load_data(const int64_t i64EqualCoeff[7][7], int iParaNum, int scale)
{
    for (int row = 0; row < iParaNum; row++) {
        for (int i = 0; i < iParaNum + 1; i++) {
            if (i == iParaNum) {
                C[row][i] = i64EqualCoeff[row + 1][i] << scale;
            } else {
                C[row][i] = i64EqualCoeff[row + 1][i];
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
