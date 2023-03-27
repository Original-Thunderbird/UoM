%multiclass classification,linear classification
%instruction run task2b at cmd line
load ORLfacedata
acc_std=zeros(50,1);
acc_kNN=zeros(50,1);
acu_spr=zeros(50,40);
for i=1:50
    true=0;
    Ytr_lbl=zeros(200,40);
    Yte_lbl=zeros(200,40);
    [Xtr, Xte, Ytr, Yte] = PartitionData(data, labels, 5);
    Xtr_l=[ones(200,1) Xtr];
    for j=1:200
        Ytr_lbl(j,Ytr(j))=1;
    end
    w=pinv(Xtr_l)*Ytr_lbl;
    Xte_l=[ones(200,1) Xte];
    y_th=Xte_l*w;
    lbl_res=zeros(200,1);
    %1-of-k coding scheme
    for j=1:200
        max=0;
        for k=1:40
            if(y_th(j,k)>max)
                max=y_th(j,k);
                in_max=k;
            end
        end
        %set the label with highest value as final value of the sample
        Yte_lbl(j,in_max)=1;
        lbl_res(j)=in_max;
        %calculate accuracy
        if(lbl_res(j)==Yte(j))
            true=true+1;
            acu_spr(i,Yte(j)) = acu_spr(i,Yte(j))+1;
        end
    acc_std(i)=true/200.0;
    end
    acu_spr(i,:) = acu_spr(i,:)/5.0;
end
av_acu_spr=mean(acu_spr);
%accuracy from k-NN
for i=1:50
    [Xtr, Xte, Ytr, Yte] = PartitionData(data, labels, 5);
    true=0;
   for j=1:200
       x=Xte(j,:);
       res_y=knearest(1,x,Xtr,Ytr);
       if(res_y==Yte(j))
            true = true+1;
        end
    end
    acc_kNN(i)=true/200.0;
end
plot(1:50,acc_kNN,'-bo');
hold on;
plot(1:50,acc_std,'-r+');
xlabel("i(representing each loop)");
ylabel("accuracy in percentage");
legend('1-NN calssifier','normal equation');