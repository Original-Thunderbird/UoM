ep = 9;
% on chromosome
%I=im2double(imread('ChromoBW_detail.bmp')); 
% on egg
ep0=im2double(imread('eggs.tif')); 
SE=[0 1 0;1 1 1; 0 1 0];

ep1 = er_dl(ep0, SE, ep, 'e');
ep2 = er_dl(ep1, SE, ep, 'd');
dif = imcomplement(ep2-ep0);
%imwrite(dif,['open_ep=',num2str(ep),'.jpg']);
imagesc(dif);
axis image;
colormap(gray(256));
