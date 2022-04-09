clc
close all
clear 

raw = importdata('omega_vals.txt');

omgs = raw(:,2);
t = raw(:,3);
alpha = zeros(length(t)-1,1);

for i = 1:1:length(t)-1
    alpha(i,1) = (omgs(i,1) - omgs(i+1,1))/(t(i+1,1) - t(i,1));
end

disp(['Mean angular decellaeration: ' num2str(mean(alpha(:,1)))]);
