clear
clc
close all
 
%%To calculate the magnetic field at the centre of a helmholtz coil
 
% fid = fopen('data.txt', 'w');
% fprintf(fid,'%4s\t\t %4s\t\t %4s\t\t %4s\t\t  %4s\t\t', 'Current', 'No of turns', 'Length of wire', 'Resistance', 'Power' );                %  X-axis common time
 
%Formula B = [0.8991*(10)^(-6)*(n*I)]/R
 
%mu = 12.56e-7;
k = 0.8991e-6;      
r = 0.265;           %radius of coil in metre
B = 0.001;          %magnetic field in Tesla
 
rho = 1.68e-8;      %resistivity of copper
A_wire = 2.62e-6;   %area of 13 gauge wire
n = 0;
j=1;

guage = [7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32];
areaArray = [15.6958 12.9717 10.5071 8.3019 6.8183 5.4805 4.2888 3.2429 2.6268 2.0755 1.5890 1.1675 0.8107 0.6567 0.5189 0.3973 0.2919 0.2452 0.2027 0.1642 0.1363 0.1110 0.0937 0.0779 0.0682 0.0591];
wires = cell(5,length(areaArray));


for z = 1:length(areaArray)
    
    A_wire = areaArray(z)*1e-6;
    
    for i=1:0.1:15   
      I(j)=i;                            %current
      n(j) = (B*r)/(k*I(j));             %number of turns    

      L_wire(j) = 2*pi*r*n(j);
      R(j) = (rho*L_wire(j))/(A_wire);       %resistance of wire

      P(j) = I(j)*I(j)*R(j);                 %Power required

%       fprintf(fid,'\n %4.4f \t\t %4.4f \t\t %4.4f \t\t %4.4f \t\t %4.4f\t\t', I(j), n(j), L_wire(j), R(j), P(j));      
      
      
      j=j+1;
      if j > 141
          j = 1;
      end
    end
    wires{1,z} = I;
    wires{2,z} = n;
    wires{3,z} = L_wire;
    wires{4,z} = R;
    wires{5,z} = P;
     
end
 
for i = 1:length(areaArray)
    srtr = strcat(num2str(guage(i)),'.txt');
    fid = fopen(srtr,'w');
    fprintf(fid,'%4s\t\t %4s\t\t %4s\t\t %4s\t\t  %4s\t\t\r\r', 'Current', 'No of turns', 'Length of wire', 'Resistance', 'Power' );
    for j = 1:length(I)
        fprintf(fid,'%4.1f \t\t %4.4f \t\t %4.4f \t\t %4.4f \t\t %4.4f\t\t\r',wires{1,i}(j),wires{2,i}(j),wires{3,i}(j),wires{4,i}(j),wires{5,i}(j));
    end
end

wht2plot = [10,11,12,13,14,15,16];      %%put the guages that are to be plotted
for i = wht2plot
    
    srtr = strcat(num2str(guage(i-6)),' guage wire');
    sid = figure('Name',srtr,'NumberTitle','off');
    
    subplot(3,1,1);
    title('Current V No of turns');
    plot(wires{2,i-6},wires{1,i-6},'r','LineWidth',2);
    xlabel('No of turns (nos)');
    ylabel('Current (A)');
    
    subplot(3,1,2);
    title('Power V No of turns');
    plot(wires{2,i-6},wires{5,i-6},'b','LineWidth',2);
    xlabel('No of turns (nos)');
    ylabel('Power (watts)');
    
    subplot(3,1,3);
    title('Length V No of turns');
    plot(wires{2,i-6},wires{3,i-6},'Color','#1d7526','LineWidth',2);
    xlabel('No of turns (nos)');
    ylabel('Length (m)');
    
    hold on
    
end
 
disp('simulation complete')



% figure(1)
% subplot(3,1,1)
% plot(I,n,'b','LineWidth',2.5,'MarkerSize',6);
% title (' Current vs Number of turns plot ');
% xlabel('Current(A)');
% ylabel('No.of turns');
% %ylim([480 650]);
% %xlim([0 2]);
% %set(gca,'YTick',480:50:650);
% grid;
%  
% subplot(3,1,2)
% plot(R,L_wire,'b','LineWidth',2.5,'MarkerSize',6);
% title (' Resistance vs length of wire plot ');
% xlabel('Resistance(ohm)');
% ylabel('Length of wire(m)');
% %ylim([480 650]);
% %xlim([0 2]);
% %set(gca,'YTick',480:50:650);
% grid;
%  
% subplot(3,1,3)
% plot(P,R,'b','LineWidth',2.5,'MarkerSize',6);
% title (' Power vs Resistance of wire ');
% xlabel('Power(watt)');
% ylabel('Resistance(ohm)');
% %ylim([480 650]);
% %xlim([0 2]);
% %set(gca,'YTick',480:50:650);
% grid;
 
 
%figure(1);
%plot(I,n);


