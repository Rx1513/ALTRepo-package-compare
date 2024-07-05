**Library compilation**

Required packages:
```bash
$ apt-get install boost-devel boost-asio-devel librpm-devel
```
Generate object file from source code:
```bash
$ g++ -I/usr/include/rpm/ -fPIC -g -c -Wall Package\ compare\ lib/lib.cpp
```

Note that "/usr/include/rpm/" is a standart directory for librmp headers. If you have different installation path for librpm then you should manually replace "-I/usr/include/rpm/" with this path.

Compile object file to a shared library:
```bash
$ g++ -shared -Wl,-soname,libpkgcmp.so.1 -o libpkgcmp.so.1.0 lib.o -lc
```
**Library instalation**

Run:
```bash
# mv libpkgcmp.so.1.0 /usr/lib64/
ln -s /usr/lib64/libpkgcmp.so.1.0 /usr/lib64/libpkgcmp.so.1
/sbin/ldconfig -n /usr/lib64/
```
**Utility compilaton**

Run the command below: 
```bash
$ g++ -Wall -o pkgcmp CLI-utility/main.cpp -l:libpkgcmp.so.1 -lboost_system -lssl -lcrypto -pthread -lrpmbuild -lrpm -lrpmio -lpopt
```

**Utility installation**

```bash
# mv pkgcmp /usr/bin
```

**Utility usage**

Open command line and write "pkgcmp".

Next enter branches you want to compare.

Wait until package lists will be compared.

The output JSON is:

```JSON
{
  "package_list": [
    {
      "arch": "string",
      "packages_presented_only_in_first_branch":
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
  "second_branch": "string"
}
```
