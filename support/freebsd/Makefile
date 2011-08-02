# New ports collection makefile for:    mod_chxj
# Date created:     15 March 2011
# Whom:             Atsushi Konno <konno@qsdn.co.jp>
#
# $FreeBSD$
#

PORTNAME=       mod_chxj
PORTVERSION=    0.14.2rc0
PORTREVISION=   1
PORTEPOCH=      1
CATEGORIES=     www
MASTER_SITES=   ${MASTER_SITE_SOURCEFORGE_JP:=modchxj/52522/}
PKGNAMEPREFIX=  ${APACHE_PKGNAMEPREFIX}
DISTNAME=       mod-chxj_0.14.2rc0
EXTRACT_SUFX=   .src.tar.gz

MAINTAINER=     konno@qsdn.co.jp
COMMENT=        CompactHTML to XHTML,HTML convert module for apache2

LIB_DEPENDS=	apr-1:${PORTSDIR}/devel/apr1
LIB_DEPENDS+=	aprutil-1:${PORTSDIR}/devel/apr1
LIB_DEPENDS+=	ssl:${PORTSDIR}/security/openssl
LIB_DEPENDS+=	crypto:${PORTSDIR}/security/openssl
LIB_DEPENDS+=	MagickWand:${PORTSDIR}/graphics/ImageMagick
LIB_DEPENDS+=	MagickCore:${PORTSDIR}/graphics/ImageMagick

MAKE_JOBS_SAFE=  yes

OPTIONS=	USE_MYSQL_COOKIE	"Use Cookie Simulator with MySQL"	on \
		USE_MEMCACHED_COOKIE	"Use Cookie Simulator with Memcached"	on 

PKGDEINSTALL=	${PKGINSTALL}

NO_INSTALL_MANPAGES= yes
HAS_CONFIGURE=  yes
CONFIGURE_ARGS=	--prefix=${LOCALBASE}

.include <bsd.port.pre.mk>
.if defined(WITH_USE_MYSQL_COOKIE)
CONFIGURE_ARGS+=	--enable-mysql-cookie \
			--with-mysql-header=${LOCALBASE}/include/mysql \
			--with-mysql-lib-dir=${LOCALBASE}/lib/mysql 
LIB_DEPENDS+=		mysqlclient_r:${PORTSDIR}/databases/mysql51-client
.endif

.if defined(WITH_USE_MEMCACHED_COOKIE)
CONFIGURE_ARGS+=	--enable-memcache-cookie \
			--with-memcached-header=${LOCALBASE}/include \
			--with-memcached-lib-dir=${LOCALBASE}/lib
LIB_DEPENDS+=		memcached:${PORTSDIR}/databases/libmemcached
.endif

AP_FAST_BUILD=  yes

PLIST_FILES=    ${APACHEETCDIR}/apache.conf.mod_chxj \
	${APACHEMODDIR}/mod_chxj.so \
	${APACHEETCDIR}/chxj/device_data.xml \
	${APACHEETCDIR}/chxj/emoji.xml

USE_APACHE=     2.0+

do-build:
	(cd $(WRKSRC); make)

do-install:
	(cd ${WRKSRC}; make install)
	${INSTALL_DATA} ${FILESDIR}/apache.conf.${PORTNAME} ${LOCALBASE}/${APACHEETCDIR}/
	${MKDIR} -p ${LOCALBASE}/${APACHEETCDIR}/chxj
	${INSTALL_DATA} ${FILESDIR}/device_data.xml ${LOCALBASE}/${APACHEETCDIR}/chxj/
	${INSTALL_DATA} ${FILESDIR}/emoji.xml ${LOCALBASE}/${APACHEETCDIR}/chxj/

post-install:
.if !defined(PACKAGE_BUILDING)
	@${SETENV} ${SH} ${PKGINSTALL} ${PKGNAME} POST-INSTALL
.endif
	@${CAT} ${PKGMESSAGE}

.include <bsd.port.post.mk>
