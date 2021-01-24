function [res] = cov(img,kernel)
se_len=size(kernel,1);
n_l=floor(se_len/2);
[width,height]=size(img);
% pad to deal wtih edge pixels
I_pad=padarray(img,[n_l,n_l],'both');
% result pic
res=zeros(width,height);
for i=1:width
    for j=1:height
        % extract subimage filtered by kernel
        Block=I_pad(i:i+2*n_l,j:j+2*n_l);
        C=Block.*kernel;
        % convolution
        res(i,j)=sum(sum(C))/sum(sum(abs(kernel)));
    end
end