%calculate accuracy of using each of 10 produced subsets as test set and
%rest of spambase(the whole base except those in subset)as training set
function [acy] = spam_t(avr, stdd, lbl_psb, testAttributeSet, validLabel)
acy=zeros(10,1);
att_set=zeros(size(testAttributeSet,2),size(testAttributeSet,3));
lbl_set=zeros(size(validLabel,2),1);
avr_set=zeros(size(avr,2),size(avr,3));
stdd_set=zeros(size(stdd,2),size(stdd,3));
psb_set=zeros(2,1);
for i=1:10
    for j=1:size(testAttributeSet,2)
        att_set(j,:)=testAttributeSet(i,j,:);
        lbl_set(j)=validLabel(i,j);
    end
    for j=1:size(avr,2)
        avr_set(j,:)=avr(i,j,:);
        stdd_set(j,:)=stdd(i,j,:);
        psb_set(j)=lbl_psb(i,j);
    end
    %calculate accuracy of a generated model
   [~,acy(i)] = avcon_t(avr_set, stdd_set, psb_set, att_set, lbl_set);
end