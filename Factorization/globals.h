#ifndef _globals_h_
#define _globals_h_

// Features amount
#define FEATURES 40

// Predefined user/item feature vector value
#define FEATURE_INIT 0.1

// Number of unique items and users
// #define ITEMS 10
// #define USERS 10
#define ITEMS 17770
#define USERS 480189

//#define ERR_THRESH 0.00000095367431640625
//#define ERR_THRESH_LOW -0.00000095367431640625
#define ERR_THRESH 0.00001
#define ERR_THRESH_LOW -0.00001

//Number of times to run through a single feature for training
#define EPOCHS 120

// Booleans used to print debug info
#define DEBUG false
#define DEBUG_UID false
#define DEBUG_SPLINE false
#define DEBUG_UPDATE_VARR false
#define DISABLE_CHECKS false
#define PROCESS_UID false
#define PRINT_VECARR false
#define DISABLE_AVGS true
#define PREDICT_FINAL false
#define PRINT_STEP true

// Gradient descent constants
#define LRATE 0.001
#define K 0.02

// Load/Save serialized
#define LOAD_SERIALIZED false

// Number of features completed
#define FEAT_DONE 0

#endif
