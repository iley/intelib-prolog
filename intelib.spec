Name: intelib
Version: 0.6.22
Release: 0

Summary: A C++ library which lets one do Lisp programming within C++
License: GPL; the library part is under LGPL
Group: System/Libraries
Url: http://www.intelib.org/

Source: ftp://ftp.croco.net/pub/intelib/%name-%version.tgz
Requires: libreadline >= 4.0
BuildRequires: libreadline-devel >= 4.0  gcc-c++

%description
InteLib is a library of C++ classes which lets you do Lisp programming
within your C++ program without any additional preprocessing, without
all those calling conventions etc. You can write C++ code (that is, a
code which is accepted by your C++ compiler) thinking in a "Lisp mode" and
the code you write will look much like Lisp code altough it will be pure
C++.

%package devel
Summary: Libraries, includes to develop applications with %name
Group: Development/Libraries
Requires: %name = %version

%description devel
The %name-devel package contains the header files and static libraries for
building applications which use %name.

%prep
%setup -q

%build
make

%install
make DESTDIR=%buildroot PREFIX=%prefix INSTALLMODE=native install
rm -rf %buildroot%_includedir/%name/refal

%post 
%post_ldconfig

%postun
%postun_ldconfig

%files
%_bindir/ill
%_bindir/illdef._ls
%_bindir/nill

%files devel
%_libdir/lib*.a
%dir %_includedir/%name
%_includedir/%name/sexpress
%_includedir/%name/lisp
%_includedir/%name/ill
%_includedir/%name/*.hpp

%changelog
* Tue Apr 18 2006 Andrey Vikt Stolyarov <avst at cs.msu.su> 0.5.76-2
- removed the alt release tag

* Thu Mar 30 2006 Alex V. Myltsev <avm@altlinux.ru> 0.5.75-alt1
- Initial build for Sisyphus.

* Wed Mar 29 2006 Andrey Vikt. Stolyarov <avst at cs.msu.su>
- First draft of the spec file
