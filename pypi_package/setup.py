# -*- coding: utf-8 -*-
"""
Created on Wed Dec 22 13:43:58 2021

@author: voya-voja

https://pypi.org/manage/account/
Username: voya-voja
Token: pypi-AgEIcHlwaS5vcmcCJDcwODM5ZTU4LTRjZjAtNGIyYS05YTllLTMxMWU0OGI5Njg0MQACJXsicGVybWlzc2lvbnMiOiAidXNlciIsICJ2ZXJzaW9uIjogMX0AAAYgJfMHimYQb1kn1OCHZtPQ34NtGc3jIod8pIWFH7-uiaU

To use this API token:

Set your username to __token__
Set your password to the token value, including the pypi- prefix

For example, if you are using Twine to upload your projects to PyPI, set up your $HOME/.pypirc file like this:

[pypi]
  username = __token__
  password = pypi-AgEIcHlwaS5vcmcCJDcwODM5ZTU4LTRjZjAtNGIyYS05YTllLTMxMWU0OGI5Njg0MQACJXsicGVybWlzc2lvbnMiOiAidXNlciIsICJ2ZXJzaW9uIjogMX0AAAYgJfMHimYQb1kn1OCHZtPQ34NtGc3jIod8pIWFH7-uiaU

"""
from setuptools import setup, find_packages

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name='dann5',    # This is the name of your PyPI-package.
    version='2.1.0',                          # Update the version number for new releases
    author="voya-voja",
    author_email="nebojsa@nand5.onmicrosoft.com",
    description="A Library of Quantum computing types",
    long_description=long_description,
    long_description_content_type='text/markdown',
    url="https://github.com/voya-voja/d5o2",
    license='GPLv3',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Development Status :: 3 - Alpha',
        'Environment :: Win32 (MS Windows)',
        'Environment :: MacOS X',
        'Intended Audience :: Developers',
        'Intended Audience :: Information Technology',
        'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
        'Natural Language :: English',
        'Operating System :: Microsoft :: Windows :: Windows 10',
        'Operating System :: MacOS :: MacOS X',
        'Programming Language :: C++',
        'Programming Language :: Python :: 3.10',
        'Topic :: Software Development :: Libraries :: Application Frameworks',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development',
        'Topic :: Scientific/Engineering :: Interface Engine/Protocol Translator',
    ],
    keywords='quantum coding development types',
    project_urls={
        "Bug Tracker": "https://github.com/voya-voja/d5o2/issues",
    },
    packages=['dann5'],
#    packages=find_packages(include=['dann5', 'dann5.*']),
#    package_dir={'dann5': 'dann5'},
    include_package_data=True,
    package_data={
        'dann5': ['d5o2.cp310-win_amd64.pyd'],
        'dann5': ['d5o2.cpython-310-darwin.so'],
    },
    install_requires=['pybind11'],
    python_requires=">=3.10",
#    data_files=[('Lib/site-packages/dann5', ['x64/Release/d5o2.cp39-win_amd64.pyd'])],
#    scripts=['dann5/d5o2.cp39-win_amd64.pyd']                  # The name of your scipt, and also the command you'll be using for calling it
)
