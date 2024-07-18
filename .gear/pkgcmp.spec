%define _unpackaged_files_terminate_build 1
%def_with check
%define abiversion 1

Name: pkgcmp
Version: 1.2
Release: alt1

Summary: CLI utility for comparing packages provided by ALTRepoAPI 
License: GPLv3
Group: Development/Tools
Url: https://github.com/Rx1513/ALTRepo-branch-compare/

# Source: https://github.com/Rx1513/ALTRepo-branch-compare.git
Source: %name-%version.tar

Requires: lib%name%abiversion = %EVR

BuildRequires(pre): rpm-macros-cmake
BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: boost-devel
BuildRequires: boost-asio-devel
BuildRequires: librpm-devel
BuildRequires: libssl-devel
%if_with check
BuildRequires: libgtest-devel
%endif

%description
CLI utility that allows you to check difference in packages between two
specified branches.

%package -n lib%name%abiversion
Summary: C/C++ library for comparing packages provided by ALTRepoAPI
Group: Development/C++

%description -n lib%name%abiversion
%summary.

%package -n lib%name-devel
Summary: Development files for %name library
Group: Development/C++

Requires: lib%name%abiversion = %EVR

%description -n lib%name-devel
%summary that include shared library symlink and header files.

%prep
%setup

%build
%cmake \
%if_with check
	-DENABLE_TEST:BOOL=ON \
%endif
	%nil
%cmake_build

%install
%cmake_install

%check
cd %_builddir/%name-%version/Package-compare-lib/tests/
%_builddir/%name-%version/%_host_alias/Package-compare-lib/test_lib

%files
%_bindir/%name

%files -n lib%name%abiversion
%_libdir/lib%name.so.%abiversion
%_libdir/lib%name.so.%abiversion.0

%files -n lib%name-devel
%_includedir/lib%name.hpp
%_libdir/lib%name.so

%changelog
* Tue Jul 16 2024 Sergey Zhidkih <Rx1513@altlinux.org> 1.2-alt1
- First build for ALT.
