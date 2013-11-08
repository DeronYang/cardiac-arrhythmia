function [orig_r_posi,orig_r_height]=QRSDector(f)
n=zeros(size(f));
for i=6:size(f,1)-5
    n(i)=2*f(i)-f(i+5)-f(i-5);
end
nmax=max(n)*0.698;
num=0;
for i=6:size(f,1)-5
    if n(i)>=nmax
        num=num+1;
    end
end
k=zeros(num,1);
j=1;
for i=6:size(f,1)-5
    if n(i)>=nmax
        k(j)=i;
        j=j+1;
    end
end
nume=ceil(num/3);
j=1;
orig_r_posi=zeros(nume,1);
orig_r_height=zeros(nume,1);
for i=2:num-1
    if (k(i+1)-k(i))<=5 && (k(i)-k(i-1))<=5
         orig_r_posi(j)=k(i);
         orig_r_height(j)=f(k(i));
         j=j+1;
    end
end