n = 350;                    %%No of turns
rodRadius = 2.85e-3;           %%torquer rod
muR = 1453;                 %%steel core
length = 86e-3;
A = pi*(rodRadius^2);       %%Area of cs
RperLength = 0.212872;      %%28 AWG
Rnet = RperLength*2*pi*rodRadius*n;     %%Net R

% demagnetisation fctr
Nd = (4*(log(length/rodRadius) - 1))/(((length^2)/(rodRadius^2))-(4*log(length/rodRadius)));

tempVar = 1 + ((muR - 1)/(1 + (Nd*(muR - 1))));
muBnorm = 0.2;              %%Mag moment  0.2Am2
