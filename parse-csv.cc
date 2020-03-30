#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ctime>

using namespace std;

// User amount
//#define USERS 480189
#define USERS 2649429

// Node for linked-list for the user-item matrix
struct cell{
    short rating;
    int uid;
    int item;
    cell *next;
};

// Forward declarations
void SparseLine(string line);
void UIDMapLine(string line);
void UpdateUser(int uid, int item, int rating);
void PrintLLArr();
void PrintCell(cell* cl);

// Boolean used to print debug info
const bool DEBUG = false;
const bool DEBUG_UID = false;

int item_counter = 0;

// Files to read from
//const string SPARSE_FILE = "InputCSV/ex_sparse.csv";
//const string UIDMAP_FILE = "InputCSV/ex_uid.csv";
const string SPARSE_FILE = "InputCSV/sparse_matrix_coords_and_values.csv";
const string UIDMAP_FILE = "InputCSV/userID_map.csv";

// Hashmap for userID map
unordered_map<int, int> uid_map;

// User array of linked-lists
cell* user[USERS];

int main(int argc, char* argv[]){
    string msg = "Running parse-csv.cc";
    cout << msg << endl;

    // Clock to check run-time
    double duration;
    clock_t start = std::clock();

    // ifs is used to read files
    ifstream ifs;

    cout << "Processing uid_map" << endl;

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

    cout << "Processed uid_map" << endl;
    cout << "Processing sparse_matrix file" << endl;

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

    // Close SPARSE_FILE
    ifs.close();

    cout << "Processed sparse_matrix file" << endl;

    PrintLLArr();

    cout << "Done processing" << endl;
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Time: " << duration << endl;
}

void PrintLLArr(){
    for(int i = 0; i < USERS; ++i){
        cell* tmp = user[i];
        while(tmp != NULL){
            PrintCell(tmp);
            tmp = tmp->next;
        }
    }
}

void PrintCell(cell* cl){
    cout << "user " << cl->uid << ", movie " << cl->item << ", rating " << cl->rating << endl;
}

void UpdateUser(int uid, int item, int rating){
    if(item - item_counter == 100){
        cout << item << endl;
        item_counter = item;
    }

    //create the corresponding person
    cell* new_user = new cell();
    new_user->rating = rating;
    new_user->item = item;
    new_user->next = NULL;
    new_user->uid = uid;

    if(DEBUG) cout << "Adding user " << uid << ", item " << new_user->item << ", rating " << new_user->rating << endl;

    //check if root is null
    if(user[uid] == NULL){
        //add user as head
        if(DEBUG) cout << "Adding user as head (null LL)" << endl;
        user[uid] = new_user;
    } else {
        //there exists a linkedlist, traverse until item is between 2 or last
        cell* tmp = user[uid];
        cell* tmp_prev = NULL;

        if(DEBUG){
            cout << "Initial tmp: ";
            PrintCell(tmp);
        }

        while(  tmp->next != NULL && 
                new_user->item > tmp->item){
            tmp_prev = tmp;
            tmp = tmp->next;
            if(DEBUG) cout << "Iterated tmp" << endl;
        }
        // tmp->next == NULL OR new_user->item < tmp->item
        if(new_user->item < tmp->item){
            //check if root was tmp
            if(tmp_prev == NULL){
                if(DEBUG) cout << "Adding user as head (before tmp)" << endl;
                //set new_user as head
                user[uid] = new_user;
                new_user->next = tmp;
            } else {
                //set new_user between tmp_prev and tmp
                if(DEBUG) cout << "Adding user before tmp and after tmp_prev" << endl;
                tmp_prev->next = new_user;
                new_user->next = tmp;
            }
        } else {
            //set new_user as tail
            if(DEBUG) cout << "Adding user at end of LL" << endl;
            tmp->next = new_user;
            if(DEBUG){
                cout << "tmp: ";
                PrintCell(tmp);
                cout << "tmp->next: ";
                PrintCell(tmp->next);
            }
        }
    }
    if(DEBUG) cout << endl;
}

/* 
 * Reads a line and inserts the values into the UIDMap
 * Line must be formatted like this; X,Y
 * Where X is the original user iD, Y is the new user ID
 * Stores into the map like so: (key | value) = (X | Y)
 */
void UIDMapLine(string line){
    if(DEBUG && DEBUG_UID) cout << "Parsing " << line << endl;

    // Position of comma
    size_t pos = line.find(',', 0);

    // Extracted values
    int uid_old, uid_new;

    // Temp string used for substr
    string temp;


    // First number: User
    temp = line.substr(0, pos);
    if(DEBUG && DEBUG_UID) cout << "pos: " << pos << ", substr: \"" << temp << "\"" << endl;
    uid_old = stoi(temp);

    // Second number: Item
    temp = line.substr(pos + 1, line.size() - pos - 2);
    if(DEBUG && DEBUG_UID) cout << "pos: " << pos << ", substr: \"" << temp << "\"" << endl;
    uid_new = stoi(temp);

    // Insert into hash map
    uid_map.insert(make_pair(uid_old,uid_new));
    if(DEBUG && DEBUG_UID) cout << "key " << uid_old << " value " << uid_map[uid_old] << endl << endl;
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
    int uid, item, rating;

    // Temp string used for substr
    string temp;

    if(DEBUG) cout << "Parsing " << line << endl;
    pos_n = line.find(',', pos_c);

    // First number: uid
    temp = line.substr(pos_c, pos_n - pos_c);
    if(DEBUG) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    uid = stoi(temp);

    // Second number: Item
    pos_c = pos_n;
    pos_n = line.find(',', pos_c + 1);
    temp = line.substr(pos_c + 1, pos_n - pos_c - 1);
    if(DEBUG) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    item = stoi(temp);

    // Third number: Rating
    pos_c = pos_n;
    temp = line.substr(pos_n + 1, line.size() - pos_n - 2); // -2 needed to get rid of funky newline
    if(DEBUG) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    rating = stoi(temp);

    if(DEBUG) cout << "Parsed into: " << uid << "," << item << "," << rating << endl;
    //if(DEBUG) cout << "uid " << uid << "->" << uid_map.at(uid) << endl;
    //uid = uid_map.at(uid);

    // Update array
    UpdateUser(uid, item, rating);
}
