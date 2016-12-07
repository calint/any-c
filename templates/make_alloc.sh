#/bin/sh -x

alert='\033[0;33m'
normal='\033[0m'

if [ "$#" -ne 4 ]; then
    echo -e "$alert"
    echo "    usage example:"
    echo "      $0 alloc object objects 1024"
    echo
    echo "     where:"
    echo "       'alloc' is the template file"
    echo "       'object' replaces the type"
    echo "       'objects' and creates 'objects.c'"
	echo "         with an array of 1024 'objects'"
	echo -e "$normal"
	exit -1
fi

template=$1
name=$2
nameplr=$3
cap=$4

echo $1 $2 $4

fn=${name}_alloc.h

cp $1 $fn &&
sed -i "s/\${name}/$name/g" $fn &&
sed -i "s/\${nameplr}/$nameplr/g" $fn &&
sed -i "s/\${cap}/$cap/g" $fn &&
mv -f $fn ../src/ &&
cat ../src/$fn &&
ls -la --color ../src/$fn
