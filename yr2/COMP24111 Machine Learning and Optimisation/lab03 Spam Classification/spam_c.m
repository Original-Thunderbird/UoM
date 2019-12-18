%generate possibility&statistic model for each of 10 training sets
function [avr, stdd, lbl_psb] = spam_c(AttributeSet, LabelSet)
avr=zeros(10,2,size(AttributeSet,3));
stdd=zeros(10,2,size(AttributeSet,3));
lbl_psb=zeros(10,2,1);
att_set=zeros(size(AttributeSet,2),size(AttributeSet,3));
lbl_set=zeros(size(LabelSet,2),1);
for i=1:10
    for j=1:size(AttributeSet,2)
        att_set(j,:)=AttributeSet(i,j,:);
        lbl_set(j)=LabelSet(i,j);
    end
    %generate possibility&statistic model for one training set
    [avr(i,:,:),stdd(i,:,:),lbl_psb(i,:)]=avcon_c(att_set, lbl_set);
end

    