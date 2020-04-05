#include <iostream>
#include "parse-csv.h"
#include "factorize.h"

using namespace std;

Parse parse_vars;
Factorize factorize_vars;


const double lrate = 0.001;
const double K = 0.02;

int main(int argc, char* argv[]){
    // Load in the files
    ProcessFiles();
    
    // Stochastic Gradient Descent
    // Initialize user/item feature vectors
    cout << "Initializing user/item feature vectors" << endl;
    FeatureInit();
    cout << "Vectors Initialized" << endl;
    PrintTimestamp();

    cout << "Training round " << 1 << endl;
    Train();
    cout << "Done" << endl;
    PrintTimestamp();
    cout << "Training round " << 2 << endl;
    Train();
    cout << "Done" << endl;
    PrintTimestamp();
    for(int i = 3; i < 1000; ++i){
        cout << "Training round " << i << endl;
        Train();
        cout << "Done" << endl;
        PrintTimestamp();
    }
}

/*
 * Loops through each existing rating and works
 * on updating a feature before moving onto the next
 */
void Train(){
    double err, u_old;

    for(int i = 0; i < ITEMS; ++i){
        cell* tmp = parse_vars.items[i];
        while(tmp != NULL){
            int item = tmp->item;
            int uid = tmp->uid;
            int rating = tmp->rating;

            for(int ftr = 0; ftr < FEATURES; ++ftr){
                err = rating - PredictRating(uid, item);
                u_old = factorize_vars.user_f[uid-1][ftr];
                factorize_vars.user_f[uid-1][ftr] += lrate * (err * factorize_vars.item_f[item-1][ftr] - K * u_old);
                factorize_vars.item_f[item-1][ftr] += lrate * (err * u_old - K * factorize_vars.item_f[item-1][ftr]);
            }

            tmp = tmp->next;
        }
    }
}

/*
 * Takes the euclidean dot product of a pair
 * of given user-feature and item-feature
 * vectors
 */
double PredictRating(int uid, int item){
    double sum = 0;
    for(int i = 0; i < FEATURES; ++i){
        sum += factorize_vars.item_f[item-1][i] * factorize_vars.user_f[uid-1][i];
    }
    return sum;
}

/*
 * Initialize user/item feature matrices
 * to defined global value in globals.h
 * as FEATURE_INIT
 */
void FeatureInit(){
    // Initialize user-feature
    for(int i = 0; i < USERS; ++i){
        for(int j = 0; j < FEATURES; ++j){
            factorize_vars.user_f[i][j] = FEATURE_INIT;
        }
    }

    // Initialize item-feature
    for(int i = 0; i < ITEMS; ++i){
        for(int j = 0; j < FEATURES; ++j){
            factorize_vars.item_f[i][j] = FEATURE_INIT;
        }
    }
}