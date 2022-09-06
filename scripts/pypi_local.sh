echo "Created on Thu Dec 23 15:20:42 2021"

echo "@author: voya-voja"

if [ $1 == "" ]; then
    echo "**** Error: provide the version number, e.g. the velue of #.# for statement:"
    echo "**** pip install dist\dann5-2.#.#.tar.gz"
else
    echo "-----------------------------------------"
    echo "*** Create and locally deploy the ***"
    echo "**** dann5 version 2.$1 package ***"
    echo "-----------------------------------------"

    python -m build --sdist
    python -m build --wheel
    twine check dist/*

    deploy="n"
    read -p "Do you want to deploy the package (y/n)? [$deploy]:" deploy

    if [ $deploy == 'y' ]; then
        pip install dist/dann5-2.$1.tar.gz
    fi
fi
