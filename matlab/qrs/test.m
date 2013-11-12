fs=360;
lp=35;
wn1=2*lp/fs;
[b1,a1] = cheby1(4,0.9,wn1);
Diff_samp = [1,2,3,4,5,6,7,8,9];
pre_data = filter(b1,a1,Diff_samp);

% a = [1,2];
% b = [2,3,4];
% x = [1,2,3];
% y = filter(b,a,x);