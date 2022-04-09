function [B_result,pqrdot_result] = Navigation(InpB,Inppqrdot)
    global Bfieldnavprev pqrdotnavprev
    
    s = 0.3;         %%How much we believe in our measurement
    
    if sum(Bfieldnavprev) + sum(pqrdotnavprev) == 0
        B_result = InpB;
        pqrdot_result = Inppqrdot;
   
    else
        Bbiasestimate = [0;0;0];
        pqrdot_biasestimate = [0;0;0];
        B_result = Bfieldnavprev*(1-s) + s*(InpB - Bbiasestimate);
        pqrdot_result = pqrdotnavprev*(1-s) + s*(Inppqrdot - pqrdot_biasestimate);
    end
    
     Bfieldnavprev = B_result;
     pqrdotnavprev = pqrdot_result;
    
end
    
    
    