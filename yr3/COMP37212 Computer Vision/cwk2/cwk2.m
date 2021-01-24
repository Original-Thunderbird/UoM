img=im2double(imread('kitty.bmp'));
% blur first
% average smoothing
%kernel=[1 1 1;1 1 1; 1 1 1];
% gaussian smoothing
kernel=[0.5 1 0.5;1 2 1; 0.5 1 0.5];
blr=cov(img,kernel);
% vertical edge
vert=[-1 0 1;-2 0 2;-1 0 1];
% horizontal edge
hori=[1 2 1;0 0 0;-1 -2 -1];
ix=cov(blr,vert);
iy=cov(blr,hori);
res=sqrt(ix.*ix+iy.*iy);
thr=res;
for i=1:size(img,1)
    for j=1:size(img,2)
        if thr(i,j)<0.1
            thr(i,j)=0;
        end
    end
end
% res=res-ones(size(img,1),size(img,2)).*0.3;
figure(1);
imhist(res);
figure(2);
imagesc(thr);
axis image;
colormap(gray(256));