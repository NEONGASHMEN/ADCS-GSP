function derivative = Satellite(t,state)

global BB invI I Bfieldmeasured pqrdotmeasured trgt
global Bfieldnav pqrdotnav current voltage muB torq

%%Earth and sat params
Earth
Inertia

x = state(1);
y = state(2);
z = state(3);

q0123 = state(7:10);
pdot = state(11);
qdot = state(12);
rdot = state(13);
pqrdot = state(11:13);

%%translation
vel = state(4:6);
r = state(1:3);
rho = norm(r);
rhat = r/rho;
Fgrav = -(G*M*m/rho^2)*rhat;
acc = Fgrav/m;

%%Mag stuff
phiE = 0;
thetaE = acos(z/rho);
psiE = atan2(y,x);
latitude = 90-thetaE*180/pi;
longitude = psiE*180/pi;
rhokm = (rho)/1000;
[BN,BE,BD] = igrf('01-Jan-2020',latitude,longitude,rhokm,'geocentric');
%%%Convert NED (North East Down to X,Y,Z in ECI frame)
%%%First we need to create a rotation matrix from the NED frame to the 
%%%inertial frame
BNED = [BN;BE;BD]; 
BI = TIB(phiE,thetaE+pi,psiE)*BNED;
BB = TIBquat(q0123)'*BI;
BB = BB*1e-9;

if (norm(pqrdot) < 0.0005)
    trgt = 1;
end

%%Bfield and ang velocty measured from snsr
[Bfieldmeasured,pqrdotmeasured] = Sensor(BB,pqrdot);

%%Navigation Block (correction)
[Bfieldnav,pqrdotnav] = Navigation(Bfieldmeasured,pqrdotmeasured);


%%CONTROL BLOCK
muB_cont = Control(Bfieldnav,pqrdotnav);
Magtorquer_params
current_cont = muB_cont/(n*A*tempVar);
voltage_cont = current_cont*Rnet;

voltage = Analog(voltage_cont);
current = voltage/Rnet;
muB = current*n*A*tempVar;

LMN_magtorquers = cross(muB,BB);

%%%Rotational Dynamics
PQRMAT = [0 -pdot -qdot -rdot;pdot 0 rdot -qdot;qdot -rdot 0 pdot;rdot qdot -pdot 0];
q0123dot = 0.5*PQRMAT*q0123;
H = I*pqrdot;
pqrddot = invI*(LMN_magtorquers - cross(pqrdot,H));
torq = LMN_magtorquers - cross(pqrdot,H);

derivative = [vel;acc;q0123dot;pqrddot];

