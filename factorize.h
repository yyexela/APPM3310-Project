#ifndef _factorize_h
#define _factorize_h

#include "globals.h"

using namespace std;

// Forward declarations

// Booleans used to print debug info

class Factorize{
    public:
    // user_f and item_f are user/item matrices with FEATURES
    // number of features for each user/item
    int user_f[USERS][FEATURES];
    int item_f[ITEMS][FEATURES];
};

#endif