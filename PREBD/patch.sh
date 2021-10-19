mkdir -p src_MCLP
mkdir -p src_PSCLP
for file in LocationCovering/src_MCLP/*
do
   name=${file##*/}
   if [[ -f src_MCLP_patch/$name.patch ]]; then
      patch LocationCovering/src_MCLP/$name src_MCLP_patch/$name.patch 
   fi
done
cp src_MCLP_patch/*.cpp LocationCovering/src_MCLP/
cp src_MCLP_patch/*.h LocationCovering/src_MCLP/
for file in LocationCovering/src_PSCLP/*
do
   name=${file##*/}
   if [[ -f src_PSCLP_patch/$name.patch ]]; then
      patch LocationCovering/src_PSCLP/$name src_PSCLP_patch/$name.patch 
   fi
done
cp src_PSCLP_patch/*.cpp LocationCovering/src_PSCLP/
cp src_PSCLP_patch/*.h LocationCovering/src_PSCLP/

