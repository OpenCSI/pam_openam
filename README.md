# How to build

In order to build this PAM module, you must have
* git
* A working C compiler
* CMake
* libcurl (and its associated headers)
* libpam (and its associated headers)

Finally, build the module:

	$ git clone https://github.com/OpenCSI/pam_openam
	$ cmake .
	$ make

## Debian

	$ sudo aptitude install build-essential libcurl4-openssl-dev cmake git
	$ git clone https://github.com/OpenCSI/pam_openam
	$ cmake .
	$ make

# Configuration
