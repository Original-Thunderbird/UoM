close all;
clear all;
%input file list:
%avc_c2.mat: examples in 2 classes, with unquantized attribute values
%av7_c3.mat: examples in 3 classes, with attribute values quantized to [0,1,2,3,4,5,6]
%av3_c2.mat: examples in 2 classes, with attribute values quantized to [0,1,2]
%av2_c2.mat: examples in 2 classes, with attribute values quantized to [0,1]
%spambase.data: examples in 2 classes, with unquantized attribute values(larger database)
fname = input('Enter a filename to load data for training/testing: ','s');
load(fname);
raw_acc=zeros(10,1);
mean_acc=zeros(31,1);
std_acc=zeros(31,1);
if(strcmp(fname,"spambase.data"))
    %split spambase.data into 10 subsets for cross validation, after this
    %step we get 10 pairs of trainng set(AttributeSet, LabelSet) and test set(testAttributeSet, validLabel).
    [AttributeSet,LabelSet,testAttributeSet,validLabel]=partition(spambase);
end

if(strcmp(fname,"spambase.data"))
    %generate possibility&statistic model for each of 10 training sets
    [ avr, stdd, lbl_psb ] = spam_c(AttributeSet, LabelSet); % NB training
    
    %calculate accuracy of using each of 10 produced subsets as test set and rest of spambase(the whole base except those in subset)as training set
    [raw_acc] = spam_t( avr, stdd, lbl_psb, testAttributeSet, validLabel); % NB test
    
    %plot accuracy of using each of 10 produced subsets as test set and rest of spambase(the whole base except those in subset)as training set
    mean_acc=mean(raw_acc);
    std_acc=std(raw_acc);
    figure(1); plot(1:10,raw_acc,'-bo');   axis([1 10 0.78 0.85]);
    xlabel("k(loop)");ylabel("testing accuracy");
    accuracy=max(mean_acc);
elseif(strcmp(fname,"avc_c2.mat"))
    [ avr, stdd, lbl_psb ] = avcon_c(AttributeSet, LabelSet); % NB training
    [conf_mtx,accuracy] = avcon_t(avr, stdd, lbl_psb, testAttributeSet, validLabel); % NB test
else
    [ res_tbl,lbl_psb,val_type ] = avdis_c(AttributeSet, LabelSet); % NB training
    [conf_mtx,accuracy] = avdis_t( val_type, res_tbl, lbl_psb, testAttributeSet, validLabel); % NB test
end
fprintf('********************************************** \n');
fprintf('Overall Accuracy on Dataset %s: %f \n', fname, accuracy);
fprintf('********************************************** \n');