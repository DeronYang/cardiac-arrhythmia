clc;clear;
samp_pre = [1.4, 2.3, 2, 3, 1, 4];
samp_length = length(samp_pre);
samp_med1 = zeros(1,samp_length);
samp_temp1 = [samp_pre(:,1)*ones(1,36) samp_pre samp_pre(:,samp_length)*ones(1,36)];
for samp_ix = 1:samp_length
        window = samp_temp1(1,samp_ix:samp_ix+72);
        samp_med1(1,samp_ix) = median(window);
end