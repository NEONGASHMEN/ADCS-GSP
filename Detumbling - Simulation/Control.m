function muB = Control(Bfieldnav,pqrdotnav)
    Magtorquer_params
    
    k = 650;        %650

    WxB = cross(pqrdotnav,Bfieldnav);

    muB = k*WxB;
%     muB = 0.2*(WxB/norm(WxB));
    
    if sum(abs(muB)) > 0.2
        muB = (muB/norm(muB))*0.2;
    end
    
end
