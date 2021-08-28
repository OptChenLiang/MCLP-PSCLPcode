timlim=10000;
exec[0]=0 #None
exec[1]=1 #All
exec[2]=2 #NO SIN_AGG
exec[3]=3 #NO ISO_AGG
exec[4]=4 #NO NON_CAN
exec[5]=5 #NO DOM
exec[6]=6 #NO NON_FIX

N[1]=500;
N[2]=1000;
N[3]=1500;
N[4]=2000;

D[0]=0.5;
D[1]=0.5;
D[2]=0.5;
D[3]=0.5;

R[0]=5.5;
R[1]=5.75;
R[2]=6;
R[3]=6.25;

D[4]=0.6;
D[5]=0.6;
D[6]=0.6;
D[7]=0.6;
D[8]=0.6;

R[4]=4;
R[5]=4.25;
R[6]=4.5;
R[7]=4.75;
R[8]=5;

D[9]=0.7;
D[10]=0.7;
D[11]=0.7;
D[12]=0.7;
D[13]=0.7;

R[9]=3.25;
R[10]=3.5;
R[11]=3.75;
R[12]=4;
R[13]=4.25;

M[1]=10000000;
M[2]=100000000;
M[3]=1000000000;

maindir=T3-PSCLP
mkdir ${maindir}
for((i=0; i<=6;i++));
do
   dir[i]=${maindir}/${exec[i]};
   mkdir ${dir[i]}
done

#Settings
for((p=0; p<=6;p++));
do
   #Number of customers
   for((k=1; k<=3;k++));
   do
      #Number of facities
      for((i=1; i<=4;i++));
      do
         #Different radius and covering demand
         for((j=0; j<=13;j++));
         do
            #Random data
            for((l=1; l<=5;l++));
            do
               DD=${D[j]};
               RR=$(awk -v r=${R[j]} -v n=${N[i]} 'BEGIN{print r*(100/n)^(0.5)}');

               f_name=../TESTDATA/n2500s$l.dat

               workname=n${N[i]}_m${M[k]}_r${RR}_d${DD}_s${l}
         
               #bsub -J ${workname} -q batch -R "span[ptile=2]" -n 2 -e ./${dir[p]}/${workname}.err -o ./${dir[p]}/${workname}.out "./PSCLP ${exec[p]} ${f_name} NULL ${N[i]} ${M[k]} $timlim ${RR} ${DD}"
               echo "./bin/PSCLPCPX ${exec[p]} ${f_name} NULL ${N[i]} ${M[k]} $timlim ${RR} ${DD}"

            done
         done
      done
   done
done
