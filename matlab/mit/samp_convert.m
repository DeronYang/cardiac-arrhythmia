clear;clc;
load samp_pre105.mat;
a = samp_pre(:,2);
m=length(a);
try 
   
    fid = fopen('samp_pre105.log','w'); 
    if(fid~=-1) 
       
        for k=1:m 
           fprintf(fid,'%d',a(k)); 
            fprintf(fid,'\n'); 
        end 
        fclose(fid); 
    else 
        msgbox('�޷������ļ�!'); 
    end 
catch 
   msgbox('�ļ���������г���!','������...','error'); 
end



