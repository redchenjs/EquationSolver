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

const int N = 4;

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

double C[N][N + 1] = {
    [0] = { 2,  -3,   1,   5,   6},
    [1] = {-3,   1,   2,  -4,   5},
    [2] = {-1,  -2,   3,   1,  11},
    [3] = { 0,   0,   1,   1,   2},
};

double D[N][N + 1] = { 0 };

double E[7][7] = {
    [0] = { 0,   0,   0,   0,   0,   0,   0},
    [1] = { 2,  -3,   1,   5,   6,   0,   0},
    [2] = {-3,   1,   2,  -4,   5,   0,   0},
    [3] = {-1,  -2,   3,   1,  11,   0,   0},
    [4] = { 0,   0,   1,   1,   2,   0,   0},
    [5] = { 0,   0,   0,   0,   0,   0,   0},
    [6] = { 0,   0,   0,   0,   0,   0,   0},
};

double F[7] = { 0 };

void solveEqual(double dEqualCoeff[7][7], int order, double *dAffinePara)
{
    printf("========================================================= E initial\n");
    for (int p = 0; p < 7; p++) {
        for (int q = 0; q < 7; q++) {
            // dEqualCoeff[p][q] *= 256;
            printf("%6.1f\t", dEqualCoeff[p][q]);
        }
        printf("\n");
    }

    for (int k = 0; k < order; k++) {
        dAffinePara[k] = 0.;
    }

    // row echelon
    for (int i = 1; i < order; i++) {
        // find column max
        double temp = fabs(dEqualCoeff[i][i - 1]);
        int tempIdx = i;
        for (int j = i + 1; j < order + 1; j++) {
            if ( fabs(dEqualCoeff[j][i - 1]) > temp ) {
                temp = fabs(dEqualCoeff[j][i - 1]);
                tempIdx = j;
            }
        }

        // swap line
        if ( tempIdx != i ) {
            for (int j = 0; j < order + 1; j++) {
                dEqualCoeff[0][j] = dEqualCoeff[i][j];
                dEqualCoeff[i][j] = dEqualCoeff[tempIdx][j];
                dEqualCoeff[tempIdx][j] = dEqualCoeff[0][j];
            }
        }

        // elimination first column
        if ( dEqualCoeff[i][i - 1] == 0. ) {
            printf("========================================================= !!!\n");
            printf("return at (dEqualCoeff[%d][%d - 1] == 0.)\n", i, i);
            return;
        }
        for (int j = i + 1; j < order + 1; j++) {
            for (int k = i; k < order + 1; k++) {
                dEqualCoeff[j][k] = dEqualCoeff[j][k] - dEqualCoeff[i][k] * dEqualCoeff[j][i - 1] / dEqualCoeff[i][i - 1];
            }
        }

        printf("========================================================= E i%d\n", i);
        for (int p = 0; p < 7; p++) {
            for (int q = 0; q < 7; q++) {
                printf("%6.1f\t", dEqualCoeff[p][q]);
            }
            printf("\n");
        }
    }

    if (dEqualCoeff[order][order - 1] == 0.) {
        printf("========================================================= ???\n");
        printf("return at (dEqualCoeff[%d][%d - 1] == 0.)\n", order, order);
        return;
    }
    printf("========================================================= E final\n");
    for (int p = 0; p < 7; p++) {
        for (int q = 0; q < 7; q++) {
            printf("%6.1f\t", dEqualCoeff[p][q]);
        }
        printf("\n");
    }
    dAffinePara[order - 1] = dEqualCoeff[order][order] / dEqualCoeff[order][order - 1];
    for (int i = order - 2; i >= 0; i--) {
        if ( dEqualCoeff[i + 1][i] == 0. ) {
            for (int k = 0; k < order; k++) {
                dAffinePara[k] = 0.;
            }
            printf("========================================================= >>>\n");
            printf("return at (dEqualCoeff[%d + 1][%d] == 0.)\n", i, i);
            return;
        }
        double temp = 0;
        for (int j = i + 1; j < order; j++) {
            temp += dEqualCoeff[i + 1][j] * dAffinePara[j];
        }
        dAffinePara[i] = (dEqualCoeff[i + 1][order] - temp) / dEqualCoeff[i + 1][i];
    }

    printf("========================================================= results\n");
    for (int i = 0; i < order; i++) {
        printf("x%d = %6.1f\n", i + 1, dAffinePara[i]);
    }
}

void method_vtm(int n)
{
    solveEqual(E, n, F);
}

void method_1sdf(int n, double D[N][N + 1], double C[N][N + 1])
{
    printf("========================================================= C initial\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            // C[p][q] *= 256;
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }

    printf("========================================================= D initial\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            // D[p][q] *= 256;
            printf("%6.1f\t", D[p][q]);
        }
        printf("\n");
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {           // Row
            for (int j = 0; j < n + 1; j++) {   // Column
                if (k == i) {
                    D[i][j] = C[k][j];
                } else {
                    D[i][j] = (C[k][k] * C[i][j]) - (C[i][k] * C[k][j]);
                }
            }

            printf("========================================================= D k%d i%d\n", k, i);
            for (int p = 0; p < N; p++) {
                for (int q = 0; q < N + 1; q++) {
                    printf("%6.1f\t", D[p][q]);
                }
                printf("\n");
            }
        }

        for (int p = 0; p < n; p++) {           // matrix C
            for (int q = 0; q < n + 1; q++) {
                C[p][q] = D[p][q];
            }
        }
    }

    printf("========================================================= C final\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }

    printf("========================================================= D final\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            printf("%6.1f\t", D[p][q]);
        }
        printf("\n");
    }
}

void method_2sdf(int n, double D[N][N + 1], double C[N][N + 1])
{
    printf("========================================================= C initial\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            // C[p][q] *= 256;
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }

    printf("========================================================= D initial\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            // D[p][q] *= 256;
            printf("%6.1f\t", D[p][q]);
        }
        printf("\n");
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {           // Row
            for (int j = 0; j < n + 1; j++) {   // Column
                if (k == i) {
                    D[i][j] = C[k][j];
                } else {
                    D[i][j] = (C[k][k] * C[i][j]) - (C[i][k] * C[k][j]);
                }
            }

            printf("========================================================= D k%d i%d\n", k, i);
            for (int p = 0; p < N; p++) {
                for (int q = 0; q < N + 1; q++) {
                    printf("%6.1f\t", D[p][q]);
                }
                printf("\n");
            }
        }

        for (int p = 0; p < n; p++) {           // matrix C
            for (int q = 0; q < n + 1; q++) {
                C[p][q] = D[p][q];
            }
        }
    }

    printf("========================================================= C final\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            printf("%6.1f\t", C[p][q]);
        }
        printf("\n");
    }

    printf("========================================================= D final\n");
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n + 1; q++) {
            printf("%6.1f\t", D[p][q]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    // method_vtm(N);
    // method_1sdf(N, D, C);
    method_2sdf(N, D, C);

    return 0;
}
