**Library compilation**
Required packages:
```
boost-devel
boost-asio-devel
```
Generate object file from source code:
```bash
$ g++ -fPIC -g -c -Wall Package\ compare\ lib/lib.cpp
```
Compile object file to a shared library:
```bash
$ g++ -shared -Wl,-soname,libpckcmp.so.1 -o libpckcmp.so.1.0 lib.o -lc
```
**Library instalation**

Place "libpckcmp.so.1.0" in usr/lib directory. Create symbolic link to "libpckcmp.so.1.0". Name it "libpckcmp.so.1" and run:
```bash
# /sbin/ldconfig -n /usr/lib/
```
**Utility compilaton**

Run the command below: 
```bash
$ g++ -user/lib -Wall -o pkgcmp CLI-utility/main.cpp -l:libpckcmp.so.1 -lboost_system -lssl -lcrypto -pthread
```
**Utility installation**

Place "pkgcmp" file in usr/bin directory.

**Utility usage**
Open command line and write "pkgcmp".
Next enter branches you want to compare.
Wait until package lists will be compared.
The output format is:
```JSON
{
  "package_list": [
    {
      "arch": "string",
      "packages_presented_only_in_second_branch":
      [
        {
          "name": "string",
          "epoch": 0,
          "version": "string",
          "release": "string",
          "arch": "string",
          "disttag": "string",
          "buildtime": 0,
          "source": "string"
        }
      ],
      "packages_presented_only_in_second_branch":
      [
        {
          "name": "string",
          "epoch": 0,
          "version": "string",
          "release": "string",
          "arch": "string",
          "disttag": "string",
          "buildtime": 0,
          "source": "string"
        }
      ],
      "packages_with_bigger_version_in_first_branch":
      [
        {
          "name": "string",
          "epoch": 0,
          "version": "string",
          "release": "string",
          "arch": "string",
          "disttag": "string",
          "buildtime": 0,
          "source": "string"
        }
      ]
    }
  ], 
  "first_branch": "string",
  "second_branch", secondBranch
}
```
