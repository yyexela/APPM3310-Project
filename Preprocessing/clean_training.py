# -*- coding: utf-8 -*-
"""
Created on Fri Mar 27 13:52:40 2020

@author: bryan

The goal of this script is to create a user by item matrix from the Netflix 
data downloaded from https://archive.org/download/nf_prize_dataset.tar

Note that the raw data were organized by movie IDs. For each movie, it has

CustomerID,Rating,Date

    MovieIDs range from 1 to 17770 sequentially.
    CustomerIDs range from 1 to 2649429, with gaps. There are 480189 users.
    Ratings are on a five star (integral) scale from 1 to 5.
    Dates have the format YYYY-MM-DD.
    
The first step is to remove redundant ratings of the same item from the same
user, since we are ignoring temporal bias. We are using Python to achieve this
due to ease of manipulation for a variety of datatypes.

Then we are going to construct the enormous sparse matrix of user by item in 
MATLAB due to more efficient handling of large matrices.

It turned out there are no redundant ratings in the training dataset. 

"""

import pandas as pd
from datetime import datetime


path = 'C:/Users/bryan/Downloads/nf_prize_dataset/training_set/mv_'
df=pd.DataFrame()
for i in range(1,17771):
    #zero padding
    mvid = '0'*(7-len(str(i)))+str(i)
    file = path+mvid+'.txt'
    headers = ['CustomerID','Rating','Date']
    #dtypes = {'CustomerID': 'int', 'Rating': 'int', 'Date': 'str'}
    dateparse = lambda x: datetime.strptime(x, '%Y-%m-%d')
    parse_dates = ['Date']
    df = pd.read_csv(file,names=headers,parse_dates=parse_dates,date_parser=dateparse,skiprows=1)
    l=len(df)
    #choosing only the earliest rating
    df = df.sort_values('Date', ascending=True).drop_duplicates(subset='CustomerID', keep="first").sort_index()
    if len(df) != l:
        #no "gotcha" appeared means we don't have redundant ratings
        print(i+', gotcha!\n')
    out= 'C:/Users/bryan/Downloads/nf_prize_dataset/cleaned_training_set/mv_'+str(i)+'.csv'
    df.to_csv(out,index=False,header=False)

