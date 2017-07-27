R=150000;
C=2.2e-6;
Rv= R * (1+ 0.01*randn(10000,1));
Cv= C * (1+ 0.1*randn(10000,1));


Fc= 1./(2*pi*Rv.*Cv);

mean(Fc)
std(Fc)

figure; hist(Fc,100)
title(['R = ' num2str(R) 'and C= ' num2str(C)]);


R=1.5e6;
C=1e-6;
Rv= R * (1+ 0.001*randn(10000,1));
Cv= C * (1+ 0.05*randn(10000,1));

Fc= 1./(2*pi*Rv.*Cv);


mean(Fc)
std(Fc)

figure; hist(Fc,100)
title(['R = ' num2str(R) 'and C= ' num2str(C)]);