N[1]=1000;
N[2]=1100;
N[3]=1200;
N[4]=1300;
N[5]=1400;
N[6]=1500;
N[7]=1600;
N[8]=1700;
N[9]=1800;
N[10]=1900;
N[11]=2000;
N[12]=2100;
N[13]=2200;
N[14]=2300;
N[15]=2400;
N[16]=2500;

D[1]=15;
D[2]=20;
D[3]=25;

R[1]=3.5;
R[2]=3.75;
R[3]=4;

timlim=10000;
exec[1]=MCLP_None
exec[2]=MCLP_All
exec[3]=MCLP_NoDom
exec[4]=MCLP_NoNC
exec[5]=MCLP_NoIA
exec[6]=MCLP_NoSA
exec[7]=MCLP_NoNF

maindir=T2
mkdir ${maindir}
for((i=1; i<=7; i++));
do
   dir[i]=${maindir}/${exec[i]};
   mkdir ${dir[i]}
done

#Settings
for((p=1; p<=7;p++));
do
   # Number of customers
   for((i=1; i<=16;i++)); 
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
               f_name=../TESTDATA/n2500_s$l.dat
               c_name=../TESTDATA/n2500_s$l.dat

               workname=n${N[i]}_m${N[i]}_r${R[j]}_d${D[k]}_s${l}
         
               #bsub -J ${workname} -q batch -R "span[ptile=2]" -n 2 -e ./${dir[p]}/${workname}.err -o ./${dir[p]}/${workname}.out "./${exec[p]} ${f_name} ${c_name} ${N[i]} ${N[i]} $timlim ${R[j]} ${D[k]}"
               echo "./${exec[p]} ${f_name} ${c_name} ${N[i]} ${N[i]} $timlim ${R[j]} ${D[k]}"
            done
         done
      done
   done
done