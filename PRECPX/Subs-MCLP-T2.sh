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

D[1]=15;
D[2]=20;
D[3]=25;

R[1]=3.5;
R[2]=3.75;
R[3]=4;

maindir=T2-MCLP
mkdir ${maindir}
for((i=0; i<=6; i++));
do
   dir[i]=${maindir}/${exec[i]};
   mkdir ${dir[i]}
done

#Settings
for((p=1; p<=1;p++));
do
   # Number of customers
   for((i=1; i<=3;i++)); 
   do
      #Radius
      for((j=1; j<=3;j++));  
      do
         # Budget
         for((k=1; k<=3;k++)); 
         do
            #Random data
            for((l=1; l<=5;l++));
            do
               f_name=../TESTDATA/n${N[i]}s$l.dat
               c_name=../TESTDATA/n${N[i]}s$l.dat

               workname=n${N[i]}_m${N[i]}_r${R[j]}_d${D[k]}_s${l}
         
               #bsub -J ${workname} -q batch -R "span[ptile=2]" -n 2 -e ./${dir[p]}/${workname}.err -o ./${dir[p]}/${workname}.out "./bin/MCLPCPX ${exec[p]} ${f_name} ${c_name} $timlim ${R[j]} 0 ${D[k]}"
               echo "./bin/MCLPCPX ${exec[p]} ${f_name} ${c_name} $timlim ${R[j]} 0 ${D[k]}"
            done
         done
      done
   done
done
