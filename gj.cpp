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
 1   1   1   1
 0   1   2   2
 0  -1  -4  -2
 3   2   1  -1

 x0 = -2
 x1 = -3
 x2 = -1
 x3 =  0
*/

double data[7][7] = {
    [0] = { 1,   1,   1,   1,   0,   0,   0},
    [1] = { 0,   1,   2,   2,   1,   0,   0},
    [2] = { 0,  -1,  -4,  -2,   1,   0,   0},
    [3] = { 3,   2,   1,  -1,  -1,   0,   0},
    [4] = { 0,   0,   0,   0,   0,   0,   0},
    [5] = { 0,   0,   0,   0,   0,   0,   0},
    [6] = { 0,   0,   0,   0,   0,   0,   0},
};

void print_mat(bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    printf("======================================================\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }
}

void print_mat(int i, int j, int k, bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    printf("====================================================== i = %d j = %d k = %d\n", i, j, k);
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }
}

void print_res(bool param6, double C[7][7])
{
    int n = param6 ? 6 : 4;

    printf("======================================================\n");
    for (int i = 0; i < n; i++) {
        printf("x%d = %6.1f\n", i, C[i][n] / C[i][i]);
    }
}

void method_gja(bool param6, bool pivoting, double C[7][7])
{
    int n = param6 ? 6 : 4;
    double D[7][7] = { 0 };

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
            print_mat(m, 0, k, param6, C);
            // swap rows k and m
            if (m != k) {
                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }
                print_mat(m, 1, k, param6, C);
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

        printf("D:");
        print_mat(param6, D);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = D[i][j];
            }
        }
    }

    printf("C:");
    print_mat(param6, C);
    print_res(param6, C);
}

void method_dfa(bool param6, bool pivoting, double C[7][7])
{
    int n = param6 ? 6 : 4;
    double D[7][7] = { 0 };

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
            print_mat(m, 0, k, param6, C);
            // swap rows k and m
            if (m != k) {
                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }
                print_mat(m, 1, k, param6, C);
            }
        }

        double M = C[k][k];
        for (int i = 0; i < n; i++) {
            double L = C[i][k];

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

        printf("D:");
        print_mat(param6, D);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = D[i][j];
            }
        }
    }

    printf("C:");
    print_mat(param6, C);
    print_res(param6, C);
}

int main(int argc, char **argv)
{
    method_gja(false, true, data);
    // method_dfa(false, true, data);

    return 0;
}
