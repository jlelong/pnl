This is PNL, a library for scientific computing. PNL is free software:
you can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License. 

PNL is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.


# Compilation and installation

See the INSTALL file

If you have cloned the git repository, you need to compile the
documentation yourself by going to the directory `man` and 
    - for the pdf version, run `make` (you need a LaTeX compiler). 
    - for the html version, run `make html` (you need tex4ht). 

# Using the library under Unix

You must have installed the library before proceeding to this step. 
If your want to use the library to develop your own code, here is one
way of doing it.

- create a new directory at the top level.

- Copy the Makefile located in `perso/` to your new directory and
edit it according to your needs. 
To define your target just add the executable name, say `my-exec`, to the `BINS`
list and create an entry
```
my_exec_OBJS 
```
carrying the list of .o files needed to create your executable.
Note that the '-' in executable names are replaced by '_' to create the
corresponding `_OBJS` variable
   

If you encounter any problem or want to report some bug, contact 
Jerome Lelong <jerome.lelong@gmail.com>


