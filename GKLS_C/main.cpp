#include "gkls.h"
#include <cassert>
#include <iostream>
#include <stdlib.h>

using namespace std;


int main (int argc, char *argv[]) {
    // assert(GKLS_set_default()== GKLS_OK);     // Standartiniai nustatymai
    if (argc < 7) {
        cout << "Not enought arguments." << endl;
        cout << "  Specify: dimension, global_dist, global_radius, num_minima and a point where to evaluate function." << endl;
        return 1;
    }

    GKLS_dim = atoi(argv[1]);
    GKLS_global_dist = atof(argv[2]);
    GKLS_global_radius = atof(argv[3]);
    GKLS_num_minima = atoi(argv[4]);
    GKLS_global_value = GKLS_GLOBAL_MIN_VALUE;
    assert(GKLS_domain_alloc() == GKLS_OK);

    double point[GKLS_dim];
    for (unsigned int i = 0; i < GKLS_dim; i++) {
        GKLS_domain_left[i] = -1;
        GKLS_domain_right[i] = 1;
        point[i] = atof(argv[i+5]);
    }

    assert(GKLS_parameters_check() == GKLS_OK);
    assert(GKLS_arg_generate(1) == GKLS_OK);
    cout << GKLS_D_func(point) << endl;
    GKLS_free();
    GKLS_domain_free();
}
