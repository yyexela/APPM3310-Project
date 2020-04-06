#include <iostream>
#include <fstream>
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
    
    // Initialize user/item feature vectors
    cout << "Initializing user/item feature vectors" << endl;
    FeatureInit();
    PrintTimestamp();
    cout << "" << endl;

    // Stochastic Gradient Descent
    cout << "Training the data" << endl;
    Train();
    cout << "Done training the data" << endl;
    PrintTimestamp();
    cout << "" << endl;


    cout << "Serializing the data to \"serialized\"" << endl;
    PrintTimestamp();
    cout << "" << endl;

    // Serialize the data so it isn't lost
    {
            ofstream ofs("serialized");
            boost::archive::text_oarchive oa(ofs);
            oa << factorize_vars;
    }

    cout << "Program finished, exiting" << endl;
    PrintTimestamp();
}

/*
 * Loops through each existing rating and works
 * on updating a feature before moving onto the next
 */
void Train(){
    double err, old_err, u_old;
    int item, uid, rating;
    int iters;

    for(int i = 0; i < ITEMS; ++i){
        cell* tmp = parse_vars.items[i];
        while(tmp != NULL){
            item = tmp->item;
            uid = tmp->uid;
            rating = tmp->rating;

            err = 1;
            old_err = -1;

            while(err != old_err){
                old_err = err;
                err = rating - PredictRating(uid, item);
                for(int ftr = 0; ftr < FEATURES; ++ftr){
                    u_old = factorize_vars.user_f[uid-1][ftr];
                    factorize_vars.user_f[uid-1][ftr] += lrate * (err * factorize_vars.item_f[item-1][ftr] - K * u_old);
                    factorize_vars.item_f[item-1][ftr] += lrate * (err * u_old - K * factorize_vars.item_f[item-1][ftr]);

                }
                iters++;
            }
            tmp = tmp->next;
        }
        cout << "item " << i+1 << endl;
        PrintTimestamp();
        cout << " " << endl;

        factorize_vars.item_num = i+1;

        // Serialize the data so it isn't lost every 1000 items
        if( (i+1) % 1000 == 0 ){
            {
                    ofstream ofs("serialized");
                    boost::archive::text_oarchive oa(ofs);
                    oa << factorize_vars;
            }
        }
    }
}

/*
 * Takes the euclidean dot product of a pair
 * of given user-feature and item-feature
 * vectors
 */
double PredictRating(int uid, int item){
    double sum1 = 0;
    for(int i = 0; i < FEATURES; i++){
        sum1 = sum1 + (factorize_vars.item_f[item-1][i] * factorize_vars.user_f[uid-1][i]);
    }
    return (sum1);
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