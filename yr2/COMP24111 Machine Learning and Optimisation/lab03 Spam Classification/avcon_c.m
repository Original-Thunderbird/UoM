%generate possibility&statistic model for a training set with unquantized attribute value
function [avr, stdd, lbl_psb] = avcon_c(AttributeSet, LabelSet)
%adjust datasets from spambase to 2-dimention matrix(for attribut set) and a 1-dimention vector(for label set)
if(size(AttributeSet)==3)
    AttributeSet=reshape(AttributeSet,size(AttributeSet,2),size(AttributeSet,3));
end
if(size(LabelSet)==2)
    LabelSet=reshape(LabelSet,size(AttributeSet,2));
end
%generative
%count number of classes
lbl_type=numel(unique(LabelSet));
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
%seperate attribute set and label set by their class
att_set_0=AttributeSet(1:lbl_cnt(1),:);
att_set_1=AttributeSet(lbl_cnt(1)+1:length(LabelSet),:);
%calculate mean and standard deviation of possibilities
avr=zeros(lbl_type,size(AttributeSet,2));
stdd=zeros(lbl_type,size(AttributeSet,2)); 
for i=1:size(AttributeSet,2) %57
   avr(1,i)=mean(att_set_0(:,i));
   avr(2,i)=mean(att_set_1(:,i));
   stdd(1,i)=std(att_set_0(:,i));
   stdd(2,i)=std(att_set_1(:,i));
end