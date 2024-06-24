workdir=$(pwd)
compiler=$2

if [ -d .git ]; then
    # Check if we have a submodule
    if [ -f .gitmodules ]; then
        # Check if the library is built
        if [ ! -f libmylloc.a ]; then
            # Build the library
            cd mylloc || exit
            ./build_lib.sh "$workdir" "$compiler"
        fi
    else
            git submodule add https://github.com/bartx3/mylloc mylloc
            cd mylloc || exit
            ./build_lib.sh "$workdir" "$compiler"
    fi
else
    if [ ! -d mylloc ]; then
        git clone https://github.com/bartx3/mylloc.git mylloc
    fi
    if [ ! -f libmylloc.a ]; then
        # Build the library
        cd mylloc || exit
        ./build_lib.sh "$workdir" "$compiler"
    fi
fi
