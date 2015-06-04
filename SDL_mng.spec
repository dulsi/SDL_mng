Summary: Simple DirectMedia Layer - MNG Loading Library
Name: SDL_mng
Version: 0.2.2
Release: 3%{?dist}
License: LGPLv2+
Group: System Environment/Libraries
Source0: http://www.identicalsoftware.com/btbuilder/%{name}-%{version}.tgz
BuildRequires: SDL-devel
BuildRequires: libpng-devel
BuildRequires: SDL_image-devel

%description
This is a simple library to load mng animations as SDL surfaces.

%package devel
Summary: Libraries and includes for SDL MNG development.
Group: Development/Libraries
Requires: %{name}
Requires: SDL-devel

%description devel
This is a simple library to load mng animations as SDL surfaces.

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%files
%defattr(-,root,root)
%doc README LICENSE
%{_libdir}/lib*.so.*

%files devel
%{_includedir}/SDL/
%{_libdir}/lib*.a
%{_libdir}/lib*.so

%changelog
* Wed Jun 3 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.2-3
- Do not package .la files
- Use %make_install instead of %makeinstall

* Sun May 31 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.2-2
- Fixed source file to tgz not tar.gz

* Sun May 31 2015 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.2-1
- Initial spec
