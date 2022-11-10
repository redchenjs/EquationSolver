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
 x1 =  3
 x2 = -1
 x3 =  0
*/

/*
  22011332        387232848         5219810        121208776        62134656
 387232848      13023152016       507249608      -4173773200      1455695872
   5219810        507249608        48421896       -575676040         7083392
 121208776      -4173773200      -575676040      12016008592      2715599360

 x0 = -10.7
 x1 =   0.5
 x2 =   5.5
 x3 =   0.8
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

/*
static const int64_t C[7][7] = {
    [0] = { 0,   0,   0,   0,   0,   0,   0},
    [1] = {  22011332,   387232848,   5219810,    121208776,   62134656,   0,   0},
    [2] = { 387232848, 13023152016,  507249608, -4173773200, 1455695872,   0,   0},
    [3] = {   5219810,   507249608,   48421896,  -575676040,    7083392,   0,   0},
    [4] = { 121208776, -4173773200, -575676040, 12016008592, 2715599360,   0,   0},
    [5] = { 0,   0,   0,   0,   0,   0,   0},
    [6] = { 0,   0,   0,   0,   0,   0,   0},
};
*/

static double D[6] = { 0.0 };

int main(int argc, char **argv)
{
    EquationSolver *solver = new EquationSolver();

    solver->set_debug(true);

    solver->load_data(C, 4);
    solver->method_gja(4);
    solver->save_data(D, 4);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_gja2(4, 4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa2(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa2s(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa3(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa3s(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa4(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa4s(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa5(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    solver->load_data(C, 4);
    solver->method_dfa5s(4);
    solver->save_data(D, 4, 8);
    solver->print_data(D, 4);

    delete(solver);

    return 0;
}
