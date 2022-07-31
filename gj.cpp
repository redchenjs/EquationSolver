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

 x1 = -3
 x2 = -2
 x3 =  1
 x4 =  1
*/

double data[7][7] = {
    [0] = { 2,  -3,   1,   5,   6,   0,   0},
    [1] = {-3,   1,   2,  -4,   5,   0,   0},
    [2] = {-1,  -2,   3,   1,  11,   0,   0},
    [3] = { 0,   0,   1,   1,   2,   0,   0},
    [4] = { 0,   0,   0,   0,   0,   0,   0},
    [5] = { 0,   0,   0,   0,   0,   0,   0},
    [6] = { 0,   0,   0,   0,   0,   0,   0},
};

void print_mat(double C[7][7])
{
    printf("======================================================\n");
    for (int p = 0; p < 7; p++) {
        for (int q = 0; q < 7; q++) {
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }
}

void print_mat(int i, int j, int k, double C[7][7])
{
    printf("====================================================== i = %d j = %d k = %d\n", i, j, k);
    for (int p = 0; p < 7; p++) {
        for (int q = 0; q < 7; q++) {
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
            print_mat(m, 0, k, C);
            // swap rows k and m
            if (m != k) {
                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }
                print_mat(m, 1, k, C);
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
        print_mat(D);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = D[i][j];
            }
        }
    }

    printf("C:");
    print_mat(C);
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
            print_mat(m, 0, k, C);
            // swap rows k and m
            if (m != k) {
                for (int j = 0; j < n + 1; j++) {
                    C[6][j] = C[k][j];
                    C[k][j] = C[m][j];
                    C[m][j] = C[6][j];
                }
                print_mat(m, 1, k, C);
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
        print_mat(D);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = D[i][j];
            }
        }
    }

    printf("C:");
    print_mat(C);
    print_res(param6, C);
}

int main(int argc, char **argv)
{
    // method_gja(false, true, data);
    method_dfa(false, true, data);

    return 0;
}
