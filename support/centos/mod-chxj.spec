%define version 0.12.18

Summary: CHTML to HDML,XHTML,JHTML convert module for Apache HTTPD.
Name:  mod_chxj
Version:  %{version}
Release:  1
License:  Apache License Version 2.0
Group: System Environment/Daemons
Vendor: Atsushi Konno
URL: http://sourceforge.jp/projects/modchxj/
Source:  http://sourceforge.jp/projects/modchxj/downloads/33637/mod-chxj_%{version}.src.tar.gz
Source1: http://download.tangent.org/libmemcached-0.23.tar.gz
Packager:  Atsushi Konno <konn@sourceforge.jp>
Requires: ImageMagick >= 6.2.8.0, httpd >= 2.2.3, apr >= 1.2.7, apr-util >= 1.2.7, mysql >= 5.0.45
BuildPreReq: mysql-devel httpd-devel ImageMagick-devel gcc-c++ libtool

%description
The contents conversion middleware for domestic main carrying (three careers)
is made as a module for Apache2.0. UserAgent is seen when contents made with
CHTML are output and it converts it into HDML, XHTML, and JHTML (The image :
to JPG, GIF, JPG, GIF from PNG, and PNG and BMP). With QR code generation
function.

%prep
cd $RPM_BUILD_DIR
rm -rf libmemcached-0.23
/bin/gzip -dc $RPM_SOURCE_DIR/libmemcached-0.23.tar.gz | tar -xf -
STATUS=$?
if [ $STATUS -ne 0 ]; then
  exit $STATUS
fi
cd libmemcached-0.23
./configure --with-pic
make
%setup -q -n mod-chxj_%{version}

%build
./configure --enable-mysql-cookie \
            --with-mysql-header=/usr/include/mysql  \
            --with-mysql-lib-dir=/usr/lib/mysql \
            --enable-memcache-cookie \
            --with-memcached-header=../libmemcached-0.23 \
            --with-memcached-lib-dir=../../libmemcached-0.23/libmemcached/.libs \
            --enable-memcached-static
%{__make}

%install
%{__rm} -rf $RPM_BUILD_ROOT
%{__mkdir} -p $RPM_BUILD_ROOT%{_sysconfdir}/httpd/conf.d
%{__mkdir} -p $RPM_BUILD_ROOT%{_sysconfdir}/httpd/chxj
%{__mkdir} -p $RPM_BUILD_ROOT%{_libdir}/httpd/modules
%{__install} -m0644 support/centos/chxj.conf $RPM_BUILD_ROOT%{_sysconfdir}/httpd/conf.d/chxj.conf
%{__install} -m0644 etc/emoji.xml $RPM_BUILD_ROOT%{_sysconfdir}/httpd/chxj/emoji.xml
%{__install} -m0644 etc/device_data.xml $RPM_BUILD_ROOT%{_sysconfdir}/httpd/chxj/device_data.xml
%{__make} -C src make_so
%{__install} -m0755 src/mod_chxj.so $RPM_BUILD_ROOT%{_libdir}/httpd/modules/mod_chxj.so

%files
%defattr(-, root, root)
%doc AUTHORS ChangeLog COPYING
%config(noreplace) %{_sysconfdir}/httpd/conf.d/chxj.conf
%config(noreplace) %{_sysconfdir}/httpd/chxj/emoji.xml
%config(noreplace) %{_sysconfdir}/httpd/chxj/device_data.xml
%{_libdir}/httpd/modules/mod_chxj.so

%changelog
* Fri Nov  7 2008 Atsushi Konno <konn@users.sourceforge.jp> 0.12.17-1
- Added initial package for CentOS5.1.
