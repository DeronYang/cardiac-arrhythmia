% files=[ 100 101 103 105 106 107 108 109 ...
%         111 112 113 114 115 116 117 118 ...
%     	119 121 122 123 124 200 201 202 203 ...
%     	205 207 208 209 210 212 213 214 215 ...
%     	217 219 220 221 222 223 228 230 231 ...
%     	232 233 234];
clear;clc;
 files=[ 100 ];
output_dir = 'ELE482';
mkdir(output_dir);

% Didn't deal with the problem of finding the QRS point. This information
% is from annotation file directly

for file_ix = 1:length(files)
    record = strcat('mitdb/',int2str(files(file_ix)));
    ann = rdann(record,'atr','start','00:01:00');
    samp_pre = rdsamp(record,'begin','00:01:00');
    
    offset = samp_pre(1,1)-1;
    samp = samp_pre(:,2)';
    
    unclassified_count = 0;
    for ann_ix = length(ann):-1:1
        pos(ann_ix) = ann(ann_ix).sampleNumber - offset;
        if strcmp(ann(ann_ix).typeMnemonic,'N')
            label(ann_ix,1) = -1;
        elseif strcmp(ann(ann_ix).typeMnemonic,'¡¤')
            label(ann_ix,1) = -1;
        elseif strcmp(ann(ann_ix).typeMnemonic,'Q')
            label(ann_ix,1) =  0;
            unclassified_count = unclassified_count+1;
        else
            label(ann_ix,1) =  1;
        end       
    end
    save(strcat(output_dir,'/file_',int2str(files(file_ix))),'samp','pos','label');
    fprintf('file %s processed, total heartbeats: %d, unclassified heartbeats: %d\n',int2str(files(file_ix)), length(ann), unclassified_count);
    clear samp pos label ann samp_pre
end