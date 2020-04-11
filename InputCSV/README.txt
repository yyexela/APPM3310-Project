README

sparse_matrix_coord_and_values.csv

First column is movie ID. Second column is the compact user ID. Third column is the rating.


    MovieIDs range from 1 to 17770 sequentially.
    CustomerIDs range from 1 to 2649429, with gaps. There are 480189 users.
    Ratings are on a five star (integral) scale from 1 to 5.


userID_map.csv

First column is the original user ID. Second column is the corresponding compact user ID.

processed_probe.csv

First column is movie ID. Second column is the compact user ID. Third column is the rating.

Apparently probe data is a subset of the training data, and the purpose is to use the exact set of data that Netflix themselves used for valid comparison. 

For probe data and what criteria we should use for comparison, see https://www.netflixprize.com/faq.html

An excerpt:
What is the probe subset for? It wasn’t mentioned in the Prize Rules.
The probe subset helps reduce the number of times you need to go to the scoring oracle. It has both similar size and characteristics to the quiz subset. However, unlike the quiz subset, you do have the answers for the probe subset. The probe subset enumerates a set of customer and movie id pairs whose ratings and dates are included in the training set we supplied. You just need to ask your system to make predictions for those pairs and then compute your RMSE based on the actual ratings for the pairs.
The RMSE Cinematch can achieve on the probe dataset is 0.9474. You can compare your progress against that number as often as you want. After someone wins the Grand Prize we’ll release the withheld ratings in the quiz and test subsets. We want to make a lasting contribution to the academic community before that: Providing standard training and test sets help people share observations and results while the Prize is in progress.
By the way, the md5 signature of the judging file, which contains the qualifying set ratings and defines the quiz and subset members, is 33d98576809268177d35c182898b6439. We’ll publish that file at the end of the Contest, when the entire dataset is deposited in the Machine Learning Archive at UC Irvine.

How to calculate RMSE:
https://en.wikipedia.org/wiki/Root-mean-square_deviation

For more information about the contest and raw data, see https://www.kaggle.com/netflix-inc/netflix-prize-data/



