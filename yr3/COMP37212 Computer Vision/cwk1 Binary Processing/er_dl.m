function [res] = er_dl(img, SE, ep, choice)
% flip pic if dilation
if strcmp(choice, 'd')
    img = imcomplement(img);
end
se_len=size(SE,1);
ind=find(SE==0);
n_l=floor(se_len/2);
[width,height]=size(img);
for k=1:ep
    % pad to deal wtih edge pixels
    I_pad=padarray(img,[n_l,n_l],'symmetric');
    % result pic
    res=zeros(width,height);
    for i=1:width
        for j=1:height
            % extract subimage filtered by SE
            Block=I_pad(i:i+2*n_l,j:j+2*n_l);
            C=Block.*SE;
            C=C(:);
            C(ind)=[];
            % erosion
            res(i,j)=min(C);
        end
    end
    img=res;
% flip back if dilation
end
if strcmp(choice, 'd')
    res = imcomplement(res);
end