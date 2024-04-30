2.4	Program Installation

Version 2.10 of the program is available as the compressed tar file “vpulse_v2p10_043024.tar.gz” and may be downloaded from the URL provided in reference [1].
To install the program and create the executable “vpulse”, enter Example command line [3] in the directory in which you wish to locate the program.

$ tar -xvzf vpulse_v2p10_043024.tar.gz						Example command line [3]

This will create a directory vpulse_v2p10 and extract its directory structure:

Documentation/		example/		plotting_routines/
README.txt			include/		src/

Navigate to the “src” subdirectory, and issue the following two UNIX commands:

$ make																Example command line [4]
$ make clean														Example command line [5]

Issuing these two commands will create the executable “vpulse” and delete object files no longer needed. In addition, Example command line [4] will also attempt to create a symbolic link to vpulse in your $HOME/bin directory if this directory exists. Assuming your $HOME/bin directory is contained in your executable search path (UNIX PATH variable), this will allow you to execute vpulse from any of your subdirectories using a command syntax similar Example command line [1] in Section 2.1 of the document in the Documentation directory.

$ vpulse 125e6 45 20 20 50 0 100 250 250					Example command line [1]

This README.txt file is contained in the top level directory of vpulse_v2p10. A script file "runline_example.sh" containing a sample program line is contained in the example directory, and the documentation file in Portable Document Format is contained in the Documentation directory.
 
References:

[1]	https://1drv.ms/u/s!AnM-GsAEZPoSr22p4VYo2o08Rw-7?e=aLSg3N

1/2/2024
rev. 4/30/2024