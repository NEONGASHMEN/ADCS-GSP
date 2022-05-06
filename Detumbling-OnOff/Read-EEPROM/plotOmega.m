clc
close all
clear 

raw1 = importdata('output_protumbling.txt');
raw2 = importdata('output_detumbling.txt');
raw3 = importdata('output_detumbling1.txt');
raw4 = importdata('output_detumbling2.txt');

fig = figure();
plot(raw1(:,1),raw1(:,2));
hold on;
plot(raw2(:,1),raw2(:,2),'k');
hold on;
plot(raw3(:,1),raw3(:,2),'r');
hold on;
plot(raw4(:,1),raw4(:,2),'g');