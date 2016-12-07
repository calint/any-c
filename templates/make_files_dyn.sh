#/bin/sh -x
sh template.sh dyn dynp void* &&
sh template.sh dyn dynf float &&
sh template.sh dyn dync char &&
sh template.sh dyn dyni int &&
sh template.sh dyn dyno object &&
mv -f dyn*.h ../src