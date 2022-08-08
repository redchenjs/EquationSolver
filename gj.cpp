/*
 * gj.cpp
 *
 *  Created on: 2022-07-30 18:20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

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

/*
 2  -3   1   5   6
-3   1   2  -4   5
-1  -2   3   1  11
 0   0   1   1   2

 x0 = -3
 x1 = -2
 x2 =  1
 x3 =  1
*/

/*
 0   1   2   2
 1   1   1   1
 0  -1  -4  -2
 3   2   1  -1

 x0 = -2
 x1 = -3
 x2 = -1
 x3 =  0
*/

const double data[7][7] = {
    [0] = { 0,   1,   2,   2,   1,   0,   0},
    [1] = { 1,   1,   1,   1,   0,   0,   0},
    [2] = { 0,  -1,  -4,  -2,   1,   0,   0},
    [3] = { 3,   2,   1,  -1,  -1,   0,   0},
    [4] = { 0,   0,   0,   0,   0,   0,   0},
    [5] = { 0,   0,   0,   0,   0,   0,   0},
    [6] = { 0,   0,   0,   0,   0,   0,   0},
};

void print_mat(const char *str, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    printf("------------------------------- %-4s ------------------------------ n = %d\n", str, n);
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
}

void print_mat(char idx, int k, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

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
}

void print_mat(char idx, int k, int m, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

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
}

void print_res(bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    printf("------------------------------- RESULT ----------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(LOG_COLOR_I "x%d = %8.1f", i, C[i][n] / C[i][i]);
        printf(LOG_RESET_COLOR "\n");
    }
}

void print_res(int a, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    printf("------------------------------- RESULT ----------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(LOG_COLOR_I "x%d = %8.1f", i, C[i][n] / C[i][i]);
        printf(LOG_RESET_COLOR "\n");
    }
}

void scale_mat(int b, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
            if (b >= 0) {
                C[p][q] = C[p][q] * pow(2, b);
            } else {
                C[p][q] = C[p][q] / pow(2, b);
            }
        }
    }
}

void scale_mat(int a, int b, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n - 1; q++) {

            C[p][q] = C[p][q] * pow(2, a);
        }
        C[p][n] = C[p][n] * pow(2, b);
    }
}

void method_gja(bool param6, bool pivoting, double C[7][7])
{
    int n = param6 ? 6 : 4;
    double D[7][7] = { 0 };

    print_mat("GJA", param6, C);

    for (int k = 0; k < n; k++) {
        if (pivoting) {
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
                print_mat('A', k, m, param6, C);

                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }

                print_mat('B', k, m, param6, C);
            }
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

        print_mat('C', k, param6, C);
    }

    print_res(param6, C);
}

void method_dfa(bool param6, bool pivoting, double C[7][7])
{
    int n = param6 ? 6 : 4;
    int64_t D[7][7] = { 0 };

    print_mat("DFA", param6, C);

    for (int k = 0; k < n; k++) {
        if (pivoting) {
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
                print_mat('A', k, m, param6, C);

                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }

                print_mat('B', k, m, param6, C);
            }
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

        print_mat('C', k, param6, C);
    }

    print_res(param6, C);
}

void method_dfa2(bool param6, bool pivoting, double C[7][7])
{
    int n = param6 ? 6 : 4;
    int64_t D[7][7] = { 0 };

    scale_mat(0, 3, param6, C);
    print_mat("DFA2", param6, C);

    for (int k = 0; k < n; k++) {
        if (pivoting) {
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
                print_mat('A', k, m, param6, C);

                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }

                print_mat('B', k, m, param6, C);
            }
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

        print_mat('C', k, param6, C);
    }

    print_res(0, param6, C);
}

int main(int argc, char **argv)
{
    double C[7][7] = {0};

    memcpy(C, data, sizeof(C));
    method_gja(false, true, C);

    memcpy(C, data, sizeof(C));
    method_dfa(false, true, C);

    memcpy(C, data, sizeof(C));
    method_dfa2(false, true, C);

    return 0;
}
