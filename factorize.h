#ifndef _factorize_h
#define _factorize_h

#include "globals.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

// Forward declarations
void FeatureInit();
double PredictRating(int uid, int item);
void Train();
void UpdateResErr(short n);
void ResErrInit();
unsigned int ResErrSize();

// Factorize class used to hold the smaller 2 matrices,
// contains serialization functionality
class Factorize{
    private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & user_f;
        ar & item_f;
    }
    public:
    // user_f and item_f are user/item matrices with FEATURES
    // number of features for each user/item
    // Ordered FEATURES then USERS/ITEMS for better locality
    double user_f[USERS][FEATURES];
    double item_f[ITEMS][FEATURES];

    // array of vectors containing residual error
    // SHOULD BE SAME SIZE OF items_v DURING RUNTIME
    vector <double> res_err[ITEMS];
};

#endif