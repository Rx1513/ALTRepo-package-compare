%define _unpackaged_files_terminate_build 1
%def_with check

Name: package-compare-util
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

%description
Utility that allows you to check difference in packages between two specified branches.

%prep
%setup

%build
%cmake
%cmake_build

%cmake \
%if_with check
	-DENABLE_TEST:BOOL=ON \
%endif
	%nil

%cmake_build

%install
%cmake_install

%check
cd %_builddir/%{name}-%{version}/Package-compare-lib/tests/
%_builddir/%{name}-%{version}/%_host_alias/Package-compare-lib/test_lib

%files
/usr/bin/pkgcmp
/usr/include/libpkgcmp.hpp
/usr/lib64/libpkgcmp.so
/usr/lib64/libpkgcmp.so.1
/usr/lib64/libpkgcmp.so.1.0


%changelog
* Tue Jul 16 2024 Sergey Zhidkih <Rx1513@altlinux.org> 1.2-alt1
- First build for ALT.
