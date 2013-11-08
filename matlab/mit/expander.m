function [A B] = expander(n,d)

if (n<=0)||(d<0)
    error('invalid input in expander.m\n');
end
if (n>1)&&(d>0)
    [A1 B1] = expander(n-1,d);
    [A2 B2] = expander(n,d-1);
    A = [A1 zeros(size(A1,1),1); A2(:,1:n-1) 1+A2(:,n)];
    B = [B1; B2.*d./(1+A2(:,n))];
else
    B = 1;
    if n==1
        A = d;
    else
        A = zeros(1,n);
     end
end
    