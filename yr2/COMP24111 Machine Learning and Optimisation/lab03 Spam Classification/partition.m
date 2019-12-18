%split spambase.data into 10 subsets for cross validation, after this
%step we get 10 pairs of trainng set(AttributeSet, LabelSet) and test set(testAttributeSet, validLabel).
function [AttributeSet,LabelSet,testAttributeSet,validLabel]=partition(spambase)
%extract attribute and label from raw data
RawAttribute=spambase(:,1:57);
RawLabel=spambase(:,58);
%randomize sequence of examples in dataset
vector = randperm(length(RawLabel));
TtlAttributeSet=zeros(size(RawAttribute));
TtlLabelSet=zeros(size(RawLabel));
for i=1:length(RawLabel)
    TtlAttributeSet(i,:)=RawAttribute(vector(i),:);
    TtlLabelSet(i)=RawLabel(vector(i));
end
%determine size of one fold
len=fix(length(RawLabel)/10);
AttributeSet=zeros(10,length(RawLabel)-len,size(RawAttribute,2));
LabelSet=zeros(10,length(RawLabel)-len,1);
testAttributeSet=zeros(10,len,size(RawAttribute,2));
validLabel=zeros(10,len,1);
for i=1:10
    %extract set for generation from raw set
    %raw_att=RawAttribute;
    raw_att=TtlAttributeSet;
    raw_att((i-1)*len+1:i*len,:)=[];
    %raw_lbl=RawLabel;
    raw_lbl=TtlLabelSet;
    raw_lbl((i-1)*len+1:i*len)=[];
    cnt_0=length(find(raw_lbl==0));
    %att_0=zeros(cnt_0,size(RawAttribute,2));
    att_0=zeros(cnt_0,size(TtlAttributeSet,2));
    %att_1=zeros(length(raw_lbl)-cnt_0,size(RawAttribute,2));
    att_1=zeros(length(raw_lbl)-cnt_0,size(TtlAttributeSet,2));
    lbl_0=zeros(cnt_0,1);
    lbl_1=zeros(length(raw_lbl)-cnt_0,1);
    %rearrange sequence of examples in test attribute sets so that examples
    %belong to a class will gather for convenience of calculating mean and
    %stdd.
    i0=1;   i1=1;
    for j=1:length(raw_lbl)
        if(raw_lbl(j)==0)
            att_0(i0,:)=raw_att(j,:);
            lbl_0(i0)=raw_lbl(j);
            i0=i0+1;
        else
            att_1(i1,:)=raw_att(j,:);
            lbl_1(i1)=raw_lbl(j);
            i1=i1+1;
        end
    end
    %extract set for generation from raw set
    AttributeSet(i,:,:)=[att_0;att_1];
    LabelSet(i,:)=[lbl_0;lbl_1];
    %testAttributeSet(i,:,:)=RawAttribute((i-1)*len+1:i*len,:);
    testAttributeSet(i,:,:)=TtlAttributeSet((i-1)*len+1:i*len,:);
    %validLabel(i,:)=RawLabel((i-1)*len+1:i*len);
    validLabel(i,:)=TtlLabelSet((i-1)*len+1:i*len);
end