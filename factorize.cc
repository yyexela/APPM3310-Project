#include <iostream>
#include <fstream>
#include "parse-csv.h"
#include "factorize.h"
#include "float.h"
#include <cmath>

using namespace std;

Parse parse_vars;
Factorize factorize_vars;


const double lrate = 0.001;
const double K = 0.02;

int main(int argc, char* argv[]){
    ofstream wfs("time.log", ofstream::out | ofstream::trunc);
    wfs << "Start time: " << clock() / (double) CLOCKS_PER_SEC << endl;
    wfs.close();

    // Load in the files
    ProcessFiles();
    
    // Initialize user/item feature vectors
    cout << "Initializing user/item feature vectors" << endl;
    FeatureInit();
    PrintTimestamp();
    cout << endl;

    cout << "Initializing res_err array of vectors to riu" << endl;
    ResErrInit();
    if(ResErrSize() != VArrSize()){
        cout << "ERROR: Mismatched VArrSize and ResArrSize" << endl;
        exit(1);
    }

    PrintTimestamp();
    cout << endl;

    /*
    cout << "Testing serialization w/ initialized values" << endl;
    {
        ofstream ofs("serialized");
        boost::archive::text_oarchive oa(ofs);
        oa << factorize_vars;
    }
    PrintTimestamp();
    cout << endl;
    */

    // Stochastic Gradient Descent
    cout << "Training the data" << endl << endl;
    Train();
    cout << "Done training the data" << endl;
    PrintTimestamp();
    cout << endl;

    cout << "Serializing the data to \"serialized\"" << endl;
    PrintTimestamp();
    cout << endl;

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
    //We need to initialize total_err and old_err
    //to two different values
    double total_err;
    double old_err;

    //Used to accurately do gradient descent since we update user_f and
    //then do item_f, but we need to use the old user_f
    double u_old;

    //Train one feature at a time
    for(int n = 0; n < FEATURES; n++){
        total_err = DBL_MAX/1.125;
        old_err = DBL_MAX;
        cout << "Feature " << n << endl;
        // Loop until total_err is under ERR_THRESH (error threshold global var)
        // while(!(-ERR_THRESH < old_err - total_err < ERR_THRESH))
        // and while old_err > total_err
        do{
            old_err = total_err;
            total_err = 0.0;
            // 1-step gradient descent
            // loop through all existing riu once
            cout << "One Step Gradient Descent" << endl;
            for(int i = 0; i < ITEMS; i++){
                for(unsigned int j = 0; j < parse_vars.items_v[i].size(); j++){
                    unsigned int item = parse_vars.items_v[i].at(j).item;
                    unsigned int uid = parse_vars.items_v[i].at(j).uid;
                    double err = factorize_vars.res_err[i].at(j) - 
                        (factorize_vars.item_f[item-1][n] * factorize_vars.user_f[uid-1][n]);
                    total_err += (pow(err,2) + K * (factorize_vars.mag_item[item-1] + factorize_vars.mag_user[uid-1]));
                    
                    u_old = factorize_vars.user_f[uid-1][n];
                    factorize_vars.user_f[uid-1][n] += lrate * (err * factorize_vars.item_f[item-1][n] - K * u_old);
                    factorize_vars.item_f[item-1][n] += lrate * (err * u_old - K * factorize_vars.item_f[item-1][n]);
                }
            }
            cout << "Total Error: " << total_err << endl;
            cout << "Old Error: " << old_err << endl;
            cout << "Difference: " << old_err - total_err << endl;
            PrintTimestamp();
            cout << endl;
        }while( !((old_err - total_err) < ERR_THRESH && (old_err - total_err) > (ERR_THRESH_LOW)) 
            && old_err > total_err);
        cout << "Trained feature " << n << endl;
        UpdateResErr(n);
        UpdateMags(n);

        ofstream wfs("time.log", ofstream::out | ofstream::app);
        wfs << "Trained feature " << (n+1) << ". Elapsed time: " << (clock() - parse_vars.start) / (double) CLOCKS_PER_SEC << endl;
        wfs << "Total error: " << total_err << endl;
        wfs << endl;
        wfs.close();

        PrintTimestamp();
        cout << endl;

        cout << "Saving item_f and user_f" << endl;
        {
            ofstream ofs("serialized");
            boost::archive::text_oarchive oa(ofs);
            oa << factorize_vars;
        }
        PrintTimestamp();
        cout << endl;
    }
}

/*
 * Updates the magnitude squared of each
 * user/item vector for feature n
 */
void UpdateMags(int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < ITEMS; j++){
            factorize_vars.mag_item[j] += pow(factorize_vars.item_f[j][n],2);
        }
        for(int j = 0; j < USERS; j++){
            factorize_vars.mag_user[j] += pow(factorize_vars.user_f[j][n],2);
        }
    }
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

/*
 * Update the res_err vector with the n-th
 * feature is put into each user/item-pair's residual error
 */
void UpdateResErr(short n){
    for(int i = 0; i < ITEMS; i++){
        for(unsigned int j = 0; j < factorize_vars.res_err[i].size(); j++){
            unsigned int item = parse_vars.items_v[i].at(j).item;
            unsigned int uid = parse_vars.items_v[i].at(j).uid;
            double residual = (factorize_vars.item_f[item-1][n] * factorize_vars.user_f[uid-1][n]);
            double old_rating = factorize_vars.res_err[i].at(j);
            double new_rating = old_rating - residual;
            factorize_vars.res_err[i].at(j) = new_rating;
        }
    }
}

/*
 * Sets res_err to riu for each item/user
 * Same functionality as UpdateResErr, but instead of
 * updating it initializes the array of vectors
 */
void ResErrInit(){
    for(int i = 0; i < ITEMS; i++){
        for(unsigned int j = 0; j < parse_vars.items_v[i].size(); j++){
            factorize_vars.res_err[i].push_back(parse_vars.items_v[i].at(j).rating);
        }
    }
}

/*
 * Used to check to make sure the size
 * of res_err is correct
 */ 
unsigned int ResErrSize(){
    unsigned int size = 0;
    for(int i = 0; i < ITEMS; i++){
        size += factorize_vars.res_err[i].size();
    }
    return size;
}