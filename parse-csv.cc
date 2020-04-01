#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ctime>

using namespace std;

// User amount
#define USERS 480189
//#define USERS 2649429
//#define USERS 1

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
unsigned int LLArrSize();
void PrintTimestamp();
int GetRating(int uid, int item);
void ProcessFiles();

// Boolean used to print debug info
const bool DEBUG = false;
const bool DEBUG_UID = false;
const bool DEBUG_SPLINE = false;
const bool DEBUG_UPDATE_USER = false;

// Files to read from
const string SPARSE_FILE = "InputCSV/sparse_matrix_coords_and_values.csv";
const string UIDMAP_FILE = "InputCSV/userID_map.csv";

// Hashmap for userID map
unordered_map<int, int> old2newuid_map;
unordered_map<int, int> new2olduid_map;

// Clock to check run-time
clock_t start = std::clock();

// User array of linked-lists
cell* user[USERS];

int main(int argc, char* argv[]){
    string msg = "Running parse-csv.cc";
    cout << msg << endl;

    ProcessFiles();

    cout << "Sample ratings: " << endl;
    cout << "user 22172 movie 1: " << GetRating(22172, 1) << endl; // should be 1
    cout << "user 473001 movie 17770: " << GetRating(473001, 17770) << endl; //should be 1
    cout << "user 205182 movie 10042: " << GetRating(205182, 10042) << endl; // should be 3
}

/*
 * Processes the two CSV files SPARSE_FILE
 * and UIDMAP_FILE and places it into
 * the user[] array of linked-lists
 */
void ProcessFiles(){
    // ifs is used to read files
    ifstream ifs;
    // string used to store lines
    string line;

    cout << "Processing uid maps" << endl;

    // Open file, check if failed
    ifs.open(UIDMAP_FILE);
    if(!ifs.is_open()){
        cout << "File \"" << UIDMAP_FILE << "\" failed to open" << endl;
        exit(1);
    }
    // Read file contents
    while(ifs.good()){
        getline(ifs, line);
        // Ignore empty lines
        if(line.size() > 0)
            UIDMapLine(line);
    }

    // Close UIDMAP_FILE
    ifs.close();

    cout << "Processed uid maps, inserted " << old2newuid_map.size() << " elements" << endl;
    PrintTimestamp();
    cout << "Processing sparse_matrix file" << endl;

    // Counts the number of lines processed
    unsigned int lines = 0;

    // Open file, check if failed
    ifs.open(SPARSE_FILE);
    if(!ifs.is_open()){
        cout << "File \"" << SPARSE_FILE << "\" failed to open" << endl;
        exit(1);
    }
    // Read file contents
    while(ifs.good()){
        getline(ifs, line);
        // Ignore empty lines
        if(line.size() > 0){
            ++lines;
            SparseLine(line);
        }
    }

    // Close SPARSE_FILE
    ifs.close();

    // Print statistics
    cout << "Processed sparse_matrix file, " << lines << " lines" << endl;
    PrintTimestamp();

    unsigned int arr_size = LLArrSize();
    if(arr_size != lines){
        cout << "Error: mismatched lines between final users[] array and SPARSE_FILE" << endl;
        cout << "    arr_size: " << arr_size << ", lines: " << lines << endl;
    }

}

/*
 * Outputs the difference in current time and the time the function was called
 */
void PrintTimestamp(){
    double duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Time: " << duration << endl;
}

/* 
 * Returns the rating given a compact UID
 * and a corresponding item
 * 
 * Returns -1 if the rating is not found
 */
int GetRating(int uid, int item){
    // Check if it exists in the array
    if(uid > USERS || uid < 1) return -1;
    
    // Loop through the array of linked-lists
    cell* tmp = user[uid-1];
    while(tmp != NULL && item <= tmp->item){
        if(tmp->item == item) return tmp->rating;
        tmp = tmp->next;
    }
    // Reached end of user's list
    return -1;
}

/* 
 * Outputs the number of indifidual cells in the
 * Linked-List array
 */
unsigned int LLArrSize(){
    unsigned int size = 0;
    for(int i = 0; i < USERS; ++i){
        cell* tmp = user[i];
        while(tmp != NULL){
            ++size;
            tmp = tmp->next;
        }
    }
    return size;
}

/* 
 * Prints each cell in the, with padding
 * Linked-List array
 */
void PrintLLArr(){
    for(int i = 0; i < USERS; ++i){
        cell* tmp = user[i];
        cout << endl;
        while(tmp != NULL){
            PrintCell(tmp);
            tmp = tmp->next;
        }
        cout << endl;
    }
}

/*
 * Prints a given cell's values
 */
void PrintCell(cell* cl){
    cout << "user " << cl->uid << ", movie " << cl->item << ", rating " << cl->rating << endl;
}

/*
 * Adds a cell to the user[] array and
 * corresponding linked-list
 */
void UpdateUser(int uid, int item, int rating){
    //create the corresponding person
    cell* new_user = new cell();
    new_user->uid = uid;
    new_user->item = item;
    new_user->rating = rating;
    new_user->next = NULL;

    if(DEBUG && DEBUG_UPDATE_USER) cout << "Adding user " << uid << ", item " << new_user->item << ", rating " << new_user->rating << endl;
    if(DEBUG && DEBUG_UPDATE_USER){
        PrintLLArr();
    }

    //check if root is null
    if(user[uid-1] == NULL){
        //add user as head
        if(DEBUG && DEBUG_UPDATE_USER) cout << "Adding user as head (null LL)" << endl;
        user[uid-1] = new_user;
    } else {
        //there exists a linkedlist, traverse until item is between 2 or last
        cell* tmp = user[uid-1];
        cell* tmp_prev = NULL;

        if(DEBUG && DEBUG_UPDATE_USER){
            cout << "Initial tmp: ";
            PrintCell(tmp);
        }

        //iterate until tmp is tail OR tmp->item >= new_user->item
        while(  tmp->next != NULL && 
                new_user->item < tmp->item){
            tmp_prev = tmp;
            tmp = tmp->next;
            if(DEBUG && DEBUG_UPDATE_USER) cout << "Iterated tmp" << endl;
        }

        if(new_user->item > tmp->item){
            //check if root was tmp
            if(tmp_prev == NULL){
                if(DEBUG && DEBUG_UPDATE_USER) cout << "Adding user as head (before tmp)" << endl;
                //set new_user as head
                user[uid-1] = new_user;
                new_user->next = tmp;
            } else {
                //set new_user after tmp
                if(DEBUG && DEBUG_UPDATE_USER) cout << "Adding user before tmp, after tmp_prev" << endl;
                new_user->next = tmp_prev->next;
                tmp_prev->next = new_user;
            }
        } else {
            // new_user->item <= tmp->item
            // accounts for it tmp is the tail of the linked list
            if(DEBUG && DEBUG_UPDATE_USER) cout << "Adding user after tmp" << endl;
            new_user->next = tmp->next;
            tmp->next = new_user;
        }
    }

    if(DEBUG && DEBUG_UPDATE_USER){
        PrintLLArr();
    }
}

/* 
 * Reads a line and inserts the values into the UIDMap
 * Line must be formatted like this; X,Y
 * Where X is the original user ID, Y is the new user ID
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


    // First number: original UID 
    temp = line.substr(0, pos);
    if(DEBUG && DEBUG_UID) cout << "pos: " << pos << ", substr: \"" << temp << "\"" << endl;
    uid_old = stoi(temp);

    // Second number: updated UID
    temp = line.substr(pos + 1, line.size() - pos - 2);
    if(DEBUG && DEBUG_UID) cout << "pos: " << pos << ", substr: \"" << temp << "\"" << endl;
    uid_new = stoi(temp);

    // Insert into hash map
    old2newuid_map.insert(make_pair(uid_old,uid_new));
    new2olduid_map.insert(make_pair(uid_new,uid_old));
    if(DEBUG && DEBUG_UID) cout << "old uid " << uid_old << " new uid " << uid_new << endl << endl;
}

/* 
 * Reads a line and inserts the values into an array of size M x N
 * Line must be formatted like this; X,Y,Z
 * Where X is the item, Y is the user ID, and Z is the corresponding rating
 * Calls UpdateUser to update the user[] array of linked-lists with the newly parsed values
 */
void SparseLine(string line){
    // Position of current comma and the next comma
    size_t pos_c = 0, pos_n;

    // Extracted values
    int uid, item, rating;

    // Temp string used for substr
    string temp;

    if(DEBUG && DEBUG_SPLINE) cout << "Parsing " << line << endl;
    pos_n = line.find(',', pos_c);

    // First number: movie ID 
    temp = line.substr(pos_c, pos_n - pos_c);
    if(DEBUG && DEBUG_SPLINE) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    item = stoi(temp);

    // Second number: user ID
    pos_c = pos_n;
    pos_n = line.find(',', pos_c + 1);
    temp = line.substr(pos_c + 1, pos_n - pos_c - 1);
    if(DEBUG && DEBUG_SPLINE) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    uid = stoi(temp);

    // Third number: rating
    pos_c = pos_n;
    temp = line.substr(pos_n + 1, line.size() - pos_n - 2); // -2 needed to get rid of funky newline
    if(DEBUG && DEBUG_SPLINE) cout << "pos_c: " << pos_c << ", pos_n: "<< pos_n << ", substr: \"" << temp << "\"" << endl;
    rating = stoi(temp);

    if(DEBUG && DEBUG_SPLINE) cout << "Parsed into: " << uid << "," << item << "," << rating << endl;

    if(!(uid > 0 && item > 0 && rating > 0)) cout << "Error on line, invalid uid/item/rating combo " << line << endl;

    // Update array
    UpdateUser(uid, item, rating);
}
