# -*- coding: utf-8 -*-
"""
Probe data only contains movie and user ID, we need to add the ratings from
training data ourselves and reorganize using sparse matrix format.

Created on Sun Apr  5 17:32:27 2020
@author: bryan
"""

import pandas as pd
import csv

path = 'C:/Users/bryan/Downloads/nf_prize_dataset/cleaned_training_set/mv_'
probe = 'C:/Users/bryan/Downloads/nf_prize_dataset/probe.txt'
idmap = 'C:/Users/bryan/Documents/MATLAB/projects/ID_map.csv'

#make a dictionary where key is movie ID and values are users who rated this movie
myfile = open(probe, "r") 
d = { } 
key=[]
for line in myfile: 
    #':' is the delimiter between movie ID and all its users
    if ':' in line:
        x = line.split(":") 
        key = int(x[0])
        d[key] = []
    else:
        id = int(line)
        d[key].append(id)

data=[]

for key in d:
    print(str(key)+'\n')
    #key is movie id
    file = path+str(key)+'.csv'
    headers = ['CustomerID','Rating','Date']
    #read csv file containing rating information for that specific movie ID
    df1 = pd.read_csv(file,names=headers,index_col=['CustomerID'])    
    for id in d[key]:
        #id is user id
        rating=df1.at[id,'Rating']
        data.append((key,id,rating))

#read the original and compact user ID map into a dictionary
#where key is original ID and value is compact ID
iddict=pd.read_csv(idmap, header=None, index_col=0, squeeze=True).to_dict()

newdata=[]
#convert all original ID to compact ID
for i in range(len(data)):
    print(i)
    lst=list(data[i])
    lst[1]=iddict[lst[1]]
    newdata.append(tuple(lst))
        
df=pd.DataFrame(newdata,columns=['Item','User','Rating'])    

out= 'C:/Users/bryan/Downloads/nf_prize_dataset/processed_probe.csv'
df.to_csv(out,index=False,header=False)