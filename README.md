# MatrixProject

Project for APPM 3310 at CU Boulder

## Group

- Tyler Jensen
- Jaden Wang
- Alexey Yermakov

## CSV Files

Git doesn't allow you to upload files larger than 25MB, so download the CSV files from the Google Drive folder into the `/InputCSV` folder

## Files

**makefile**
- Compiles the binary executable factorize by running `make` in the directory on the command line
  - Note that the compiler is g++, so make sure you compile on a Unix-like OS
  - Also note the compiler flags that are used to optimize the final binary, the program isn't portable (if at all) so recompile for each new device you run it on
  - IMPORTANT: Make sure you have Boost Libraries installed!!! Change the directory path of the makefile to locate boost (in the one provided, you would modify `~/boost/...` to `<path-to>/boost/...`)
- Cleans the directory by running `make clean`  

**parse-csv.cc**
- Program that parses through the files in the `InputCSV` directory and stores the resulting values in an array of vectors  

**factorize.c**
- Performs the stochastic gradient descent of the data set provided by `InputCSV` (for this project, the Netflix data set)  

**/InputCSV/testsparse_matrix_coords_and_values.csv**
- Sample file for the format of the real `sparse_matrix_coords_and_values.csv` file  
  
**/InputCSV/testuserID_map.csv**
- Sample file for the format of the real `userID_map.csv` file  

**globals.h**
- Contains global variables used throughout the program  

**factorize.h**
- Contains the Factorize class which stores the two final matrices for the decomposition/factorization  

**parse-csv.cc**
- Contains the Parse class which contains the array of vectors which is created when reading the CSV files along with a pair of maps which link old and new user-IDs together  
