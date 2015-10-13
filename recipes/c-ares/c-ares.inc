DESCRIPTION = "c-ares is a C library for asynchronous DNS requests (including name resolves)"
LICENSE = "MIT"
RECIPE_TYPES = "machine sdk native"

inherit c autotools library

SRC_URI = "http://c-ares.haxx.se/download/c-ares-${PV}.tar.gz"
SRC_URI:>HOST_LIBC_mingw = " file://0001-deps-provide-TXT-chunk-info-in-c-ares.patch "

LIBRARY_NAME="libcares"
LIBRARY_VERSION = "2"
DEPENDS_${PN} = "libc"
RDEPENDS_${PN} = "libc"
