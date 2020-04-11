C = cell(17770,1);
%construct cell array for sparse matrix format {item,user,rating}
for n=1:17770
    file = "C:\Users\bryan\Downloads\nf_prize_dataset\cleaned_training_set\mv_" + string(n) + ".csv";
    T = readtable(file);
    %movie ID
    i = ones(size(T,1),1)*n;
    %user ID
    j = table2array(T(:,1));
    %ratings
    r = table2array(T(:,2));
    n
    C{n} = [i,j,r];
end
D = cell2mat(C);
A = sparse(D(:,1),D(:,2),D(:,3),2649429,17770);
save('raw_sparse_matrix.mat','A');
data=A;
%remove empty columns (where users do not exist)
data( :, ~any(data,1) ) = [];  %columns
%data( ~any(data,2), : ) = [];  %rows
%sanity check
max(data,[],'all')
save('sparse_matrix.mat','data');
%indices (j) represent original IDs
[i,j,r] = find(A);
%indicies (J) represent compact IDs
[I,J,V] = find(data);
IJV=[I J V];
writematrix(IJV,'sparse_matrix_coords_and_values.csv') ;

%sanity check
min(j==y);
min(r==v);

%make original to compact ID map
D=[i x];
issorted(D(:,2))
U=unique(sort(D,1), 'rows');
min(A(6,:)==data(1,:));
min(nonzeros(A(6,:))==nonzeros(data(1,:)));
save('ID_map','U');
writematrix(U,'ID_map.csv') ;

%visualize the sparsity of matrix (too much data, looking denser than it is)
spy(data(1:17770,1:17770));