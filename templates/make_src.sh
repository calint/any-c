#/bin/sh -x
echo "generated source:"
./dyn.apply ptrs void* &&
./dyn.apply strb char &&
mv -f *.h ../src/lib/ &&
echo
