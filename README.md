**Requirements**

cmake 3.10.0 or older.\
gcc 13.2.1 or older.\
Required packages:
```bash
$ apt-get install boost-devel boost-asio-devel librpm-devel
```
And additionl package for testing:
```bash
$ apt-get install libgtest-devel
```

**Library and utility compilation**

Compilation:
```bash
$ mkdir build
cd build/
cmake ..
cmake --build .
```

Note that by default path to librpm is set to "/usr/include/rpm/". If you have different librpm installation path then you can specify your path by running:
``$ cmake .. -DPATH_TO_LIBRPM=<Your path>``

***Library and utility installation***

Run:
```bash
# cmake --install .
/sbin/ldconfig
```

If you don't need to install both library and utility then you can specify what component you want to install by running: ``cmake --install . --component <component>``

Proided components are the following:\
  library - shared libraries with header file.\
  utility - utlity executable.

Example of library installation:
```bash
# cmake --install . --component library
/sbin/ldconfig
```

**Utility usage**

Open command line and write "pkgcmp".\
Next enter branches you want to compare.\
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
**Library test**

Run from project root directory:
```bash
$  mkdir Package-compare-lib/build | cd Package-compare-lib/build
cmake .. -DENABLE_TEST=ON
cmake --build .
./test_lib
```
