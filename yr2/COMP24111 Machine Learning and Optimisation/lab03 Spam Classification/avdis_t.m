%calculate accuracy of generated possibility&statistic mode with quantized attribute value
function [conf_mtx,acy] = avdis_t(val_type, res_tbl, lbl_psb, testAttributeSet, validLabel)
%discriminative
%MAP rule for raw possibility that an example belong to each class/label
raw_res=ones(size(testAttributeSet,1),length(lbl_psb)); %2300*3
for i=1:size(testAttributeSet,1) %2300
    for j=1:length(lbl_psb) %3
       for k=1:size(testAttributeSet,2) %57
           lctn_tbl_r=(j-1)*val_type+testAttributeSet(i,k)+1;
           raw_res(i,j)=raw_res(i,j)*res_tbl(lctn_tbl_r,k);
       end
       raw_res(i,j)=raw_res(i,j)*lbl_psb(j);
    end
end
%find the largest possibility for each example, thus determine a class that
%the example belongs to
lbl_res=zeros(size(testAttributeSet,1),1);
for i=1:size(testAttributeSet,1) %2300
    max=0;
    index=0;
    for j=1:length(lbl_psb)
        if(raw_res(i,j)>max)
            max=raw_res(i,j);
            index=j;
        end
    end
    lbl_res(i)=index-1;
end
%verify lbl_res with vector validLabel and calculate accuracy
acy=0;
for i=1:size(testAttributeSet,1) %2300
    if(lbl_res(i)==validLabel(i))
        acy = acy+1;
    end
end
acy = acy/size(testAttributeSet,1);
%build confusion matrix
conf_mtx=zeros(length(lbl_psb),length(lbl_psb));
for i=1:length(validLabel)
    conf_mtx(lbl_res(i)+1,validLabel(i)+1)=conf_mtx(lbl_res(i)+1,validLabel(i)+1)+1;
end