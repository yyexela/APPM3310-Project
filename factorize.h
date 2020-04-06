#ifndef _factorize_h
#define _factorize_h

#include "globals.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/array.hpp>

using namespace std;

// Forward declarations
void FeatureInit();
double PredictRating(int uid, int item);
void Train();

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
    double user_f[USERS][FEATURES];
    double item_f[ITEMS][FEATURES];
};

#endif