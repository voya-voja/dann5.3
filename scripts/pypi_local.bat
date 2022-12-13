@echo off

REM Created on Thu Dec 23 15:20:42 2021

REM @author: Nebojsa.Vojinovic

if NOT [%1%]==[] goto welcome
echo **** Error: provide the version number, e.g. the velue of #.# for statement:
echo **** pip install dist\dann5-2.#.#.tar.gz
echo .
goto Exit

:welcome
echo.
echo -----------------------------------------
echo *** Create and locally deploy the ***
echo **** dann5 version 2.%1 package ***
echo -----------------------------------------
echo.

python -m build --sdist
python -m build --wheel
twine check dist/*

set deploy=n
set /p deploy=Do you want to deploy the package (y/n)? [%deploy%]:

if [%deploy%]==[n] goto Exit

pip install dist\dann5-2.%1%.tar.gz
:Exit