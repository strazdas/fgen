#include "gkls.h"
#include <cassert>
#include <iostream>
#include <stdlib.h>

using namespace std;


int main (int argc, char *argv[]) {
    // assert(GKLS_set_default()== GKLS_OK);     // Standartiniai nustatymai
    if (argc < 6) {
        cout << "Not enought arguments." << endl;
        cout << "  Specify: dimension, global_dist, global_radius, num_minima and function_id." << endl;
        return 1;
    }

    GKLS_dim = atoi(argv[1]);
    GKLS_global_dist = atof(argv[2]);
    GKLS_global_radius = atof(argv[3]);
    GKLS_num_minima = atoi(argv[4]);
    GKLS_global_value = GKLS_GLOBAL_MIN_VALUE;
    int function_id = atoi(argv[5]);
    assert(GKLS_domain_alloc() == GKLS_OK);

    assert(GKLS_parameters_check() == GKLS_OK);
    assert(GKLS_arg_generate(function_id) == GKLS_OK);

    int n = GKLS_glob.num_global_minima;
    assert(n == 1);
    int glob_idx = 1;
    assert(GKLS_minima.f[glob_idx] == GKLS_global_value);

    for (unsigned int i = 0; i < GKLS_dim; i++) {
        cout << GKLS_minima.local_min[glob_idx][i] << " ";
    }
    cout << endl;
    GKLS_free();
    GKLS_domain_free();
}
