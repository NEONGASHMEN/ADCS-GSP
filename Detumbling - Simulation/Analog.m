function volt_disc = Analog(volt_cont)
    rsltn = 256;
    maxVolt = 5;
    voltAvail = (-maxVolt:(maxVolt/rsltn):maxVolt)';
    cmprsn = zeros(length(voltAvail),3);
    volt_disc = volt_cont;
    
    for z = 1:length(voltAvail)
    cmprsn(z,1) = voltAvail(z,1) - volt_cont(1);
    cmprsn(z,2) = voltAvail(z,1) - volt_cont(2);
    cmprsn(z,3) = voltAvail(z,1) - volt_cont(3);
    end
    [indexVal1,index1] = min(abs(cmprsn(1:length(voltAvail),1)));
    [indexVal2,index2] = min(abs(cmprsn(1:length(voltAvail),2)));
    [indexVal3,index3] = min(abs(cmprsn(1:length(voltAvail),3)));

    volt_disc(1) = voltAvail(index1);
    volt_disc(2) = voltAvail(index2);
    volt_disc(3) = voltAvail(index3);
    
end
