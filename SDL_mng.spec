Summary: Simple DirectMedia Layer - MNG Loading Library
Name: SDL_mng
Version: 0.2.4
Release: 2%{?dist}
License: LGPLv2+
URL: https://github.com/dulsi/SDL_mng
Source0: http://www.identicalsoftware.com/btbuilder/%{name}-%{version}.tgz
BuildRequires: SDL-devel
BuildRequires: libpng-devel
BuildRequires: SDL_image-devel

%description
This is a simple library to load mng animations as SDL surfaces.

%package devel
Summary: Libraries and includes for SDL MNG development.
Requires: %{name}%{?_isa} = %{version}-%{release}
Requires: SDL-devel%{?_isa}
Requires: pkgconfig

%description devel
This package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'
find $RPM_BUILD_ROOT -name 'lib*.a' -exec rm -f {} ';'

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%doc README ChangeLog
%license LICENSE
%{_libdir}/lib*.so.*

%files devel
%{_includedir}/SDL/*
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/%{name}.pc

%changelog
* Wed Oct 28 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.4-2
- Fix typo in requires

* Wed Sep 23 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.4-1
- Added pckconfig file
- Removed Group tags
- Changed description of devel package
- Reordered post and postun section

* Sat Aug 15 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.3-1
- Do not package static libraries
- Made the requirement for the devel package specific to the version.
- Specify the license file properly.
- Post install ldconfig step added.

* Wed Jun 3 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.2-3
- Do not package .la files
- Use make_install instead of makeinstall

* Sun May 31 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.2-2
- Fixed source file to tgz not tar.gz

* Sun May 31 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.2-1
- Initial spec
