clear;clc;
filepath='ecg.log';
A=load(filepath);
points=4096;       level=4;    sr=360; 
%读入ECG信号
offset = 0;
% ecgdata=A(:,2)'/1000;
ecgdata=A(:,2)'/ 128 + 1024;
%只取一部分，看着快。
%ecgdata = ecgdata(1:2000);
ecgdata = round(ecgdata);
ecgdata_upsamp =[];
index = 1;
len = length(ecgdata);
% 将250Hz数据上采样为360Hz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
while (index + 24) <= len
    upsamp_out = my_upsamp(ecgdata(index:(index+24)));
    ecgdata_upsamp = [ecgdata_upsamp upsamp_out];
    index = index + 25;
end
    in_temp = zeros(1,25);
    tmp = ecgdata(index:len);
    len = length(tmp);
    in_temp(1:len) = tmp;
    upsamp_out = my_upsamp(in_temp);
    ecgdata_upsamp = [ecgdata_upsamp upsamp_out];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ecgdata_upsamp = round(ecgdata_upsamp);
x = find(ecgdata_upsamp == 0);
ecgdata = ecgdata_upsamp;clear ecgdata_upsamp;
len = length(ecgdata);
points = len;
% ecgdata=ecgdata(offset+1:offset+points);
% plot(ecgdata(offset+1:offset+points));grid on;axis tight;%axis([1,points,-2,5]);
% title('ECG信号');

swa=zeros(4,points);
swd=zeros(4,points);
signal=ecgdata(1:1*points);

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
subplot(level,1,1); plot(ecgdata(1:points)); grid on;axis tight;
title('ECG信号及其在j=1,2,3,4尺度下的尺度系数及小波系数');
for i=1:level
    subplot(level+1,2,2*(i)+1);
    plot(swa(i,:)); axis tight;grid on;xlabel('time');
    ylabel(strcat('a   ',num2str(i)));
    subplot(level+1,2,2*(i)+2);
    plot(swd(i,:)); axis tight;grid on;
    ylabel(strcat('d   ',num2str(i)));
end

%画出原图及小波系数
figure;
subplot(level,1,1); plot(real(ecgdata(1:points)),'b'); grid on;axis tight;
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
thposi=(max(posi(1:round(points/4))) + max(posi(round(points/4):2*round(points/4))) + max(posi(2*round(points/4):3*round(points/4))) + max(posi(3*round(points/4):points)))/4;
posi=(posi>thposi/3);
nega=Mj4.*(Mj4<0);
%求负极大值的平均
thnega=(min(nega(1:round(points/4)))+min(nega(round(points/4):2*round(points/4)))+min(nega(2*round(points/4):3*round(points/4)))+min(nega(3*round(points/4):points)))/4;
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
       while(i<points&interva2(i)==0)
          i=i+1;
       end
       mark2=i;
%求极大值对的过零点
       mark3= round((abs(Mj4(mark2))*mark1+mark2*abs(Mj4(mark1)))/(abs(Mj4(mark2))+abs(Mj4(mark1))));
%R波极大值点
       R_result(j)=mark3-10;
       count(mark3-10)=1;
%求出QRS波起点
       kqs=mark3-10;
       markq=0;
     while (kqs>1)&&( markq< 3)
         if Mj1(kqs)~=0
            markq=markq+1;
         end
         kqs= kqs -1;
     end
  count2(kqs)=-1;
  
%求出QRS波终点  
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
            while(j<points2&interva3(j)==0)
                 j=j+1;
            end
            mark5=j;
%求过零点
            mark6= round((abs(Mjadjust(mark5))*mark4+mark5*abs(Mjadjust(mark4)))/(abs(Mjadjust(mark5))+abs(Mjadjust(mark4))));
            count(R(i-1)+80+mark6-10)=1;
            j=j+60;
         end
         j=j+1;
     end
    end
 end
end
%画出原图及标出检测结果
figure;
plot(ecgdata),grid on,axis tight;%axis([1,points,-2,5]);
title('ECG信号的R波峰值及QRS波波段');
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


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract begin
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% data_dir = 'extract';
% mkdir(data_dir);
% files=[ 100 101 103 105 106 107 108 109 ...
%         111 112 113 114 115 116 117 118 ...
%     	119 121 122 123 124 200 201 202 203 ...
%     	205 207 208 209 210 212 213 214 215 ...
%     	217 219 220 221 222 223 228 230 231 ...
%     	232 233 234];
% files=[ 100 ];
% % Didn't deal with the problem of finding the QRS point. This information
% % is from annotation file directly

samp_pre = ecgdata;
samp_length = length(ecgdata);
samp_med1 = zeros(1,samp_length);
samp_temp1 = [samp_pre(1)*ones(1,36) samp_pre samp_pre(samp_length)*ones(1,36)];
    for samp_ix = 1:samp_length
        window = samp_temp1(samp_ix:samp_ix+72);
        samp_med1(samp_ix) = median(window);
    end
    
samp_med2 = zeros(1,samp_length);
samp_temp2 = [samp_med1(:,1)*ones(1,108) samp_med1 samp_med1(:,samp_length)*ones(1,108)];
    for samp_ix = 1:samp_length
        window = samp_temp2(samp_ix:samp_ix+216);
        samp_med2(samp_ix) = median(window);
    end
%baseline corrected ECG signal
samp_cor = samp_pre - samp_med2;
    
clear samp_temp1 samp_med1 samp_temp2 samp_med2 window samp_pre;
    
%remove power-line and high-frequency noise with low pass filter
fir_coeff = fir1(11,35/180);    % 12-tap low pass filter with cut-off frequency 35Hz/180Hz
samp_filt = zeros(1,samp_length);

temp = conv(samp_cor,fir_coeff);
samp_filt = temp(1:samp_length);
clear temp samp_cor;
% for file_ix = 1:length(files)
%     fprintf('Processing file %s\n',int2str(files(file_ix)));
%     record = strcat('mitdb/',int2str(files(file_ix)));
%     ann = rdann(record,'atr','start','00:01:00');
%     samp_pre = rdsamp(record,'begin','00:01:00');
%     
%     offset = samp_pre(1,1)-1;
%     samp_pre = samp_pre(:,[2,3])';
%     samp_length = size(samp_pre,2);
%     ann_length = length(ann);
%     
%     lead = 1;   % only process the 1st lead
%     
% %     Preprocessing phase
% 
% %     median filter of 200ms width to remove QRS complexes and P-waves
% %     200ms = 73 samples (200*0.360 + 1)    
%     samp_med1 = zeros(2,samp_length);
%     samp_temp1 = [samp_pre(:,1)*ones(1,36) samp_pre samp_pre(:,samp_length)*ones(1,36)];
%     for samp_ix = 1:samp_length
%         window = samp_temp1(lead,samp_ix:samp_ix+72);
%         samp_med1(lead,samp_ix) = median(window);
%     end

% %     median filter of 600ms width to remove T-waves
% %     600ms = 217 samples (600*0.360 + 1)    
%     samp_med2 = zeros(2,samp_length);
%     samp_temp2 = [samp_med1(:,1)*ones(1,108) samp_med1 samp_med1(:,samp_length)*ones(1,108)];
%     for samp_ix = 1:samp_length
%         window = samp_temp2(lead,samp_ix:samp_ix+216);
%         samp_med2(lead,samp_ix) = median(window);
%     end
    
% %     baseline corrected ECG signal
%     samp_cor = samp_pre - samp_med2;
%     
%     clear samp_temp1 samp_med1 samp_temp2 samp_med2 window samp_pre
%     
% %     remove power-line and high-frequency noise with low pass filter
%     fir_coeff = fir1(11,35/180);    % 12-tap low pass filter with cut-off frequency 35Hz/180Hz
%     samp_filt = zeros(2,samp_length);
% 
%     temp = conv(samp_cor(lead,:),fir_coeff);
%     samp_filt(lead,:) = temp(1:samp_length);
%     
%     clear temp samp_cor
    

R_length = length(R_result);
%     Feature extraction phase
    for ann_ix = R_length-5:-1:6
        rec_ix = ann_ix-5;
        
% % %         RR interval features
        feature(rec_ix).preRR = (R_result(ann_ix) - R_result(ann_ix-1)) / 360;
        feature(rec_ix).postRR = (R_result(ann_ix+1) - R_result(ann_ix)) / 360;
        feature(rec_ix).localAvgRR = (R_result(ann_ix+5) - R_result(ann_ix-5))/10 / 360;
        
% % %         Morphology features
        fiducial_ix = R_result(ann_ix);
        orig_samp_freq = 360;    % if want to change sampling frequency, should also manipulate the above codes!
 
% % %         QRS feature
        window_samp_freq = 60;
        time_ix = fiducial_ix-round(0.050*orig_samp_freq):round(orig_samp_freq/window_samp_freq):fiducial_ix+round(0.100*orig_samp_freq);
        feature(rec_ix).morphQRS = samp_filt(time_ix);

% % %        T-wave feature
        window_samp_freq = 20;
        time_ix = fiducial_ix+round(0.150*orig_samp_freq):round(orig_samp_freq/window_samp_freq):fiducial_ix+round(0.500*orig_samp_freq);
        feature(rec_ix).morphT = samp_filt(time_ix);
        
% % %         label
% % %         if strcmp(ann(ann_ix).typeMnemonic,'N')
% % %             label(rec_ix,1) = -1;
% % %         elseif strcmp(ann(ann_ix).typeMnemonic,'·')
% % %             label(rec_ix,1) = -1;
% % %         elseif strcmp(ann(ann_ix).typeMnemonic,'Q')
% % %             label(rec_ix,1) =  0;
% % %         else
% % %             label(rec_ix,1) =  1;
% % %         end
        
% % %         DataMtx
        DataMtx(rec_ix,:) = [feature(rec_ix).preRR feature(rec_ix).postRR feature(rec_ix).localAvgRR...
                             feature(rec_ix).morphQRS feature(rec_ix).morphT];
    end
   
    save('my_log','DataMtx');
    clear feature DataMtx;


