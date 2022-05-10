clc
close all
clear 

raw1 = importdata('./10-05/de test 1.txt');
raw2 = importdata('./10-05/de test 2.txt');
raw3 = importdata('./10-05/tumbling test 1.txt');

fig = figure();
plot(raw1(:,3),raw1(:,2));
hold on;
plot(raw2(:,3),raw2(:,2),'k');
hold on;
plot(raw3(:,3),raw3(:,2),'r');
hold on;
line([0 200000],[0 0]);
hold on;
line([0 0],[0 800]);