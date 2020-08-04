Summary: Simple DirectMedia Layer - MNG Loading Library
Name: SDL_mng
Version: 0.2.8
Release: 1%{?dist}
License: LGPLv2+
URL: https://github.com/dulsi/SDL_mng
Source0: http://www.identicalsoftware.com/btbuilder/%{name}-%{version}.tgz
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: SDL2-devel
BuildRequires: libpng-devel
BuildRequires: SDL2_image-devel
BuildRequires: cmake

%description
This is a simple library to load mng animations as SDL surfaces.

%package devel
Summary: Libraries and includes for SDL MNG development
Requires: %{name}%{?_isa} = %{version}-%{release}
Requires: SDL2-devel%{?_isa}
Requires: pkgconfig

%description devel
This package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q

%build
export CXXFLAGS="%{optflags} -Wl,--as-needed"
%cmake
%cmake_build

%install
%cmake_install

%ldconfig_scriptlets

%files
%doc README ChangeLog
%license LICENSE
%{_libdir}/lib*.so.*

%files devel
%{_includedir}/SDL2/*
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/%{name}.pc

%changelog
* Tue Aug 04 2020 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.8-1
- Switch to cmake_build and cmake_install macros

* Mon Aug 03 2020 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.7-15
- Switch to cmake_build and cmake_install macros

* Sat Aug 01 2020 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-14
- Second attempt - Rebuilt for
  https://fedoraproject.org/wiki/Fedora_33_Mass_Rebuild

* Mon Jul 27 2020 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-13
- Rebuilt for https://fedoraproject.org/wiki/Fedora_33_Mass_Rebuild

* Tue Jan 28 2020 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-12
- Rebuilt for https://fedoraproject.org/wiki/Fedora_32_Mass_Rebuild

* Wed Jul 24 2019 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-11
- Rebuilt for https://fedoraproject.org/wiki/Fedora_31_Mass_Rebuild

* Thu Jan 31 2019 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-10
- Rebuilt for https://fedoraproject.org/wiki/Fedora_30_Mass_Rebuild

* Wed Jan 23 2019 Björn Esser <besser82@fedoraproject.org> - 0.2.7-9
- Append curdir to CMake invokation. (#1668512)

* Thu Jul 12 2018 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-8
- Rebuilt for https://fedoraproject.org/wiki/Fedora_29_Mass_Rebuild

* Wed Mar 07 2018 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.7-7
- Add build requires g++ to build

* Wed Feb 07 2018 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-6
- Rebuilt for https://fedoraproject.org/wiki/Fedora_28_Mass_Rebuild

* Wed Aug 02 2017 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-5
- Rebuilt for https://fedoraproject.org/wiki/Fedora_27_Binutils_Mass_Rebuild

* Wed Jul 26 2017 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-4
- Rebuilt for https://fedoraproject.org/wiki/Fedora_27_Mass_Rebuild

* Fri Feb 10 2017 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.7-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_26_Mass_Rebuild

* Sun Oct 09 2016 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.7-2
- Add requires cmake to build

* Sun Oct 09 2016 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.7-1
- Convert to cmake

* Fri Aug 26 2016 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.6-2
- Fix unused direct shlib dependency

* Sat Aug 13 2016 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.6-1
- Updated to new version

* Tue Jul 12 2016 Dennis Payne <dulsi@identicalsoftware.com> - 0.2.5-1
- Updated to new version

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
