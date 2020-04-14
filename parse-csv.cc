#include <iostream>
#include <fstream>
#include "parse-csv.h"

using namespace std;

extern Parse parse_vars;

/*
 * Processes the two CSV files SPARSE_FILE
 * and UIDMAP_FILE and places it into
 * the user[] array of linked-lists
 */
void ProcessFiles(){
    string msg = "Running parse-csv.cc";
    cout << msg << endl;
    cout << endl;

    // ifs is used to read files
    ifstream ifs;
    // string used to store lines
    string line;

    if(PROCESS_UID){
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

        cout << "Processed uid maps, inserted " << parse_vars.old2newuid_map.size() << " elements" << endl;
        PrintTimestamp();
        cout << endl;
    }

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

    if(!DISABLE_CHECKS){
        cout << "" << endl;
        cout << "Running tests to ensure file transfer was correct" << endl;

        unsigned int arr_size = VArrSize();
        if(arr_size != lines){
            cout << "Error: mismatched lines between final items_v[] array and SPARSE_FILE" << endl;
            cout << "    arr_size: " << arr_size << ", lines: " << lines << endl;
        }

        if(!TestSample()){
            cout << "User-Item-Rating mismatch, exiting" << endl;
            exit(1);
        }
    }

    cout << "File read process complete" << endl;
    PrintTimestamp();
    cout << endl;
}

/*
 * Outputs the difference in current time and the time the function was called
 */
void PrintTimestamp(){
    clock_t curr_time = std::clock();
    cout << "Time: " << (curr_time - parse_vars.start) / (double) CLOCKS_PER_SEC << "s" << endl;
    cout << "Difference : " << (curr_time - parse_vars.prev_time) / (double) CLOCKS_PER_SEC << "s" << endl;
    parse_vars.prev_time = curr_time;
}

/* 
 * Returns the rating given a compact UID
 * and a corresponding item
 * 
 * Returns -1 if the rating is not found
 */
int GetRating(int uid, int item){
    // Check if it exists in the array
    if(item > ITEMS || item < 1) return -1;
    
    // Loop through the array of vectors
    for(unsigned int i = 0; i < parse_vars.items_v[item-1].size(); i++){
        if(parse_vars.items_v[item-1].at(i).uid == uid){
            return parse_vars.items_v[item-1].at(i).rating;
        }
    }
    // Reached end of item's list
    return -1;
}

/* 
 * Outputs the number of individual cells in the
 * array of vectors
 */
unsigned int VArrSize(){
    unsigned int size = 0;
    for(int i = 0; i < ITEMS; ++i){
        size += parse_vars.items_v[i].size();
    }
    return size;
}

/* 
 * Prints each cell in the array with padding
 * Linked-List array
 */
void PrintVArr(){
    for(int i = 0; i < ITEMS; i++){
        for(unsigned int j = 0; j < parse_vars.items_v[i].size(); j++){
            PrintCell(&parse_vars.items_v[i].at(j));
        }
    }
    cout << endl;
}

/*
 * Prints a given cell's values
 * Requires a cell pointer
 */
void PrintCell(cell* cl){
    cout << "item " << cl->item << ", user " << cl->uid << ", rating " << cl->rating << endl;
}

/*
 * Adds a cell to the items[] array and
 * corresponding linked-list
 */
void UpdateVArr(int uid, int item, int rating){
    //create the corresponding cell
    cell new_cell;
    new_cell.uid = uid;
    new_cell.item = item;
    new_cell.rating = rating;

    if(DEBUG && DEBUG_UPDATE_VARR){
        cout << "Adding: item " << item << " uid " << uid << " rating " << rating << endl; 
        PrintVArr();
    }

    //add cell lto the end of the vector at index [item]
    parse_vars.items_v[item-1].push_back(new_cell);
    
    if(DEBUG && DEBUG_UPDATE_VARR){
        PrintVArr();
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
    parse_vars.old2newuid_map.insert(make_pair(uid_old,uid_new));
    parse_vars.new2olduid_map.insert(make_pair(uid_new,uid_old));
    if(DEBUG && DEBUG_UID) cout << "old uid " << uid_old << " new uid " << uid_new << endl << endl;
}

/* 
 * Reads a line and inserts the values into an array of size M x N
 * Line must be formatted like this; X,Y,Z
 * Where X is the item, Y is the user ID, and Z is the corresponding rating
 * Calls UpdateLLArr to update the user[] array of linked-lists with the newly parsed values
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
    UpdateVArr(uid, item, rating);
}

/*
 * Checks 10 random sample values to see if they exist in
 * the created linked-list array
 */
bool TestSample(){
    if(GetRating(407948, 2001) != 3){
        cout << "TestSample failed: uid " << 407948 << " item " << 2001 << endl;
        return false;
    }
    if(GetRating(188863, 10852) != 5){
        cout << "TestSample failed: uid " << 188863 << " item " << 10852 << endl;
        return false;
    }
    if(GetRating(65600, 14968) != 5){
        cout << "TestSample failed: uid " << 65600 << " item " << 14968 << endl;
        return false;
    }
    if(GetRating(306405, 8782) != 2){
        cout << "TestSample failed: uid " << 306405 << " item " << 8782 << endl;
        return false;
    }
    if(GetRating(193667, 16784) != 5){
        cout << "TestSample failed: uid " << 193667 << " item " << 16784 << endl;
        return false;
    }
    if(GetRating(353047, 1976) != 2){
        cout << "TestSample failed: uid " << 353047 << " item " << 1976 << endl;
        return false;
    }
    if(GetRating(451987, 11446) != 3){
        cout << "TestSample failed: uid " << 451987 << " item " << 11446 << endl;
        return false;
    }
    if(GetRating(189727, 1180) != 4){
        cout << "TestSample failed: uid " << 189727 << " item " << 1180 << endl;
        return false;
    }
    if(GetRating(156529, 2751) != 5){
        cout << "TestSample failed: uid " << 156529 << " item " << 2751 << endl;
        return false;
    }
    if(GetRating(92710, 16730) != 3){
        cout << "TestSample failed: uid " << 92710 << " item " << 16730 << endl;
        return false;
    }
    return true;
}
