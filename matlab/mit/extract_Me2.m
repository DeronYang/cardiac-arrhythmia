
clear;
data_dir = 'extract';
mkdir(data_dir);

files=[ 105 ];

for file_ix = 1:length(files)
    fprintf('Processing file %s\n',int2str(files(file_ix)));
%     record = strcat('mitdb/',int2str(files(file_ix)));
%     ann = rdann(record,'atr','start','00:01:00');
%     samp_pre = rdsamp(record,'begin','00:01:00');
%     save samp_pre105.mat samp_pre;
%     save ann105.mat ann;
    load samp_pre105.mat;
    load ann105.mat;
    offset = samp_pre(1,1)-1;
    samp_pre = samp_pre(:,[2,3])';
%     samp_length = size(samp_pre,2);
    samp_length = 100;
    ann_length = length(ann);

    lead = 1;   % only process the 1st lead
    
    % Preprocessing phase

    % median filter of 200ms width to remove QRS complexes and P-waves
    % 200ms = 73 samples (200*0.360 + 1)    
    samp_med1 = zeros(2,samp_length);
    samp_temp1 = [samp_pre(:,1)*ones(1,36) samp_pre samp_pre(:,samp_length)*ones(1,36)];
    for samp_ix = 1:samp_length
        window = samp_temp1(lead,samp_ix:samp_ix+72);
        samp_med1(lead,samp_ix) = median(window)
    end

    % median filter of 600ms width to remove T-waves
    % 600ms = 217 samples (600*0.360 + 1)    
    samp_med2 = zeros(2,samp_length);
    samp_temp2 = [samp_med1(:,1)*ones(1,108) samp_med1 samp_med1(:,samp_length)*ones(1,108)];
    for samp_ix = 1:samp_length
        window = samp_temp2(lead,samp_ix:samp_ix+216);
        samp_med2(lead,samp_ix) = median(window);
    end
    
    % baseline corrected ECG signal
    samp_cor = samp_pre - samp_med2;

    clear samp_temp1 samp_med1 samp_temp2 samp_med2 window samp_pre

    % remove power-line and high-frequency noise with low pass filter
    fir_coeff = fir1(11,35/180);    % 12-tap low pass filter with cut-off frequency 35Hz/180Hz
    samp_filt = zeros(2,samp_length);

    temp = conv(samp_cor(lead,:),fir_coeff); 
    samp_filt(lead,:) = temp(1:samp_length);

    clear temp samp_cor
    % Feature extraction phase
    for ann_ix = ann_length-5:-1:6
        rec_ix = ann_ix-5;

        % RR interval features
        feature(rec_ix).preRR = ann(ann_ix).timeInSeconds - ann(ann_ix-1).timeInSeconds;
        feature(rec_ix).postRR = ann(ann_ix+1).timeInSeconds - ann(ann_ix).timeInSeconds;
        feature(rec_ix).localAvgRR = (ann(ann_ix+5).timeInSeconds - ann(ann_ix-5).timeInSeconds)/10;

        % Morphology features
        fiducial_ix = ann(ann_ix).sampleNumber - offset;
        orig_samp_freq = 360;    % if want to change sampling frequency, should also manipulate the above codes!

        % QRS feature
        window_samp_freq = 60;
        time_ix = fiducial_ix-round(0.050*orig_samp_freq):round(orig_samp_freq/window_samp_freq):fiducial_ix+round(0.100*orig_samp_freq);
        feature(rec_ix).morphQRS = samp_filt(lead,time_ix);

        % T-wave feature
        window_samp_freq = 20;
        time_ix = fiducial_ix+round(0.150*orig_samp_freq):round(orig_samp_freq/window_samp_freq):fiducial_ix+round(0.500*orig_samp_freq);
        feature(rec_ix).morphT = samp_filt(lead,time_ix);

        % label
        if strcmp(ann(ann_ix).typeMnemonic,'N')
            label(rec_ix,1) = -1;
        elseif strcmp(ann(ann_ix).typeMnemonic,'¡¤')
            label(rec_ix,1) = -1;
        elseif strcmp(ann(ann_ix).typeMnemonic,'Q')
            label(rec_ix,1) =  0;
        else
            label(rec_ix,1) =  1;
        end
        
    % DataMtx
    DataMtx(rec_ix,:) = [feature(rec_ix).preRR feature(rec_ix).postRR feature(rec_ix).localAvgRR...
                         feature(rec_ix).morphQRS feature(rec_ix).morphT];
    end
    save(strcat(data_dir,'/data_',int2str(files(file_ix))),'DataMtx','label');
end