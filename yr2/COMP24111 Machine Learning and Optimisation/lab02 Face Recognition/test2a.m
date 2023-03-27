%binary classification,linear classification
%instruction run task2a at cmd line
load ORLfacedata
%extract data
ext1_30=data([1:10,291:300],:);
lbe1_30=labels([1:10,291:300],:);
accuracy_te=zeros(50,1);
accuracy_tr=zeros(50,1);
%6 samples for testing, 14 for training
y_th=zeros(14,50);
y_tr=zeros(6,50);
lbl_th=zeros(14,50);
lbl_tr=zeros(14,50);
for i=1:50
    [Xtr, Xte, Ytr, Yte] = PartitionData(ext1_30, lbe1_30, 3);
    Xtr_=[ones(6,1) Xtr];
    w=pinv(Xtr_)*Ytr;
    Xte_l=[ones(14,1) Xte];
    y_th(:,i)=Xte_l*w;
    Xtr_l=[ones(6,1) Xtr];
    y_tr(:,i)=Xtr_l*w;
    %testing accuracy
    for j=1:14
        if(y_th(j,i)<15.5)
            lbl_th(j,i)=1;
        end
        if(y_th(j,i)>15.5)
            lbl_th(j,i)=30;
        end
    end
    true=0;
    for j=1:14
        if(lbl_th(j,i)==Yte(j))
            true = true+1;
        end
    end    
    accuracy_te(i)=true/14.0;
    %training accuracy
    for j=1:6
        if(y_tr(j,i)<15.5)
            lbl_tr(j,i)=1;
        end
        if(y_tr(j,i)>15.5)
            lbl_tr(j,i)=30;
        end
    end
    true=0;
    for j=1:6
        if(lbl_tr(j,i)==Ytr(j))
            true = true+1;
        end
    end
    accuracy_tr(i)=true/6.0;
end
% classification result on test set
figure(1); ShowResult(Xte, Yte, lbl_th(:,50), 7);
% classification result on training set
figure(2); ShowResult(Xtr, Ytr, lbl_tr(:,50), 3);
% test accuracy for each round
figure(3); plot(1:50,accuracy_te,'-bo');
xlabel("i(representing each loop)");
ylabel("testing accuracy");
acc_te_avg = sum(accuracy_te)/50.0;