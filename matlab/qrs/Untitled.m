clear;clc;
load orig_posi
fid = fopen('orig_posi.log','w');
len = length(orig_posi);
 for k=1:len 
    fprintf(fid,'%d\n',orig_posi(k)); 
end 
fclose(fid);
