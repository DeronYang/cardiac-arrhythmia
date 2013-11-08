clear;
data_dir = 'extract';
mkdir(data_dir);

% files=[ 100 101 103 105 106 107 108 109 ...
%         111 112 113 114 115 116 117 118 ...
%     	119 121 122 123 124 200 201 202 203 ...
%     	205 207 208 209 210 212 213 214 215 ...
%     	217 219 220 221 222 223 228 230 231 ...
%     	232 233 234];
files=[ 105 ];
% Didn't deal with the problem of finding the QRS point. This information
% is from annotation file directly

for file_ix = 1:length(files)
    fprintf('Processing file %s\n',int2str(files(file_ix)));
    record = strcat('mitdb/',int2str(files(file_ix)));
    ann = rdann(record,'atr','start','00:01:00');
    samp_pre = rdsamp(record,'begin','00:01:00');
    save samp_pre105.mat samp _pre;
    offset = samp_pre(1,1)-1;
    samp_pre = samp_pre(:,[2,3])';
    samp_length = size(samp_pre,2);
    ann_length = length(ann);
    
    lead = 1;   % only process the 1st lead
    
    % Preprocessing phase

    % median filter of 200ms width to remove QRS complexes and P-waves
    % 200ms = 73 samples (200*0.360 + 1)    
    samp_med1 = zeros(2,samp_length);
    samp_temp1 = [samp_pre(:,1)*ones(1,36) samp_pre samp_pre(:,samp_length)*ones(1,36)];
    for samp_ix = 1:samp_length
        window = samp_temp1(lead,samp_ix:samp_ix+72);
        samp_med1(lead,samp_ix) = median(window);
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
%     clear feature label DataMtx;
end

%train_files = [101 106 108 109 112 114 115 116 118 119 122 124 201 203 205 207 208 209 215 220 223 230];
%test_files  = [100 103 105 111 113 117 121 123 200 202 210 212 213 214 219 221 222 228 231 232 233 234];

%% Training Phase
%FeatureMtx_train = [];    label_train = [];
%for k = 1:length(train_files)
%    tic;
%    load(strcat(data_dir,'/data_',int2str(train_files(k))));
%    FeatureMtx_train = [FeatureMtx_train; DataMtx];
%    label_train = [label_train; label];
%    fprintf('%d/%d processed in %f seconds\n',k,length(train_files),toc);
%end
%nz_indices = find(label_train);
%FeatureMtx_train = FeatureMtx_train(nz_indices,:);
%label_train = label_train(nz_indices,:);
%
%tic
%smo_opts = svmsmoset('MaxIter',500000);
%SVMStruct = svmtrain(FeatureMtx_train, label_train, 'Kernel_Function', 'rbf', 'rbf_sigma', 2.5, 'BoxConstraint', ...
%    10, 'Method', 'SMO', 'SMO_Opts', smo_opts);
%fprintf('SVM training completed in %f seconds\n',toc);
%
%% Testing Phase
%FeatureMtx_test = [];    label_test = [];
%for k = 1:length(test_files)
%    tic;
%    load(strcat(data_dir,'/data_',int2str(test_files(k))));
%    FeatureMtx_test = [FeatureMtx_test; DataMtx];
%    label_test = [label_test; label];
%    fprintf('%d/%d processed in %f seconds\n',k,length(test_files),toc);
%end
%nz_indices = find(label_test);
%FeatureMtx_test = FeatureMtx_test(nz_indices,:);
%label_test = label_test(nz_indices,:);
%
%tic
%prediction = svmclassify(SVMStruct,FeatureMtx_test);
%fprintf('SVM testing completed in %f seconds\n',toc);
%result = compare(prediction,label_test)