#/bin/sh -x

# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37

alert='\033[0;33m'
normal='\033[0m'

if [ "$#" -ne 3 ]; then
    echo -e "$alert"
    echo "    usage example:"
    echo "      $0 dyn vvec void*"
    echo "      $0 dyn string char"
    echo
    echo "     where:"
    echo "       'dyn' is the template file"
    echo "       'char' replaces the type in dyn"
    echo "       'string' replaces the name 'dyn'"
    echo "                 and creates file 'string.c'"
	echo -e "$normal"
	exit -1
fi

template=$1
name=$2
type=$3

cp $1 $name.h &&
sed -i "s/\${name}/$name/g" $name.h &&
sed -i "s/\${type}/$type/g" $name.h &&
ls -la --color $name.h
