echo Created on Thu Dec 23 15:23:44 2021

echo @author: voya-voja

echo -----------------------------------------
echo *** Create and deploy to PYPI the ***
echo ********** dann5 package ************
echo -----------------------------------------

python -m build --sdist
python -m build --wheel
twine check dist/*

deploy="n"
read -p "Do you want to deploy the package (y/n)? [$deploy]:" deploy

if [ $deploy == 'y' ]; then
    twine upload dist/*
fi
