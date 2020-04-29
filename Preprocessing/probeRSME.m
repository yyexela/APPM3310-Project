file = "C:\Users\Andrews Lab\Documents\project\item_feature_nonprobe.csv";
Q = readtable(file);
file = "C:\Users\Andrews Lab\Documents\project\user_feature_nonprobe.csv";
P = readtable(file);
file = "C:\Users\Andrews Lab\Documents\project\processed_probe.csv";
R = readtable(file);

Q = table2array(Q);
P = table2array(P);
R = table2array(R);

ping=0;
pong=0;
err=0;
for n=1:size(R,1)
    i=R(n,1);
    u=R(n,2);
    score=0;
    for m=1:size(Q,2)
        score = score + Q(i,m)*P(u,m);
        %clipping
        if score > 5
            score = 5;
            ping=ping+1;
        elseif score < 1
            score = 1;
            pong=pong+1;
        end
    end
    err = err + (R(n,3)-score)^2;
end

rmse = (err/size(R,1))^.5;
perc = (0.9474 - rmse)/.9474 * 100;
if perc > 0
    fprintf("The RMSE is %f, beating Netflix by %f%%. We are millionaires now!\n",rmse,perc);
else
    fprintf("The RMSE is %f, we are behind by %f%%. I guess biases are important!\n",rmse,perc);
end