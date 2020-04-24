#ifndef _parse_csv_h
#define _parse_csv_h

#include "globals.h"

#include <string>
#include <unordered_map>
#include <ctime>
#include <vector>

using namespace std;

// Node for linked-list for the user-item matrix
struct cell{
    short rating;
    int uid;
    int item;
};

// Forward declarations
void SparseLine(string line);
void UIDMapLine(string line);
void UpdateVArr(int uid, int item, int rating);
void PrintVArr();
void PrintCell(cell* cl);
unsigned int VArrSize();
void PrintTimestamp();
int GetRating(int uid, int item);
void ProcessFiles();
bool TestSample();
void MakeAvgItem();
void MakeAvgUser();


// Files to read from
const string SPARSE_FILE = "../InputCSV/sparse_matrix_coords_and_values.csv";
const string UIDMAP_FILE = "../InputCSV/userID_map.csv";

class Parse{
    public:
    // Hashmap for userID map
    typedef unordered_map<int, int> uid_map_t;
    uid_map_t old2newuid_map;
    uid_map_t new2olduid_map;

    // Clock to check run-time
    clock_t start = std::clock();
    clock_t prev_time = start;

    // Item array of vectors:
    // There are ITEMS items with a vector of ratings by users for each item
    vector <cell> items_v[ITEMS];

    // Contains the average initial rating for each movie
    double item_avg[ITEMS];

    // Contains the average initial offset for ratings by each user
    double user_avg[USERS];

    // Global average and global offset
    double global_avg, global_offset;
};

#endif