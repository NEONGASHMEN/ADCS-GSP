function [Bfield,pqrdot] = Sensor(Bfield,pqrdot)
for i = 1:3
    Sensor_params;
    
    Bfield(i) = Bfield(i) + Magsnsr_bias + Magsnsr_noise;
    pqrdot(i) = pqrdot(i) + Angsnsr_bias + Angsnsr_noise;
    
end
end
