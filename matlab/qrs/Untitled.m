load orig_samp
fid = fopen('orig_samp.log','w');
orig_samp = orig_samp(1:5);
samp_length = length(orig_samp);
median_flt_samp_200ms = medfilt1(orig_samp, 5);
