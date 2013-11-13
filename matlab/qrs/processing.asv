clear;clc;
load file_01.mat
orig_samp = samp(1:7200);
save orig_samp.mat orig_samp
orig_points = length(orig_samp);
median_flt_samp_200ms = medfilt1(orig_samp, 72);
median_flt_samp_600ms = medfilt1(median_flt_samp_200ms, 216);
Diff_samp = orig_samp-median_flt_samp_600ms;
% plot(Diff_samp, 'DisplayName', 'Diff_samp', 'YDataSource', 'Diff_samp'); figure(gcf)
t= 0:orig_points-1;
% plot(t, Diff_samp,t,orig_samp)
fs=360;
lp=35;
wn1=2*lp/fs;
[b1,a1] = cheby1(4,0.9,wn1);
pre_data = filter(b1,a1,Diff_samp);
pre_data = pre_data';
% plot(pre_data, 'DisplayName', 'pre_data', 'YDataSource', 'pre_data'); figure(gcf)
% plot(t, pre_data,t,orig_samp)
% plot(t, pre_data,t,Diff_samp)
% plot(t, pre_data,t, Diff_samp)

% load R position And caculate RR-interval
orig_posi = pos(1:2067);
save orig_posi.mat orig_posi
rr_interval = zeros(1,2067);
for i=1:2066
    rr_interval(1,i) = (orig_posi(1,i+1)-orig_posi(1,i))/360;
end

% load label text 
orig_label = label(1:2067);
save orig_label.mat orig_label
% R wave detection（R波检测代码，离散小波变换）
% [orig_r_posi,orig_r_height] = QRSDetector(pre_data(10:1080));
% subplot(2,1,2);plot(pre_data);hold on;plot(orig_r_posi,orig_r_height,'rp');title('R波峰检测'); 

points=length(pre_data);   level=4;    sr=360; 
swa=zeros(4,points);
swd=zeros(4,points);
signal=pre_data;
%算小波系数和尺度系数
for i=1:points-3
  swa(1,i+3)=1/4*signal(i+3-2^0*0)+3/4*signal(i+3-2^0*1)+3/4*signal(i+3-2^0*2)+1/4*signal(i+3-2^0*3);
  swd(1,i+3)=-1/4*signal(i+3-2^0*0)-3/4*signal(i+3-2^0*1)+3/4*signal(i+3-2^0*2)+1/4*signal(i+3-2^0*3);
end
j=2;
while j<=level
   for i=1:points-24
     swa(j,i+24)=1/4*swa(j-1,i+24-2^(j-1)*0)+3/4*swa(j-1,i+24-2^(j-1)*1)+3/4*swa(j-1,i+24-2^(j-1)*2)+1/4*swa(j-1,i+24-2^(j-1)*3);
     swd(j,i+24)=-1/4*swa(j-1,i+24-2^(j-1)*0)-3/4*swa(j-1,i+24-2^(j-1)*1)+3/4*swa(j-1,i+24-2^(j-1)*2)+1/4*swa(j-1,i+24-2^(j-1)*3);
   end
   j=j+1;
end
%画出原信号和尺度系数，小波系数
figure;
subplot(level,1,1); plot(pre_data(1:points)); grid on;axis tight;
title('ECG信号及其在j=1,2,3,4尺度下的尺度系数及小波系数');
for i=1:level
    subplot(level+1,2,2*(i)+1);
    plot(swa(i,:)); axis tight;grid on;xlabel('time');
    ylabel(strcat('a ',num2str(i)));
    subplot(level+1,2,2*(i)+2);
    plot(swd(i,:)); axis tight;grid on;
    ylabel(strcat('d ',num2str(i)));
end

%画出原图及小波系数
figure;
subplot(level,1,1); plot(real(pre_data(1:points)),'b'); grid on;axis tight;
title('ECG信号及其在j=1,2,3,4尺度下的小波系数');
for i=1:level
    subplot(level+1,1,i+1);
    plot(swd(i,:),'b'); axis tight;grid on;
    ylabel(strcat('d   ',num2str(i)));
end

%**************************************求正负极大值对*****************************************%
ddw=zeros(size(swd));
pddw=ddw;
nddw=ddw;
%小波系数的大于0的点
posw=swd.*(swd>0);
%斜率大于0
pdw=((posw(:,1:points-1)-posw(:,2:points))<0);
%正极大值点
pddw(:,2:points-1)=((pdw(:,1:points-2)-pdw(:,2:points-1))>0);
%小波系数小于0的点
negw=swd.*(swd<0);
ndw=((negw(:,1:points-1)-negw(:,2:points))>0);
%负极大值点
nddw(:,2:points-1)=((ndw(:,1:points-2)-ndw(:,2:points-1))>0);
%或运算
ddw=pddw|nddw;
ddw(:,1)=1;
ddw(:,points)=1;
%求出极值点的值,其他点置0
wpeak=ddw.*swd;
wpeak(:,1)=wpeak(:,1)+1e-10;
wpeak(:,points)=wpeak(:,points)+1e-10;

%画出各尺度下极值点
figure;
for i=1:level
    subplot(level,1,i);
    plot(wpeak(i,:)); axis tight;grid on;
ylabel(strcat('j=   ',num2str(i)));
end
subplot(4,1,1);
title('ECG信号在j=1,2,3,4尺度下的小波系数的模极大值点');

interva2=zeros(1,points);
intervaqs=zeros(1,points);
Mj1=wpeak(1,:);
Mj4=wpeak(3,:);

%画出尺度3极值点
figure;
plot (Mj4);
title('尺度3下小波系数的模极大值点');

posi=Mj4.*(Mj4>0);
%求正极大值的平均
thposi=(max(posi(1:round(points/4)))+max(posi(round(points/4):2*round(points/4)))+max(posi(2*round(points/4):3*round(points/4)))+max(posi(3*round(points/4):4*round(points/4))))/4;
posi=(posi>thposi/3);
nega=Mj4.*(Mj4<0);
%求负极大值的平均
thnega=(min(nega(1:round(points/4)))+min(nega(round(points/4):2*round(points/4)))+min(nega(2*round(points/4):3*round(points/4)))+min(nega(3*round(points/4):4*round(points/4))))/4;
nega=-1*(nega<thnega/4);
%找出非0点
interva=posi+nega;
loca=find(interva);
for i=1:length(loca)-1
    if abs(loca(i)-loca(i+1))<80
       diff(i)=interva(loca(i))-interva(loca(i+1));
    else
       diff(i)=0;
    end
end
%找出极值对
loca2=find(diff==-2);
%负极大值点
interva2(loca(loca2(1:length(loca2))))=interva(loca(loca2(1:length(loca2))));
%正极大值点
interva2(loca(loca2(1:length(loca2))+1))=interva(loca(loca2(1:length(loca2))+1));
intervaqs(1:points-10)=interva2(11:points);
count=zeros(1,1);
count2=zeros(1,1);
count3=zeros(1,1);
mark1=0;
mark2=0;
mark3=0;
i=1;
j=1;
Rnum=0;
%*************************求正负极值对过零，即R波峰值，并检测出QRS波起点及终点*******************%
while i<points
    if interva2(i)==-1
       mark1=i;
       i=i+1;
       while(i<points&&interva2(i)==0)
          i=i+1;
       end
       mark2=i;
%求极大值对的过零点
       mark3= round((abs(Mj4(mark2))*mark1+mark2*abs(Mj4(mark1)))/(abs(Mj4(mark2))+abs(Mj4(mark1))));
%R波极大值点
       R_result(j)=mark3-10;
       count(mark3-10)=300;
  i=i+60;
  j=j+1;
  Rnum=Rnum+1;
 end
i=i+1;
end
%************************删除多检点，补偿漏检点**************************%
num2=1;
while(num2~=0)
   num2=0;
%j=3,过零点
   R=find(count);
%过零点间隔
   R_R=R(2:length(R))-R(1:length(R)-1);
   RRmean=mean(R_R);
%当两个R波间隔小于0.4RRmean时,去掉值小的R波
for i=2:length(R)
    if (R(i)-R(i-1))<=0.4*RRmean
        num2=num2+1;
        if signal(R(i))>signal(R(i-1))
            count(R(i-1))=0;
        else
            count(R(i))=0;
        end
    end
end
end

num1=2;
while(num1>0)
   num1=num1-1;
   R=find(count);
   R_R=R(2:length(R))-R(1:length(R)-1);
   RRmean=mean(R_R);
%当发现R波间隔大于1.6RRmean时,减小阈值,在这一段检测R波
for i=2:length(R)
    if (R(i)-R(i-1))>1.6*RRmean
        Mjadjust=wpeak(4,R(i-1)+80:R(i)-80);
        points2=(R(i)-80)-(R(i-1)+80)+1;
%求正极大值点
        adjustposi=Mjadjust.*(Mjadjust>0);
        adjustposi=(adjustposi>thposi/4);
%求负极大值点
        adjustnega=Mjadjust.*(Mjadjust<0);
        adjustnega=-1*(adjustnega<thnega/5);
%或运算
        interva4=adjustposi+adjustnega;
%找出非0点
        loca3=find(interva4);
        diff2=interva4(loca3(1:length(loca3)-1))-interva4(loca3(2:length(loca3)));
%如果有极大值对,找出极大值对
        loca4=find(diff2==-2);
        interva3=zeros(points2,1)';
        for j=1:length(loca4)
           interva3(loca3(loca4(j)))=interva4(loca3(loca4(j)));
           interva3(loca3(loca4(j)+1))=interva4(loca3(loca4(j)+1));
        end
        mark4=0;
        mark5=0;
        mark6=0;
    while j<points2
         if interva3(j)==-1;
            mark4=j;
            j=j+1;
            while(j<points2&&interva3(j)==0)
                 j=j+1;
            end
            mark5=j;
%求过零点
            mark6= round((abs(Mjadjust(mark5))*mark4+mark5*abs(Mjadjust(mark4)))/(abs(Mjadjust(mark5))+abs(Mjadjust(mark4))));
            count(R(i-1)+80+mark6-10)=400;
            j=j+60;
         end
         j=j+1;
     end
    end
 end
end
%画出原图及标出检测结果
figure;
plot(pre_data(1:points)),grid on,axis tight,axis([1,points,-100,400]);
title('ECG信号的R波峰值及QRS波波段');
hold on
plot(count,'r');
for i=1:Rnum
    if R_result(i)==0;
        break
    end
    plot(R_result(i),pre_data(R_result(i)),'bo','MarkerSize',10,'MarkerEdgeColor','g');
end
hold off