%������ʵ�ֻ���С���任��QRS���ļ��   %
%֣����  ���ڴ�ѧ��������DSPsʵ����   %
%2007��7��2��                        %

clear all;
close all;
points=4096;       level=4;    sr=360; 
%����ECG�ź�
load ecgdata.mat;
ecgdata=mydata';
plot(ecgdata(1:points));grid on;axis tight;axis([1,points,-2,5]);
title('ECG�ź�');

swa=zeros(4,points);
swd=zeros(4,points);
signal=ecgdata(0*4096+1:1*4096);

%��С��ϵ���ͳ߶�ϵ��
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
%����ԭ�źźͳ߶�ϵ����С��ϵ��
figure;
subplot(level,1,1); plot(ecgdata(1:points)); grid on;axis tight;
title('ECG�źż�����j=1,2,3,4�߶��µĳ߶�ϵ����С��ϵ��');
for i=1:level
    subplot(level+1,2,2*(i)+1);
    plot(swa(i,:)); axis tight;grid on;xlabel('time');
    ylabel(strcat('a   ',num2str(i)));
    subplot(level+1,2,2*(i)+2);
    plot(swd(i,:)); axis tight;grid on;
    ylabel(strcat('d   ',num2str(i)));
end

%����ԭͼ��С��ϵ��
figure;
subplot(level,1,1); plot(real(ecgdata(1:points)),'b'); grid on;axis tight;
title('ECG�źż�����j=1,2,3,4�߶��µ�С��ϵ��');
for i=1:level
    subplot(level+1,1,i+1);
    plot(swd(i,:),'b'); axis tight;grid on;
    ylabel(strcat('d   ',num2str(i)));
end

%**************************************����������ֵ��*****************************************%
ddw=zeros(size(swd));
pddw=ddw;
nddw=ddw;
%С��ϵ���Ĵ���0�ĵ�
posw=swd.*(swd>0);
%б�ʴ���0
pdw=((posw(:,1:points-1)-posw(:,2:points))<0);
%������ֵ��
pddw(:,2:points-1)=((pdw(:,1:points-2)-pdw(:,2:points-1))>0);
%С��ϵ��С��0�ĵ�
negw=swd.*(swd<0);
ndw=((negw(:,1:points-1)-negw(:,2:points))>0);
%������ֵ��
nddw(:,2:points-1)=((ndw(:,1:points-2)-ndw(:,2:points-1))>0);
%������
ddw=pddw|nddw;
ddw(:,1)=1;
ddw(:,points)=1;
%�����ֵ���ֵ,��������0
wpeak=ddw.*swd;
wpeak(:,1)=wpeak(:,1)+1e-10;
wpeak(:,points)=wpeak(:,points)+1e-10;

%�������߶��¼�ֵ��
figure;
for i=1:level
    subplot(level,1,i);
    plot(wpeak(i,:)); axis tight;grid on;
ylabel(strcat('j=   ',num2str(i)));
end
subplot(4,1,1);
title('ECG�ź���j=1,2,3,4�߶��µ�С��ϵ����ģ����ֵ��');

interva2=zeros(1,points);
intervaqs=zeros(1,points);
Mj1=wpeak(1,:);
Mj4=wpeak(3,:);

%�����߶�3��ֵ��
figure;
plot (Mj4);
title('�߶�3��С��ϵ����ģ����ֵ��');

posi=Mj4.*(Mj4>0);
%��������ֵ��ƽ��
thposi=(max(posi(1:round(points/4)))+max(posi(round(points/4):2*round(points/4)))+max(posi(2*round(points/4):3*round(points/4)))+max(posi(3*round(points/4):4*round(points/4))))/4;
posi=(posi>thposi/3);
nega=Mj4.*(Mj4<0);
%�󸺼���ֵ��ƽ��
thnega=(min(nega(1:round(points/4)))+min(nega(round(points/4):2*round(points/4)))+min(nega(2*round(points/4):3*round(points/4)))+min(nega(3*round(points/4):4*round(points/4))))/4;
nega=-1*(nega<thnega/4);
%�ҳ���0��
interva=posi+nega;
loca=find(interva);
for i=1:length(loca)-1
    if abs(loca(i)-loca(i+1))<80
       diff(i)=interva(loca(i))-interva(loca(i+1));
    else
       diff(i)=0;
    end
end
%�ҳ���ֵ��
loca2=find(diff==-2);
%������ֵ��
interva2(loca(loca2(1:length(loca2))))=interva(loca(loca2(1:length(loca2))));
%������ֵ��
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
%*************************��������ֵ�Թ��㣬��R����ֵ��������QRS����㼰�յ�*******************%
while i<points
    if interva2(i)==-1
       mark1=i;
       i=i+1;
       while(i<points&interva2(i)==0)
          i=i+1;
       end
       mark2=i;
%�󼫴�ֵ�ԵĹ����
       mark3= round((abs(Mj4(mark2))*mark1+mark2*abs(Mj4(mark1)))/(abs(Mj4(mark2))+abs(Mj4(mark1))));
%R������ֵ��
       R_result(j)=mark3-10;
       count(mark3-10)=1;
%���QRS�����
       kqs=mark3-10;
       markq=0;
     while (kqs>1)&&( markq< 3)
         if Mj1(kqs)~=0
            markq=markq+1;
         end
         kqs= kqs -1;
     end
  count2(kqs)=-1;
  
%���QRS���յ�  
  kqs=mark3-10;
  marks=0;
  while (kqs<points)&&( marks<2)
      if Mj1(kqs)~=0
         marks=marks+1;
      end
      kqs= kqs+1;
  end
  count3(kqs)=-1;
  i=i+60;
  j=j+1;
  Rnum=Rnum+1;
 end
i=i+1;
end
%************************ɾ�����㣬����©���**************************%
num2=1;
while(num2~=0)
   num2=0;
%j=3,�����
   R=find(count);
%�������
   R_R=R(2:length(R))-R(1:length(R)-1);
   RRmean=mean(R_R);
%������R�����С��0.4RRmeanʱ,ȥ��ֵС��R��
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
%������R���������1.6RRmeanʱ,��С��ֵ,����һ�μ��R��
for i=2:length(R)
    if (R(i)-R(i-1))>1.6*RRmean
        Mjadjust=wpeak(4,R(i-1)+80:R(i)-80);
        points2=(R(i)-80)-(R(i-1)+80)+1;
%��������ֵ��
        adjustposi=Mjadjust.*(Mjadjust>0);
        adjustposi=(adjustposi>thposi/4);
%�󸺼���ֵ��
        adjustnega=Mjadjust.*(Mjadjust<0);
        adjustnega=-1*(adjustnega<thnega/5);
%������
        interva4=adjustposi+adjustnega;
%�ҳ���0��
        loca3=find(interva4);
        diff2=interva4(loca3(1:length(loca3)-1))-interva4(loca3(2:length(loca3)));
%����м���ֵ��,�ҳ�����ֵ��
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
            while(j<points2&interva3(j)==0)
                 j=j+1;
            end
            mark5=j;
%������
            mark6= round((abs(Mjadjust(mark5))*mark4+mark5*abs(Mjadjust(mark4)))/(abs(Mjadjust(mark5))+abs(Mjadjust(mark4))));
            count(R(i-1)+80+mark6-10)=1;
            j=j+60;
         end
         j=j+1;
     end
    end
 end
end
%����ԭͼ����������
figure;
plot(ecgdata(0*4096+1:1*4096)),grid on,axis tight,axis([1,points,-2,5]);
title('ECG�źŵ�R����ֵ��QRS������');
hold on
plot(count,'r');
plot(count2,'k');
plot(count3,'k');
for i=1:Rnum
    if R_result(i)==0;
        break
    end
    plot(R_result(i),ecgdata(R_result(i)),'bo','MarkerSize',10,'MarkerEdgeColor','g');
end
hold off
