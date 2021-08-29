ptimlim=10000;
exec[0]=0 #None
exec[1]=1 #All presolving methods without NON_CAN and NON_FIX

N[1]=200;
N[2]=300;
N[3]=400;
N[4]=500;

D[0]=0.1;
D[1]=0.1;
D[2]=0.1;
D[3]=0.1;

R[0]=5.5;
R[1]=5.75;
R[2]=6;
R[3]=6.25;

D[4]=0.15;
D[5]=0.15;
D[6]=0.15;
D[7]=0.15;
D[8]=0.15;

R[4]=4;
R[5]=4.25;
R[6]=4.5;
R[7]=4.75;
R[8]=5;

D[9]=0.2;
D[10]=0.2;
D[11]=0.2;
D[12]=0.2;
D[13]=0.2;

R[9]=3.25;
R[10]=3.5;
R[11]=3.75;
R[12]=4;
R[13]=4.25;

M[1]=10000000;
M[2]=100000000;
M[3]=1000000000;

maindir=T3-MCLP
mkdir ${maindir}
for((i=0; i<=1;i++));
do
   dir[i]=${maindir}/${exec[i]};
   mkdir ${dir[i]}
done

#Settings
for((p=0; p<=1;p++));
do
   #Number of customers
   for((k=1; k<=3;k++));
   do
      #Number of facities
      for((i=1; i<=4;i++));
      do
         #Different radius and budget
         for((j=0; j<=13;j++));
         do
            #Random data
            for((l=1; l<=5;l++));
            do
               DD=${D[j]};
               RR=$(awk -v r=${R[j]} -v n=${N[i]} 'BEGIN{print r*(100/n)^(0.5)}');

               f_name=../TESTDATA/n2500s$l.dat

               workname=n${N[i]}_m${M[k]}_r${RR}_d${DD}_s${l}
         
               #bsub -J ${workname} -q batch -R "span[ptile=2]" -n 2 -e ./${dir[p]}/${workname}.err -o ./${dir[p]}/${workname}.out "./MCLP ${exec[p]} ${f_name} NULL ${N[i]} ${M[k]} $timlim ${RR} ${DD}"
               echo "./bin/MCLPBD ${exec[p]} ${f_name} NULL ${N[i]} ${M[k]} $timlim ${RR} ${DD}"
            done
         done
      done
   done
done
