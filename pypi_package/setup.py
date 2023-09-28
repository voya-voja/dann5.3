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
    version='2.2.3',                          # Update the version number for new releases
    author="voya-voja",
    author_email="nebojsa@nand5.onmicrosoft.com",
    description="A Library of Quantum computing types for programming",
    long_description=long_description,
    long_description_content_type='text/markdown',
    url="https://github.com/voya-voja/d5o2",
    license='GPLv3',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Development Status :: 4 - Beta',
        'Environment :: OpenStack',
        'Environment :: Plugins',
        'Environment :: Other Environment',
        'Framework :: IPython',
        'Framework :: Jupyter :: JupyterLab',
        'Framework :: Jupyter :: JupyterLab :: Extensions',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'Intended Audience :: Information Technology',
        'Intended Audience :: Science/Research',
        'Intended Audience :: Science/Research',
        'Intended Audience :: Financial and Insurance Industry',
        'Intended Audience :: Healthcare Industry',
        'Intended Audience :: Telecommunications Industry',
        'Intended Audience :: Other Audience',
        'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
        'Natural Language :: English',
        'Operating System :: Microsoft :: Windows :: Windows 10',
        'Operating System :: Microsoft :: Windows :: Windows 11',
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: C++',
        'Programming Language :: Python :: 3.10',
        'Topic :: Security',
        'Topic :: Software Development',
        'Topic :: Scientific/Engineering :: Interface Engine/Protocol Translator',
        'Topic :: Scientific/Engineering :: Artificial Intelligence',
        'Topic :: Scientific/Engineering :: Information Analysis',
        'Topic :: Scientific/Engineering :: Mathematics',
        'Topic :: Scientific/Engineering :: Physics',
        'Topic :: Software Development :: Libraries :: Application Frameworks',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development :: Build Tools',
        'Topic :: Software Development :: Code Generators',
        'Topic :: Software Development :: Compilers',
        'Topic :: Software Development :: Debuggers',
        'Topic :: Software Development :: Interpreters',
        'Topic :: Software Development :: Embedded Systems',
        'Topic :: System :: Distributed Computing',
    ],
    keywords='quantum computing programming coding language development types dwave ibm azure',
    project_urls={
        "Bug Tracker": "https://github.com/voya-voja/d5o2/issues",
    },
    packages=['dann5'],
#    packages=find_packages(include=['dann5', 'dann5.*']),
#    package_dir={'dann5': 'dann5'},
    include_package_data=True,
    package_data={
        'dann5': ['d5o2.cp310-win_amd64.pyd', 'd5o2.cpython-310-darwin.so', 'd5o2.so'],
#        'dann5': ['d5o2.cpython-310-darwin.so'],
#        'dann5': ['d5o2.so'],
    },
    install_requires=['pybind11'],
    python_requires=">=3.10",
#    data_files=[('Lib/site-packages/dann5', ['x64/Release/d5o2.cp39-win_amd64.pyd'])],
#    scripts=['dann5/d5o2.cp39-win_amd64.pyd']                  # The name of your scipt, and also the command you'll be using for calling it
)
