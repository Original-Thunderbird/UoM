% erosion for binary image
% read image
img=imread('eggs.tif');
% turn the image to a binary one
% img=im2bw(img);
% create structuring element               
se=[0 1 0; 1 1 1; 0 1 0];
% store number of rows in P and number of columns in Q.             
[w_se, h_se]=size(se);
% create a zero matrix of size I.
[w_im, h_im]=size(img);
In=zeros(w_im, h_im);  

for i=ceil(w_se/2):w_im-floor(w_se/2) 
    for j=ceil(h_se/2):h_im-floor(h_se/2) 
        % take all the neighbourhoods. 
        on=img(i-floor(w_se/2):i+floor(w_se/2), j-floor(h_se/2):j+floor(h_se/2));  
        % take logical se 
        nh=on(logical(se));  
        % compare and take minimum value of the neighbor  
        % and set the pixel value to that minimum value.  
        In(i, j)=min(nh(:));       
    end
end
imshow(In); 