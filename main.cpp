/*
 * main.cpp
 *
 *  Created on: 2022-07-30 18:20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "EquationSolver.h"

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
 0   1   2   2   1
 1   1   1   1   0
 0  -1  -4  -2   1
 3   2   1  -1  -1

 x0 = -2
 x1 = -3
 x2 = -1
 x3 =  0
*/

static const int64_t C[7][7] = {
    [0] = { 0,   0,   0,   0,   0,   0,   0},
    [1] = { 0,   1,   2,   2,   1,   0,   0},
    [2] = { 1,   1,   1,   1,   0,   0,   0},
    [3] = { 0,  -1,  -4,  -2,   1,   0,   0},
    [4] = { 3,   2,   1,  -1,  -1,   0,   0},
    [5] = { 0,   0,   0,   0,   0,   0,   0},
    [6] = { 0,   0,   0,   0,   0,   0,   0},
};

static double D[6] = { 0.0 };

int main(int argc, char **argv)
{
    EquationSolver *solver = new EquationSolver();

    solver->set_debug(true);

    solver->load_data(C, 4, 0);
    solver->method_gja(4);
    solver->save_data(D, 4, 0);

    solver->load_data(C, 4, 4);
    solver->method_gja2(4, 4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 8);
    solver->method_gja2(4, 8, 8);
    solver->save_data(D, 4, 8);

    solver->load_data(C, 4, 0);
    solver->method_dfa(4, 0);
    solver->save_data(D, 4, 0);

#ifdef __GNUC__
    solver->load_data(C, 4, 0);
    solver->method_dfa128(4, 0);
    solver->save_data(D, 4, 0);
#endif

    solver->load_data(C, 4, 4);
    solver->method_dfa2(4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 4);
    solver->method_dfa2s(4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 4);
    solver->method_dfa3(4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 8);
    solver->method_dfa3(4, 8);
    solver->save_data(D, 4, 8);

    solver->load_data(C, 4, 4);
    solver->method_dfa3s(4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 4);
    solver->method_dfa4a(4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 4);
    solver->method_dfa4b(4, 4);
    solver->save_data(D, 4, 4);

    solver->load_data(C, 4, 0);
    solver->method_cra(4, 0);
    solver->save_data(D, 4, 0);

    delete(solver);

    return 0;
}
