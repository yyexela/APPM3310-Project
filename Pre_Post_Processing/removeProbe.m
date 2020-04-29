file = "C:\Users\Andrews Lab\Documents\project\sparse_matrix_coords_and_values.csv";
F = readtable(file);
file = "C:\Users\Andrews Lab\Documents\project\processed_probe.csv";
P = readtable(file);
F = table2array(F);
P = table2array(P);
%remove probe data
F(ismember(F,P,'rows'),:)=[];
writematrix(F,'nonProbeData.csv') ;
%check if unique movies and users are the same
I=unique(F(:,1),'rows');
U=unique(F(:,2),'rows');