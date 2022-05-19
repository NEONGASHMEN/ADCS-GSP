clc
close all
clear 

Draw1 = importdata('./11-05/detumb_1.txt');
Draw1(:,2) = abs(Draw1(:,2));
Draw2 = importdata('./14-05/detumb_1.txt');
Draw2(:,2) = abs(Draw2(:,2));
Draw3 = importdata('./14-05/detumb_2.txt');
Draw3(:,2) = abs(Draw3(:,2));
Draw4 = importdata('./11-05/detumb_4.txt');
Draw4(:,2) = abs(Draw4(:,2));
Draw5 = importdata('./11-05/detumb_5.txt');
Draw5(:,2) = abs(Draw5(:,2));
D = {Draw1,Draw2,Draw3,Draw4,Draw5};

Traw1 = importdata('./13-05/tumb_1.txt');
Traw1(:,2) = abs(Traw1(:,2));
Traw2 = importdata('./19-05/tumb_1.txt');
Traw2(:,2) = abs(Traw2(:,2));
Traw3 = importdata('./13-05/tumb_3.txt');
Traw3(:,2) = abs(Traw3(:,2));
Traw4 = importdata('./13-05/tumb_4.txt');
Traw4(:,2) = abs(Traw4(:,2));
Traw5 = importdata('./13-05/tumb_5.txt');
Traw5(:,2) = abs(Traw5(:,2));
T = {Traw1,Traw2,Traw3,Traw4,Traw5};

%%Instantaneous angular accelerations

Acc_De1 = zeros(size(Draw1(:,1)));
Acc_De2 = zeros(size(Draw2(:,1)));
Acc_De3 = zeros(size(Draw3(:,1)));
Acc_De4 = zeros(size(Draw4(:,1)));
Acc_De5 = zeros(size(Draw5(:,1)));

Acc_Tum1 = zeros(size(Traw1(:,1)));
Acc_Tum2 = zeros(size(Traw2(:,1)));
Acc_Tum3 = zeros(size(Traw3(:,1)));
Acc_Tum4 = zeros(size(Traw4(:,1)));
Acc_Tum5 = zeros(size(Traw5(:,1)));

Acc_De = {Acc_De1,Acc_De2,Acc_De3,Acc_De4,Acc_De5};
Acc_Tum = {Acc_Tum1,Acc_Tum2,Acc_Tum3,Acc_Tum4,Acc_Tum5};

for i = 1:5
    idx1 = size(Acc_De{i});
    for j = 1:(idx1(1)-1)
        Acc_De{i}(j,1) = (D{i}(j,2)-D{i}(j+1,2))*1000/(D{i}(j+1,3)-D{i}(j,3));
    end
    idx2 = size(Acc_Tum{i});
    for k = 1:(idx2(1)-1)
        Acc_Tum{i}(k,1) = (T{i}(k,2)-T{i}(k+1,2))*1000/(T{i}(k+1,3)-T{i}(k,3));
    end
end

%%Export
D_omegaLast = [Draw1(end,2);Draw2(end,2);Draw3(end,2);Draw4(end,2);Draw5(end,2)];
D_timeDelta = [Draw1(end,3)-Draw1(1,3);Draw2(end,3)-Draw2(1,3);Draw3(end,3)-Draw3(1,3);Draw4(end,3)-Draw4(1,3);Draw5(end,3)-Draw5(1,3)];
out = cell(4,7);
out{1,2} = "Test 1";
out{1,3} = "Test 2";
out{1,4} = "Test 3";
out{1,5} = "Test 4";
out{1,6} = "Test 5";
out{1,7} = "Mean";
out{2,1} = "Avrg Acceleration: ";
out{3,1} = "Max Acceleration: ";
out{4,1} = "Detumbling time (from 700deg/s): ";

for i = 1:5
    out{2,i+1} = num2str(mean(Acc_De{i}(:,1)));
    out{3,i+1} = num2str(max(Acc_De{i}(:,1)));
    out{4,i+1} = num2str(700/mean(Acc_De{i}(:,1)));
end
for i = 1:3
    out{i+1,7} = num2str(mean([str2num(out{i+1,2}) str2num(out{i+1,3}) str2num(out{i+1,4}) str2num(out{i+1,5}) str2num(out{i+1,6})]));
end

writecell(out,"Output-Detumbling.csv");

%%-------------------------------------------------------------------------------------------------------------------------------------%%
T_omegaLast = [Traw1(end,2);Traw2(end,2);Traw3(end,2);Traw4(end,2);Traw5(end,2)];
T_timeDelta = [Traw1(end,3)-Traw1(1,3);Traw2(end,3)-Traw2(1,3);Traw3(end,3)-Traw3(1,3);Traw4(end,3)-Traw4(1,3);Traw5(end,3)-Traw5(1,3)];
out = cell(4,7);
out{1,2} = "Test 1";
out{1,3} = "Test 2";
out{1,4} = "Test 3";
out{1,5} = "Test 4";
out{1,6} = "Test 5";
out{1,7} = "Mean";
out{2,1} = "Avrg Acceleration: ";
out{3,1} = "Max Acceleration: ";
out{4,1} = "Detumbling time (from 700deg/s): ";

for i = 1:5
    out{2,i+1} = num2str(mean(Acc_Tum{i}(:,1)));
    out{3,i+1} = num2str(max(Acc_Tum{i}(:,1)));
    out{4,i+1} = num2str((T_timeDelta(i,1)/1000)+(T_omegaLast(i,1)/mean(Acc_Tum{i}(:,1))));
end
for i = 1:3
    out{i+1,7} = num2str(mean([str2num(out{i+1,2}) str2num(out{i+1,3}) str2num(out{i+1,4}) str2num(out{i+1,5}) str2num(out{i+1,6})]));
end

writecell(out,"Output-Tumbling.csv");

%%Figures

fig1 = figure();

%%Plot detumbling omegas
plot(Draw1(:,1),Draw1(:,2),'b');
hold on;
plot(Draw2(:,1),Draw2(:,2),'g');
hold on;
plot(Draw3(:,1),Draw3(:,2),'g');
hold on;
% plot(Draw4(:,1),Draw4(:,2),'b');
% hold on;
% plot(Draw5(:,1),Draw5(:,2),'b');
% hold on;

%%Plot tumbling omegas
plot(Traw1(:,1),Traw1(:,2),'r');
hold on;
plot(Traw2(:,1),Traw2(:,2),'y');
hold on;
plot(Traw3(:,1),Traw3(:,2),'r');
hold on;
plot(Traw4(:,1),Traw4(:,2),'r');
hold on;
plot(Traw5(:,1),Traw5(:,2),'r');

fig2 = figure();

%%Plot total decelleration
plot(Draw1(:,1),Acc_De{1}(:,1));
hold on;
% plot(Draw2(:,1),Acc_De{2}(:,1));
% plot(Draw3(:,3),Acc_De{3}(:,1));
% plot(Draw4(:,4),Acc_De{4}(:,1));
% plot(Draw5(:,5),Acc_De{5}(:,1));
line([0 4500],[0 0],'Color','k');



