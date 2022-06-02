timlim=10000;
exec[0]=0 #None
exec[1]=1 #All
exec[2]=2 #NO SIN_AGG
exec[3]=3 #NO ISO_AGG
exec[4]=4 #NO NON_CAN
exec[5]=5 #NO DOM
exec[6]=6 #NO NON_FIX

N[1]=1000;
N[2]=1500;
N[3]=2000;
N[4]=2500;

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

maindir=T2-PSCLP
mkdir ${maindir}
for((i=0; i<=6; i++));
do
   dir[i]=${maindir}/${exec[i]};
   mkdir ${dir[i]}
done

#Settings
for((p=0; p<=1;p++));
do
   # Number of customers
   for((i=1; i<=4;i++)); 
   do
      #Different radius and budget
      for((j=0; j<=13;j++));
      do
         #Random data
         for((l=1; l<=5;l++));
         do
            f_name=../TESTDATA/n${N[i]}s$l.dat
            c_name=../TESTDATA/n${N[i]}s$l.dat

            workname=n${N[i]}_m${N[i]}_r${R[j]}_d${D[j]}_s${l}

               #bsub -J ${workname} -q batch -R "span[ptile=2]" -n 2 -e ./${dir[p]}/${workname}.err -o ./${dir[p]}/${workname}.out "./bin/MCLPCPX ${exec[p]} ${f_name} ${c_name} $timlim ${R[j]} 0 ${D[k]}"
            echo "./bin/PSCLPCPX ${exec[p]} ${f_name} ${c_name} $timlim ${R[j]} 1 ${D[j]}"
         done
      done
   done
done
