# This spec file was generated using Kpp
# If you find any problems with this spec file please report
# the error to ian geiser <geiseri@msoe.edu>
Summary:   lineakd is a daemon that allows you to configure a keyboard that has multimedia, easy access, and internet keys. 
Name:      lineakd
Version:   0.8
Release:   1mdk
Copyright: GPL
Vendor:    Sheldon Lee Wen <leewsb@hotmail.com>
Url:       http://lineak.sourceforge.net

Packager:  Sheldon Lee Wen <leewsb@hotmail.com>
Group:     Utilities/System
Source:    lineakd-0.8beta4.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
Reimplementation of the lineakd daemon. Support toggleable keys
 and on screen displays as well as local message passing instead of signals.

%prep
%setup
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" ./configure \
                --prefix=/usr --sysconfdir=/etc  \
                $LOCALFLAGS
%build
# Setup for parallel builds
numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "$numprocs" = "0" ]; then
  numprocs=1
fi

make -j$numprocs

%install
make install-strip DESTDIR=$RPM_BUILD_ROOT

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > $RPM_BUILD_DIR/file.list.lineakd
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.lineakd
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.lineakd
#cat $RPM_BUILD_DIR/file.list.lineakd | sed 's/lineakd\.1$/lineakd\.1\.bz2/' >> $RPM_BUILD_DIR/file.list.lineakd.final

%clean
rm -rf $RPM_BUILD_ROOT/*
rm -rf $RPM_BUILD_DIR/lineakd
rm -rf ../file.list.lineakd
#rm -rf ../file.list.lineakd ../file.list.lineakd

%doc AUTHORS COPYING ChangeLog README TODO lineakd.conf.example lineakd.conf.kde.example lineakkb.def.custom_example
%doc %{_mandir}

#%files -f ../file.list.lineakd.final
%files -f ../file.list.lineakd
