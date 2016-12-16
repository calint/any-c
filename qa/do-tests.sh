#/bin/sh
date &&
cd .. &&
./make.sh &&
cd qa &&
./do-test.sh 1 &&
./do-test.sh 2 &&
./do-test.sh 3 &&
date &&
echo done. ok
