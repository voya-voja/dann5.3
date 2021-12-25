@echo off

REM Created on Thu Dec 23 15:23:44 2021

REM @author: Nebojsa.Vojinovic

echo.
echo -----------------------------------------
echo *** Create and deploy to PYPI the ***
echo ********** dann5 package ************
echo -----------------------------------------
echo.

python -m build --sdist
python -m build --wheel
twine check dist/*
set deploy=n
set /p deploy=Do you want to deploy the package (y/n)? [%deploy%]:

if [%deploy%]==[n] goto Exit

twine upload dist/*

:Exit