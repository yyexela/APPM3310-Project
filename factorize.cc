#include <iostream>
#include "parse-csv.h"
#include "factorize.h"

using namespace std;

Parse parse_vars;
Factorize factorize_vars;

int main(int argc, char* argv[]){
    string msg = "Running parse-csv.cc";
    cout << msg << endl;
    ProcessFiles();
    if(!TestSample()){
        cout << "User-Item-Rating mismatch, exiting" << endl;
        exit(1);
    }
}