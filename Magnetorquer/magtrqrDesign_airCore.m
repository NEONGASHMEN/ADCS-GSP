%Same stuffs as prev one

clear
clc
close all

N = 0;
I = 0;
Rnet = 0;
P = 0;

l = 80e-3;
b = 80e-3;
A = l*b;
muB = 0.2;
rho = 1.68e-8;
Rperlength = 0.212872;

interval = 10:10:200;
nMatrix = zeros(length(interval),1);
currentMatrix = zeros(length(interval),1);
powerMatrix = zeros(length(interval),1);

for i = interval
    
    N = i;
    I = muB/(N*A);
    Rnet = Rperlength*2*(l+b)*N;
    P = I*I*Rnet;
    
    currentMatrix(i/10) = I;
    powerMatrix(i/10) = P;
    nMatrix(i/10) = N;
    
end

fig1 = figure('Name','Current V No of turns');
plot(nMatrix,currentMatrix,'Color','b');

fig2 = figure('Name','Power V No of turns');
plot(nMatrix,powerMatrix,'Color','k');



