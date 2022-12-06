# Set up the main info portions of the spec
Summary:   lineakd is a daemon that allows you to configure a keyboard that has multimedia, easy access, and internet keys.
Name:      lineakd
Version:   0.8.3
Release:   1
License:   GPL
Vendor:    Sheldon Lee Wen <leewsb@hotmail.com>
Url:       http://lineak.sourceforge.net

Packager:  Sheldon Lee Wen <leewsb@hotmail.com>
Group:     Utilities/System
Source:    %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
Reimplementation of the lineakd daemon. Support toggleable keys
and on screen displays as well as local message passing instead
of signals.

%package -n lineakd-devel
Summary: Development files for libmyth.
Group: Development/Libraries
Requires: lineakd = %{version}

%description -n lineakd-devel
This package contains the header files and libraries for developing
plugins for lineakd.

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" LDFLAGS=-s %{configure}
%{__make}

%install
[  %{buildroot} != "/" ] && rm -rf %{buildroot}
%{makeinstall}

%clean
[  %{buildroot} != "/" ] && rm -rf %{buildroot}
rm -f $RPM_BUILD_DIR/file.list.%{name}

%files
%defattr(-,root,root,-)
%{_sysconfdir}/*
%{_bindir}/*
%{_libdir}/*.so.*
%{_libdir}/*.la
%{_libdir}/lineakd

%doc AUTHORS COPYING ChangeLog README TODO lineakd.conf.example lineakd.conf.kde.example lineakkb.def.custom_example
%doc %{_mandir}/*/*

%files -n lineakd-devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.a
%{_libdir}/*.so

%changelog
* Wed Sep 15  2004 Chris Petersen <rpm@forevermore.net>
- Rework how files are picked, and create a -devel package
* Tue Mar 9  2004 Chris Petersen <rpm@forevermore.net>
- Updated spec file to a better template that should work with redhat and mandrake

