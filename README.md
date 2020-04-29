# MatrixProject

Project for APPM 3310 at CU Boulder.

## Group

- Alexey Yermakov
- Jaden Wang

## Folders

### InputCSV

**testsparse_matrix_coords_and_values.csv**
* Sample file for the format of the real `sparse_matrix_coords_and_values.csv` file  .
  
**testuserID_map.csv**
* Sample file for the format of the real `userID_map.csv` file.

**README.txt**
* Explains how the used CSV files are formatted, how the Netflix Prize competition measured progress, and various links to relevant information (including how to calculate RMSE and the Kaggle site for where to get the data from).

### Factorization

**makefile**
- Compiles the binary executable factorize by running `make` in the directory on the command line.
  - Note that the compiler is g++, so make sure you compile on a Unix-like OS.
  - Also note the compiler flags that are used to optimize the final binary, the program isn't portable (if at all) so recompile for each new device you run it on.
  - IMPORTANT: Make sure you have Boost Libraries installed!!! Change the directory path of the makefile to locate boost (in the one provided, you would modify `~/boost/...` to `<path-to>/boost/...`) everywhere you see linking.
- Cleans the directory by running `make clean`. 

**parse-csv.cc**
- Program that parses through the files in the `InputCSV` directory and stores the resulting values in an array of vectors.

**parse-csv.h**
- Contains the Parse class which stores the array of vectors containing initial ratings.

**factorize.c**
- Performs the gradient descent of the data set provided by `InputCSV` (for this project, the Netflix data set).
- Saves the final data to `serialized` and a time log to `time.log`
  
**factorize.h**
- Contains the Factorize class which stores the two final matrices for the decomposition/factorization.

**globals.h**
- Contains global variables used throughout the program.
- Contains debug pre-processor definitions.

### CreateCSV
**create-csv.cc**
- Takes in a file called `serialized` in the same folder, which should result from serializing the Factorize class, and loads it into a run-time Factorize class. The file then goes through and creates two separate item/user-feature matrices as CSV files to be used elsewhere (used in the project to get a final RMSE and compare to Netflix's original recommender system score).

**create-csv.h**
- Contains forward declarations for create-csv.cc.

**makefile**
- Compiles create-csv.cc and create-csv.h.

### Pre_Post_Processing

**clean_training.py**

**item_user_rating_sparse_matrix.m**

**probeRMSE.m**

**removeProbe.m**
