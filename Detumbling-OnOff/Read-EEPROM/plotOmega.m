clc
close all
clear 

raw1 = importdata('./09-05/detumbling.txt');
raw2 = importdata('./09-05/tumbling.txt');

fig = figure();
plot(raw1(:,1),raw1(:,2));
hold on;
plot(raw2(:,1),raw2(:,2),'k');
hold on;
line([0 4500],[0 0]);
hold on;
line([0 0],[0 800]);