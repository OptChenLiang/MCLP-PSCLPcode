timlim=10000;
exec[0]=0 #None
exec[1]=1 #All presolving methods without NON_CAN and NON_FIX

N[1]=100;

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

M[1]=10000;
M[2]=50000;
M[3]=100000;
M[4]=500000;
M[5]=1000000;
M[6]=1500000;
M[7]=2000000;
M[8]=4000000;
M[9]=6000000;
M[10]=10000000;
M[11]=15000000;
M[12]=20000000;
M[13]=25000000;
M[14]=30000000;
M[15]=35000000;
M[16]=40000000;

maindir=T1-PSCLP
mkdir ${maindir}
for((i=0; i<=1;i++));
do
   dir[i]=${maindir}/${exec[i]};
   mkdir ${dir[i]}
done

#Settings
for((p=0; p<=1; p++));
do
   #Number of customers
   for((k=1; k<=16; k++));
   do
      #Number of facities
      for((i=1; i<=1; i++));
      do
         #Different radius and budget
         for((j=0; j<=13;j++));
         do
            #Random data
            for((l=1; l<=5;l++));
            do
               DD=${D[j]};
               RR=$(awk -v r=${R[j]} -v n=${N[i]} 'BEGIN{print r*(100/n)^(0.5)}');

               f_name=../TESTDATA/n${N[i]}s$l.dat

               workname=n${N[i]}_m${M[k]}_r${RR}_d${DD}_s${l}
               #bsub -J ${workname} -q batch -R "span[ptile=2]" -n 2 -e ./${dir[p]}/${workname}.err -o ./${dir[p]}/${workname}.out "./PSCLP ${exec[p]} ${f_name} NULL ${N[i]} ${M[k]} $timlim ${RR} ${DD}"
               echo "./bin/PSCLPBD ${exec[p]} ${f_name} ${M[k]} $timlim ${RR} 1 ${DD}"
            done
         done
      done
   done
done
