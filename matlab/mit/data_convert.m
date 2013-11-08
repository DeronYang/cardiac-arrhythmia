clc;clear;
load('data_105.mat');
try 
B = DataMtx;
    
    [m,n] = size(B); 
    [filename, pathname] = uiputfile({'*.txt;*.dat' ,'数据文件(*.txt;*.dat)';'*.*','所有文件 (*.*)'},'保存数据文件'); 
    fid = fopen([pathname filename],'w'); 
    if(fid~=-1) 

        for k=1:m 

            for kk = 1:12 
                fprintf(fid,'\t%d',kk); 
                fprintf(fid,':'); 
                fprintf(fid,'%d',B(k,kk)); 
            end 
            fprintf(fid,'\n'); 
        end 
        fclose(fid); 
    else 
        msgbox('无法保存文件!'); 
    end 
catch 
   msgbox('文件保存过程中出错!','出错了...','error'); 
end


%  [filename, pathname] = uiputfile({'*.txt;*.dat' ,'数据文件(*.txt;*.dat)';'*.*','所有文件 (*.*)'},'保存数据文件'); 
% try 
%     TestData=load([pathname 'TestData.mat']); 
%     B=TestData.TestData_FE;
%     clear TestData;
%     %fieldName = fieldnames(S); 
%     %str = cell2mat(fieldName); 
%     %B = getfield(S,str); 
%    
%     [m,n] = size(B); 
% %     [filename, pathname] = uiputfile({'*.txt;*.dat' ,'数据文件(*.txt;*.dat)';'*.*','所有文件 (*.*)'},'保存数据文件'); 
%     fid = fopen([pathname 'TestData.dat'],'w'); 
%     if(fid~=-1) 
% 
%         for k=1:m 
%             for kk = 1:n 
%                 fprintf(fid,'\t%d',kk); 
%                 fprintf(fid,':'); 
%                 fprintf(fid,'%d',B(k,kk)); 
%             end 
%             fprintf(fid,'\n'); 
%         end 
%         fclose(fid); 
%     else 
%         msgbox('无法保存文件!'); 
%     end 
% catch 
%    msgbox('文件保存过程中出错!','出错了...','error'); 
% end

try 

    B=label;
    
    %fieldName = fieldnames(S); 
    %str = cell2mat(fieldName); 
    %B = getfield(S,str); 
   
    [m,n] = size(B); 
    B = 1 + 0.5*(1-B);
%   [filename, pathname] = uiputfile({'*.txt;*.dat' ,'数据文件(*.txt;*.dat)';'*.*','所有文件 (*.*)'},'保存数据文件'); 
    fid = fopen([pathname 'groundtruth105.dat'],'w'); 
    if(fid~=-1) 
        for k=1:m 
                fprintf(fid,'%d',B(k,1)); 
            fprintf(fid,'\n'); 
        end 
        fclose(fid); 
    else 
        msgbox('无法保存文件!'); 
    end 
catch 
   msgbox('文件保存过程中出错!','出错了...','error'); 
end

% try 
%     group=load([pathname 'group.mat']); 
%     B=group.group;
%     a=find(B==1);
%     %fieldName = fieldnames(S); 
%     %str = cell2mat(fieldName); 
%     %B = getfield(S,str); 
%    
%     [m,n] = size(B); 
% %   [filename, pathname] = uiputfile({'*.txt;*.dat' ,'数据文件(*.txt;*.dat)';'*.*','所有文件 (*.*)'},'保存数据文件'); 
%     fid = fopen([pathname 'group.dat'],'w'); 
%     if(fid~=-1) 
%         for k=1:n 
%                 fprintf(fid,'%d',B(1,k)); 
%             fprintf(fid,'\n'); 
%         end 
%         fclose(fid); 
%     else 
%         msgbox('无法保存文件!'); 
%     end 
% catch 
%    msgbox('文件保存过程中出错!','出错了...','error'); 
% end


