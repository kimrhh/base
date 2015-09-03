# -*- mode:python; -*-
DESCRIPTION = "This is the libwebsockets C library for lightweight websocket \
clients and servers. "
HOMEPAGE = "http://libwebsockets.org"
LICENSE = "LGPL-2.1+"

RECIPE_TYPES = "machine"

inherit c cmake auto-package-utils library pkgconfig

SRC_URI = "git://github.com/warmcat/libwebsockets.git;protocol=https;${SRC_REV}"
S = "${SRCDIR}/libwebsockets"

DEPENDS = "openssl libz libm"
DEPENDS_${PN} = "openssl libz libc libcrypto libdl libm libssl"
RDEPENDS_${PN} = "libz libc libcrypto libdl libm libssl "
LIBRARY_VERSION = "5"

AUTO_PACKAGE_UTILS = "\
    ${PN}-test-client \
    ${PN}-test-echo \
    ${PN}-test-fraggle \
    ${PN}-test-ping \
    ${PN}-test-server \
    ${PN}-test-server-extpoll \
"

AUTO_PACKAGE_UTILS_DEPENDS = "libc libcrypto libdl libm libssl libz"
AUTO_PACKAGE_UTILS_RDEPENDS = "libc libcrypto libdl libm libssl libz"
FILES_${PN}-${PN}-test-server += "${datadir}/libwebsockets-test-server/*"

PACKAGES += "${PN}-cmake"
FILES_${PN}-cmake += "${libdir}/cmake/libwebsockets/*"
