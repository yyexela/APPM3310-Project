#ifndef _parse_csv_h
#define _parse_csv_h

#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

// User amount
#define ITEMS 17770


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
void UpdateLLArr(int uid, int item, int rating);
void PrintLLArr();
void PrintCell(cell* cl);
unsigned int LLArrSize();
void PrintTimestamp();
int GetRating(int uid, int item);
void ProcessFiles();
bool TestSample();

// Boolean used to print debug info
const bool DEBUG = false;
const bool DEBUG_UID = false;
const bool DEBUG_SPLINE = false;
const bool DEBUG_UPDATE_USER = false;

// Files to read from
const string SPARSE_FILE = "InputCSV/sparse_matrix_coords_and_values.csv";
const string UIDMAP_FILE = "InputCSV/userID_map.csv";

class Parse{
    public:
    // Hashmap for userID map
    typedef unordered_map<int, int> uid_map_t;
    uid_map_t old2newuid_map;
    uid_map_t new2olduid_map;

    // Clock to check run-time
    clock_t start = std::clock();

    // User array of linked-lists
    cell* items[ITEMS];

};

#endif