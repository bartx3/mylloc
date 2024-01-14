if [ -d .git ]; then
    # Check if we have a submodule
    if [ -f .gitmodules ]; then
        # Check if the library is built
        if [ ! -f libmylloc.a ]; then
            # Build the library
            ./mylloc/build_lib.sh .
        fi
    else
            git submodule add https://github.com/bartx3/mylloc mylloc

            ./mylloc/build_lib.sh .
    fi
else
    if [ ! -d logger ]; then
        # Download the logger zip file
        git clone https://github.com/bartx3/mylloc.git
        # Unzip the logger file
    fi
    if [ ! -f liblogger.a ]; then
        # Build the library
        ./mylloc/build_lib.sh .
    fi
fi
