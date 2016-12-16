#/bin/sh -x
echo "generated source:"
./dyn.apply dynp void* &&
./dyn.apply dynf float &&
./dyn.apply str char &&
./dyn.apply dyni int &&
mv -f *.h ../src/lib/ &&

./dyn.apply objmtls objmtl* &&
mv -f *.h ../src/gfx/ &&

./alloc.apply alloc object objects 8196 &&
mv -f *.h ../src/obj/ &&

echo
