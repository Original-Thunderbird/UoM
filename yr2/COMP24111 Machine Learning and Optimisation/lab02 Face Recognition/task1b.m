%multiclass classification,k-NN
%instruction run task1b at cmd line
load ORLfacedata
kNN=1:6;
err_va=zeros(6,1);
%look for the best k
err_k=zeros(10);
min=1;
for k=1:10
    false=0;
    for i=1:400
        dta_k_tr=data;
        dta_k_tr(i,:)=[];
        dta_k_te=data(i,:);
        lbl_k_tr=labels;
        lbl_k_tr(i)=[];
        lbl_k_te=labels(i);
        res_y=knearest(k,dta_k_te,dta_k_tr,lbl_k_tr);
        if(res_y~=lbl_k_te)
            false = false+1;
        end
    end
    err_k(k)=false/400;
    if (err_k(k)<min)
        min=err_k(k);
        in_min=k;
    end
end
%prepare 50 datasets,k-NN using the 'best k'
acu_se=zeros(50,40);
accuracy_te=zeros(50,1);
for i=1:50
    [Xtr, Xte, Ytr, Yte] = PartitionData(data, labels, 5);
    true=0;
    for j=1:200
        x=Xte(j,:);
        res_y=knearest(in_min,x,Xtr,Ytr);
        if(res_y==Yte(j))
            true = true+1;
            acu_se(i,res_y) = acu_se(i,res_y)+1;
        end
    end
    acu_se(i,:) = acu_se(i,:)/5.0;
    accuracy_te(i)=true/200.0;
end
av_acu_se = mean(acu_se);
div=std(accuracy_te);
mean=sum(accuracy_te)/50.0;