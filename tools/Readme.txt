This modules requires the ICU library. You may download installation files from:
http://site.icu-project.org/
(Tested with icu4c-4_8_1_1-Win32-msvc10.zip)
I had to modify the "include\unicode\pwin32.h" file in order to replace the "#define U_SIZEOF_WCHAR_T 2" statement to "#define U_SIZEOF_WCHAR_T 1". Doing that the compiler uses the same UChar type as in the provided lib file.

The provided script (node-gyp.bat) requires two environment variables in order to run properly:
NODE_ROOT: Points to the root of the Node's git repo. Download node from git (https://github.com/joyent/node) and build it using the vcbuild.bat script.
ICU_ROOT: The root of the extracted ICU zip file so that the script can locate %ICU_ROOT%\include\unicode\pwin32.h.

To generate a visual studio 2010 project run
node-gyp.bat
If you want to generate the project and build it at once do:
node-gyp.bat make

 