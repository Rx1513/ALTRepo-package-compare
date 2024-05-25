**Library compilation:**
Generate object file from source code:
```bash
$ g++ -fPIC -g -c -Wall Package\ compare\ lib/lib.cpp
```
Compile object file to a shared library:
```bash
$ g++ -shared -Wl,-soname,libpckcmp.so.1 -o libpckcmp.so.1.0 lib.o -lc
```
**Library instalation**
Put libpckcmp.so.1.0 into usr/lib directory, create symbolic link to that library named "libpckcmp.so.1" and run:
```bash
# /sbin/ldconfig -n /usr/lib/
```
**Utility compilaton**
Run comand below: 
```bash
$ g++ -user/lib -Wall -o pckcmp CLI-utility/main.cpp -l:libpckcmp.so.1 -lboost_system -lssl -lcrypto -pthread
```
**Utility installation**
Put "pckcmp" into usr/bin directory.
