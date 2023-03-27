%binary classification,k-NN
%instruction run task1a at cmd line
load ORLfacedata
mean_te=zeros(6,1);
mean_tr=zeros(6,1);
div_te=zeros(6,1);
div_tr=zeros(6,1);
kNN=1:6;
res_y_te=zeros(14,6);
for k=1:6
    lbl_y=zeros(14,1);
    ext1_30=data([1:10,291:300],:);
    lbe1_30=labels([1:10,291:300],:);
    %testing accuracy
    accuracy_te=zeros(50,1);
    for i=1:50
        [Xtr, Xte, Ytr, Yte] = PartitionData(ext1_30, lbe1_30, 3);
        true=0;
        for j=1:14
            x=Xte(j,:);
            res_y_te(j,k)=knearest(k,x,Xtr,Ytr);
            if(res_y_te(j,k)==Yte(j))
                true = true+1;
            end
        end    
        accuracy_te(i)=true/14.0;
    end
    for i=1:14
        if(res_y_te(i,k)>=15.5)
            lbl_y(i)=30;
        elseif(res_y_te(i,k)<15.5)
            lbl_y(i)=1;
        end
    end
    %Figure 1-6 gives classification result according to 1-6 nearest neighbors.
    figure(k); ShowResult(Xte, Yte, lbl_y, 7);
    div_te(k)=std(accuracy_te);
    mean_te(k)=sum(accuracy_te)/50.0;
    %training accuracy
    accuracy_tr=zeros(50,1);
    for i=1:50
        res_y_tr=zeros(6,1);
        [Xtr, Xte, Ytr, Yte] = PartitionData(ext1_30, lbe1_30, 3);
        true=0;
        for j=1:6
            x=Xtr(j,:);
            res_y_tr(j)=knearest(k,x,Xtr,Ytr);
            if(res_y_tr(j)==Ytr(j))
                true = true+1;
            end
        end    
        accuracy_tr(i)=true/6.0;
    end
    div_tr(k)=std(accuracy_tr);
    mean_tr(k)=sum(accuracy_tr)/50.0;    
end
figure(7);errorbar(kNN,mean_te,div_te); xlabel("k");ylabel("testing accuracy");set(gca,'xtick',1:1:6);
figure(8);errorbar(kNN,mean_tr,div_tr); xlabel("k");ylabel("training accuracy");set(gca,'xtick',1:1:6);