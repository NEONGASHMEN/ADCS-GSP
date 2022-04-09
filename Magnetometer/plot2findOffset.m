close all
clear
clc

data = importdata('opts-1.txt');
fig1 = figure();
xlim([-100 100]);
ylim([-100 100]);
zlim([-100 100]);
plot3(0,0,0,'*');
hold on;
line([-100 100],[0 0],[0 0]);
line([0 0],[-100 100],[0 0]);
line([0 0],[0 0],[-100 100]);

plot3(data(:,1),data(:,2),data(:,3),'.');
