#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ctime>

using namespace std;

// Size of the sparse matrix: M x N
#define M 3 
#define N 3

// 2D Initial Array of size M x N
unsigned short arr[M][N];

// Forward declarations
void SparseLine(string line);
void PrintSparse();
void UIDMapLine(string line);

// Boolean used to print debug info
const bool DEBUG = true;

// Files to read from
//const string SPARSE_FILE = "InputCSV/ex_sparse.csv";
//const string UIDMAP_FILE = "InputCSV/ex_uid.csv";
const string SPARSE_FILE = "InputCSV/sparse_matrix_coords_and_values.csv";
const string UIDMAP_FILE = "InputCSV/userID_map.csv";

// Hashmap for userID map
unordered_map<int, int> uid_map;

int main(int argc, char* argv[]){
    string msg = "Running parse-csv.cc";
    cout << msg << endl;

    // Clock to check run-time
    double duration;
    clock_t start = std::clock();

    // ifs is used to read files
    ifstream ifs;

    // Open file, check if failed
    ifs.open(UIDMAP_FILE);
    if(!ifs.is_open())
        cout << "File \"" << UIDMAP_FILE << "\" failed to open" << endl;
    // Read file contents
    while(ifs.good()){
        string line;
        getline(ifs, line);
        // Ignore empty lines
        if(line.size() > 0)
            UIDMapLine(line);
    }

    // Close UIDMAP_FILE
    ifs.close();

    // Open file, check if failed
    ifs.open(SPARSE_FILE);
    if(!ifs.is_open())
        cout << "File \"" << SPARSE_FILE << "\" failed to open" << endl;
    // Read file contents
    while(ifs.good()){
        string line;
        getline(ifs, line);
        // Ignore empty lines
        if(line.size() > 0)
            SparseLine(line);
    }

    // Print parsed sparse matrix
    PrintSparse();

    // Close SPARSE_FILE
    ifs.close();

    // Check map contents
    cout << "Done processing" << endl;
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Time: " << duration << endl;
}

/* 
 * Reads a line and inserts the values into the UIDMap
 * Line must be formatted like this; X,Y
 * Where X is the original user iD, Y is the new user ID
 * Stores into the map like so: (key | value) = (X | Y)
 */
void UIDMapLine(string line){
    if(DEBUG) cout << "Parsing " << line << endl;

    // Position of comma
    size_t pos = line.find(',', 0);

    // Extracted values
    int uid_old, uid_new;

    // Temp string used for substr
    string temp;


    // First number: User
    temp = line.substr(0, pos);
    if(DEBUG) cout << "pos: " << pos << ", substr: \"" << temp << "\"" << endl;
    uid_old = stoi(temp);

    // Second number: Item
    temp = line.substr(pos + 1, line.size() - pos - 2);
    if(DEBUG) cout << "pos: " << pos << ", substr: \"" << temp << "\"" << endl;
    uid_new = stoi(temp);

    // Insert into hash map
    uid_map.insert(make_pair(uid_old,uid_new));
    if(DEBUG) cout << "key " << uid_old << " value " << uid_map[uid_old] << endl;
}

/* 
 * Reads a line and inserts the values into an array of size M x N
 * Line must be formatted like this; X,Y,Z
 * Where X is the user, Y is the item, and Z is the corresponding rating
 * Stores into the array specified like so: arr[X][Y] = Z
 */
void SparseLine(string line){
    // Position of current comma and the next comma
    size_t pos_c = 0, pos_n;

    // Extracted values
    int user, item, rating;

    // Temp string used for substr
    string temp;

    if(DEBUG) cout << "Parsing " << line << endl;
    pos_n = line.find(',', pos_c);

    // First number: User
    temp = line.substr(pos_c, pos_n - pos_c);
    if(DEBUG) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    user = stoi(temp);

    // Second number: Item
    pos_c = pos_n;
    pos_n = line.find(',', pos_c + 1);
    temp = line.substr(pos_c + 1, pos_n - pos_c - 1);
    if(DEBUG) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    item = stoi(temp);

    // Third number: Rating
    pos_c = pos_n;
    temp = line.substr(pos_n + 1, line.size() - pos_n - 2); // -2 needed to get rid of funky newline
    if(DEBUG) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl << endl;
    rating = stoi(temp);

    // Update array
    arr[uid_map.at(user) - 1][item - 1] = rating;
}

/*
 * Prints an array sized MxN, assuming
 * it is a user-item-rating matrix (outputs start at 1, not 0)
 */
void PrintSparse(){
    for(int i = 0; i < M; ++i){
        for(int j = 0; j < N; ++j){
            if(DEBUG) cout << "arr[" << i + 1 << "][" << j + 1 << "] = " << arr[i][j] << endl;
        }
    }
}
