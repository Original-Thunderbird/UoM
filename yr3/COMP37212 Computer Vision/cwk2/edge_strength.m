img=im2double(imread('kitty.bmp'));
% kernel=[0.5 1 0.5;1 2 1; 0.5 1 0.5];
% blr=cov(img,kernel);
vert_kernel=[-1 0 1;-2 0 2;-1 0 1];
hori_kernel=[1 2 1;0 0 0;-1 -2 -1];
se_len=size(vert_kernel,1);
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
        vert=Block.*vert_kernel;
        hori=Block.*hori_kernel;
        ve_sum=sum(sum(vert));
        ho_sum=sum(sum(hori));
        % convolution
        res(i,j)=sqrt(ve_sum*ve_sum+ho_sum*ho_sum)./sum(sum(abs(vert_kernel)));
    end
end
imagesc(res);
axis image;
colormap(gray(256));