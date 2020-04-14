#ifndef _globals_h_
#define _globals_h_

// Features amount
#define FEATURES 40

// Predefined user/item feature vector value
#define FEATURE_INIT 0.4

// Number of unique items and users
//#define ITEMS 10
//#define USERS 10
#define ITEMS 17770
#define USERS 480189

//#define ERR_THRESH 0.00000095367431640625
//#define ERR_THRESH_LOW -0.00000095367431640625
#define ERR_THRESH 0.00000001
#define ERR_THRESH_LOW -0.00000001

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

// Gradient descent constants
#define LRATE 0.001
#define K 0.02

#endif