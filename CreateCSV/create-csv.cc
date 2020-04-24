#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "create-csv.h"
#include "factorize.h"

using namespace std;

Factorize factorize_vars;

int main(int argc, char * argv[]){
    cout << "Running create-csv" << endl;

    cout << "Sample output of 1.234567890123456789: " << doubleToText(1.234567890123456789) << endl;

    cout << "Loading \"serialized\"" << endl;
    {
        // create and open an archive for input
        std::ifstream ifs("serialized");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> factorize_vars;
        // archive and stream closed when destructors are called
    }

    cout << "Saving to file \"user_feature.csv\"" << endl;

    ofstream ifs("user_feature.csv", ofstream::out | ofstream::trunc);
    if(!ifs.is_open()){
        cout << "File couldn't open, exiting" << endl;
        return 1;
    }
    for(int i = 0; i < USERS; i++){
        for(int j = 0; j < FEATURES - 1; j++){
            ifs << doubleToText(factorize_vars.user_f[i][j]) << ",";
        }
        // add last feature to not have a trailing comma
        ifs << doubleToText(factorize_vars.user_f[i][FEATURES-1]) << endl;
    }
    ifs.close();

    cout << "Saving to file \"item_feature.csv\"" << endl;

    ifs.open("item_feature.csv", ofstream::out | ofstream::trunc);
    if(!ifs.is_open()){
        cout << "File couldn't open, exiting" << endl;
        return 1;
    }
    for(int i = 0; i < ITEMS; i++){
        for(int j = 0; j < FEATURES - 1; j++){
            ifs << doubleToText(factorize_vars.item_f[i][j]) << ",";
        }
        // add last feature to not have a trailing comma
        ifs << doubleToText(factorize_vars.item_f[i][FEATURES-1]) << endl;
    }
    ifs.close();
}

string doubleToText(const double & d){
    std::stringstream ss;
    ss << std::setprecision( std::numeric_limits<double>::digits10+2);
    //ss << std::setprecision( std::numeric_limits<int>::max() );
    ss << d;
    return ss.str();
}