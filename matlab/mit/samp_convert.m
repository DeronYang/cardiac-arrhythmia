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
        msgbox('无法保存文件!'); 
    end 
catch 
   msgbox('文件保存过程中出错!','出错了...','error'); 
end



