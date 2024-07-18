%define _unpackaged_files_terminate_build 1
%def_with check

%define abiversion 1

Name: pkgcmp
Version: 1.2
Release: alt1

Summary: CLI utility for comparing packages provided by ALTRepoAPI
License: GPLv3
Group: Development/C++

Url: https://github.com/Rx1513/ALTRepo-branch-compare/
# Source: https://github.com/Rx1513/ALTRepo-branch-compare.git
Source: %name-%version.tar

BuildRequires(pre): rpm-macros-cmake
BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: boost-devel
BuildRequires: boost-asio-devel
BuildRequires: librpm-devel
BuildRequires: libssl-devel
BuildRequires: libgtest-devel
Requires: libpkgcmp%abiversion = %version-%release

%description
CLI utility that allows you to check difference in packages between two
specified branches.

%package -n libpkgcmp%abiversion
Summary: C/C++ library for comparing packages provided by ALTRepoAPI
Group: Development/C++

%description -n libpkgcmp%abiversion
%summary

%package -n libpkgcmp-devel
Summary: Development files for pkgcmp library
Group: Development/C++
Requires: libpkgcmp%abiversion = %version-%release

%description -n libpkgcmp-devel
%summary that include shared library symlink and header files.

%prep
%setup

%build
%cmake \
%if_with check
	-DENABLE_TEST:BOOL=ON
%endif
%cmake_build

%install
%cmake_install

%check
cd %_builddir/%name-%version/Package-compare-lib/tests/
%_builddir/%name-%version/%_host_alias/Package-compare-lib/test_lib

%files
%_bindir/pkgcmp

%files -n libpkgcmp%abiversion
%_libdir/libpkgcmp.so.1
%_libdir/libpkgcmp.so.1.0

%files -n libpkgcmp-devel
%_includedir/libpkgcmp.hpp
%_libdir/libpkgcmp.so

%changelog
* Tue Jul 16 2024 Sergey Zhidkih <Rx1513@altlinux.org> 1.2-alt1
- First build for ALT.
