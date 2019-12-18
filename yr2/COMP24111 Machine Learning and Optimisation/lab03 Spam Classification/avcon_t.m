%calculate accuracy of generated possibility&statistic mode with unquantized attribute value
function [conf_mtx,acy] = avcon_t(avr, stdd, lbl_psb, testAttributeSet, validLabel)
%discriminative
%set any standard deviation as 1/t(in continuous naive bayes t is 1; in spambase t changes from 1 to 31)
fea_res=ones(length(validLabel),length(lbl_psb));
for i=1:size(testAttributeSet,2)
    for j=1:length(lbl_psb)
        if(stdd(j,i)==0)
            stdd(j,i)=0.0000001;
        end
    end
end
%MAP rule for raw possibility that an example belong to each class/label
for i=1:size(testAttributeSet,1) %2201
    for j=1:size(testAttributeSet,2) %57
        for k=1:length(lbl_psb) %2
            fea_res(i,k)=fea_res(i,k)*normpdf(testAttributeSet(i,j),avr(k,j),stdd(k,j));
        end
    end
    for j=1:length(lbl_psb) %2
        fea_res(i,j)=fea_res(i,j)*lbl_psb(j);
    end
end
%find the largest possibility for each example, thus determine a class that the example belongs to
lbl_res=zeros(size(testAttributeSet,1),1);
for i=1:size(testAttributeSet,1) %2201
    if(fea_res(i,1)>fea_res(i,2))
        lbl_res(i)=0;
    else
        lbl_res(i)=1;
    end
end
%verify lbl_res with vector validLabel and calculate accuracy
acy=0;
for i=1:size(testAttributeSet,1) %2201
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