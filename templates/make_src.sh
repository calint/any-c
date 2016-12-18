#/bin/sh -x
echo "generated source:"
./dyn.apply dynp void* &&
./dyn.apply str char &&
mv -f *.h ../src/lib/ &&
echo
