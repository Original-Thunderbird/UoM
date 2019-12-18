%generate possibility&statistic model for a training set with quantized attribute value
function [res_tbl,lbl_psb,val_type] = avdis_c(AttributeSet, LabelSet)
%generative
%count number of value types
val_type=0;
for i=1:size(AttributeSet,2)
    if (numel(unique(AttributeSet(:,i)))>val_type)
        val_type=numel(unique(AttributeSet(:,i)));
    end
end
%count number of label types
lbl_type=numel(unique(LabelSet));
res_tbl=zeros(val_type*lbl_type,size(AttributeSet,2));
%count number of examples under each value-label combination
for i=1:size(AttributeSet,1) %2300
    for j=1:size(AttributeSet,2)  %57
        r_idx=LabelSet(i)*val_type+AttributeSet(i,j)+1; 
        res_tbl(r_idx,j)=res_tbl(r_idx,j)+1;
    end
end
%count number of examples under each label
lbl_cnt=zeros(lbl_type,1);
for i=1:length(LabelSet)
    lbl_cnt(LabelSet(i)+1)=lbl_cnt(LabelSet(i)+1)+1;
end
%calculate possibility that an example has a type of label
lbl_psb=zeros(lbl_type,1);
for i=1:length(lbl_psb)
    lbl_psb(i)=lbl_cnt(i)/length(LabelSet);
end
%calculate raw probability table
for i=1:size(res_tbl,1)
    lbl=fix((i-1)/val_type);
    res_tbl(i,:)=res_tbl(i,:)./lbl_cnt(lbl+1);
end
%refine probability table with m-estimate(m=1)
for i=1:size(AttributeSet,2)
    for j=1:lbl_type
        if (ismember(0,res_tbl(val_type*(j-1)+1:val_type*j,i)))
            for k=val_type*(j-1)+1:val_type*j
                res_tbl(k,i)=(res_tbl(k,i)*lbl_cnt(j)+1/val_type)/(lbl_cnt(j)+1);
            end
        else
            for k=val_type*(j-1)+1:val_type*j
                res_tbl(k,i)=res_tbl(k,i);
            end
        end
    end
end


            