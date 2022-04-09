
% Copyright - Carlos Montalvo 2015
%https://github.com/cmontalvo251/MATLAB/tree/master/ADCS_Seminar_Series


%%Main
clear
clc
close all

disp('Sim started');

tic
global BB Bfieldmeasured pqrdotmeasured current voltage muB torq
global Bfieldnav pqrdotnav Bfieldnavprev pqrdotnavprev trgt

%%Earth and orbit params
Earth
Orbit

%%Initial Params
x0 = Rad_mod;
y0 = 0;
z0 = 0;
xdot0 = 0;
ydot0 = Velocity*cos(Inclination);
zdot0 = Velocity*sin(Inclination);
p = 0;
q = 0;
r = 0;
pqr = [p q r];
q0123_0 = EulerAngles2Quaternions(pqr);
pdot = 0;
qdot = 0;
rdot = 0.174533;
state = [x0 y0 z0 xdot0 ydot0 zdot0 q0123_0 pdot qdot rdot];


%%Time params
period = 2*pi*sqrt((Rad_mod^3)/mu);
no_of_orbs = 0.5;
tfinal = no_of_orbs*period;
timestep = 0.2;
tout = 0:timestep:tfinal;
stateout = zeros(length(tout),length(state));
trgt = 0;
deed = 0;

%%Initialising mag matrices
Bxs = zeros(length(tout),1);    %%B field from IGRF
Bys = zeros(length(tout),1);
Bzs = zeros(length(tout),1);
Bxm = zeros(length(tout),1);    %%Polluted B field
Bym = zeros(length(tout),1);
Bzm = zeros(length(tout),1);
Bxn = zeros(length(tout),1);    %%Navigated B field
Byn = zeros(length(tout),1);
Bzn = zeros(length(tout),1);
Bfieldnavprev = [0;0;0];

%%Initialising polluted ang velocity matrices
pqrdot_measured = zeros(length(tout),3);
pqrdot_navigation = zeros(length(tout),3);
pqrdotnavprev = [0;0;0];

%%Initialising other matrices
currentmatrix = zeros(length(tout),4);
voltagematrix = zeros(length(tout),4);
muBmatrix = zeros(length(tout),4);
powermatrix = zeros(length(tout),1);
torqmatrix = zeros(length(tout),3);
angaccmatrix = zeros(length(tout),3);
power = [0 0 0];

%%RK4
state = state';
for i = 1:length(tout)
    stateout(i,:) = state;
    k1 = Satellite(tout,state);
    k2 = Satellite(tout + timestep/2,state + k1*timestep/2);
    k3 = Satellite(tout + timestep/2,state + k2*timestep/2);
    k4 = Satellite(tout + timestep,state + k3*timestep);
    k = (1/6)*(k1 + 2*k2 + 2*k3 + k4);
    state = state + k*timestep;
    
    Bxs(i,1) = BB(1);
    Bys(i,1) = BB(2);
    Bzs(i,1) = BB(3);
    
    Bxm(i,1) = Bfieldmeasured(1);
    Bym(i,1) = Bfieldmeasured(2);
    Bzm(i,1) = Bfieldmeasured(3);
    
    Bxn(i,1) = Bfieldnav(1);
    Byn(i,1) = Bfieldnav(2);
    Bzn(i,1) = Bfieldnav(3);
    
    pqrdot_measured(i,1) = pqrdotmeasured(1);
    pqrdot_measured(i,2) = pqrdotmeasured(2);
    pqrdot_measured(i,3) = pqrdotmeasured(3);
    
    pqrdot_navigation(i,1) = pqrdotnav(1);
    pqrdot_navigation(i,2) = pqrdotnav(2);
    pqrdot_navigation(i,3) = pqrdotnav(3);
    
    currentmatrix(i,1) = current(1);
    currentmatrix(i,2) = current(2);
    currentmatrix(i,3) = current(3);
    currentmatrix(i,4) = norm(current);
    
    voltagematrix(i,1) = voltage(1);
    voltagematrix(i,2) = voltage(2);
    voltagematrix(i,3) = voltage(3);
    voltagematrix(i,4) = norm(voltage);

    muBmatrix(i,1) = muB(1);
    muBmatrix(i,2) = muB(2);
    muBmatrix(i,3) = muB(3);
    muBmatrix(i,4) = norm(muB);
    
    power(1) = current(1)*voltage(1);
    power(2) = current(2)*voltage(2);
    power(3) = current(3)*voltage(3);
    powermatrix(i,1) = sum(abs(power));
    
    torqmatrix(i,1) = torq(1);
    torqmatrix(i,2) = torq(2);
    torqmatrix(i,3) = torq(3);
    
    angaccmatrix(i,1) = torq(1)*0.006;
    angaccmatrix(i,1) = torq(1)*0.006;
    angaccmatrix(i,1) = torq(1)*0.006;
    
    if (trgt == 1) & (deed == 0)
        t_dtmbl = i;
        deed = 1;
    end
    
    if mod(i,10) == 0
        perc = (i/length(tout))*100;
        disp(['Percentage completed: ' num2str(perc)]);
    end
    
end

%%In kms
stateout(:,1:6) = stateout(:,1:6)/1000;

%%Extraction
pqrdot_out = stateout(:,11:13);
Bnorm = sqrt(Bxs.^2 + Bys.^2 +Bzs.^2);
Xout = stateout(:,1);
Yout = stateout(:,2);
Zout = stateout(:,3);
no_of_discretes = max([length(unique(muBmatrix(:,1)));length(unique(muBmatrix(:,2)));length(unique(muBmatrix(:,3)))]);

%%Display
disp('Simulation completed');
toc
disp(' ');

%%Print critical values
if deed == 1
    disp(['Detumbling in: ' num2str(t_dtmbl/60) ' mins']);
else
    t_dtmbl = 1200;
    disp("Angular velocity have never crossed the min value specified");
    disp("Average value calculated from 20th min onwards");
end
meanResidualW(1) = mean(abs(pqrdot_out(t_dtmbl:length(tout),1)));
meanResidualW(2) = mean(abs(pqrdot_out(t_dtmbl:length(tout),2)));
meanResidualW(3) = mean(abs(pqrdot_out(t_dtmbl:length(tout),3)));

maxResidualW(1) = max(abs(pqrdot_out(t_dtmbl:length(tout),1)));
maxResidualW(2) = max(abs(pqrdot_out(t_dtmbl:length(tout),2)));
maxResidualW(3) = max(abs(pqrdot_out(t_dtmbl:length(tout),3)));

disp(['Mean residual omega: ' num2str(meanResidualW(3))]);
disp(['Max residual omega: ' num2str(maxResidualW(3))]);
disp(['No of discrete outputs: ' num2str(no_of_discretes)]);
 
%%plot x y z
fig0 = figure('Name','Pstn V Time','NumberTitle','off');
plot(tout,Xout,'r-');
hold on
plot(tout,Yout,'g-');
hold on
plot(tout,Zout,'b-');

%%plot Bx By Bz wrt BInertial frame
fig1 = figure('Name','Field vals V Time','NumberTitle','off');
plot(tout,Bxs,'b-','Linewidth',2);
hold on
plot(tout,Bys,'g-','LineWidth',2);
plot(tout,Bzs,'r-','LineWidth',2);
% plot(tout,Bxm,'b--');
% plot(tout,Bym,'g--');
% plot(tout,Bzm,'r--');
% plot(tout,Bxn,'k--');
% plot(tout,Byn,'c--');
% plot(tout,Bzn,'m--');
ylabel('B components in T');
xlabel('Time');

%%Plot Bnorm wrt Inertial frame
fig2 = figure('Name','Field norm V Time','NumberTitle','off');
plot(tout,Bnorm,'LineWidth',3);
xlabel('Time');
ylabel('B');
ylabel('Magnetic field in T');
xlabel('Time');

%%Plot Angular Velocity
fig3 = figure('Name','Angular velocity V Time','NumberTitle','off');
plot(tout,pqrdot_out,'LineWidth',2);
hold on 
% plot(tout,pqrdot_measured,'--');
% plot(tout,pqrdot_navigation,'--','LineWidth',2);
xlabel('Time (s)');
ylabel('Angular Velocity (rad/s)');

%%Plot current
fig4 = figure('Name','Current V Time','NumberTitle','off');
plot(tout,currentmatrix(:,1:3));
ylabel('Current (A)');
xlabel('Time (s)');

%%Plot power
fig5 = figure('Name','Power draw V Time','NumberTitle','off');
plot(tout,powermatrix);
xlabel('Time (s)');
ylabel('Power (W)');

%%Plot torque
fig6 = figure('Name','Torque V Time','NumberTitle','off');
plot(tout,torqmatrix);
xlabel('Time (s)');
ylabel('Torque (Nm)');
