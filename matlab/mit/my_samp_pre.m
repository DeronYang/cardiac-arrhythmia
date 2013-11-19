clear;
clc;
load my_samp_pre;
len = length(my_samp_pre);
path = 'my_samp_pre.log';
fid = fopen('my_samp_pre.log','w+');
for i=1:len
    fprintf(fid,'%d\n',my_samp_pre(i));
end
fclose(fid);