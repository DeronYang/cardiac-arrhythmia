% load ecg.log
% a = ecg(:,2);
% len = length(a);
fid = fopen('server.log','w');
for k = 1:len
    fprintf(fid,'%d\n',a(k));
end
