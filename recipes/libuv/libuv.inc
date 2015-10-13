DESCRIPTION = "libuv is a multi-platform support library with a focus on asynchronous I/O."
LICENSE = "MIT BSD Artistic-2.0"
RECIPE_TYPES = "machine sdk native"

inherit c autotools library

SRC_URI = "http://dist.libuv.org/dist/v${PV}/libuv-v${PV}.tar.gz"
S = "${SRCDIR}/${PN}-v${PV}"

LIBRARY_VERSION = "11"
DEPENDS_${PN} = "libc"
RDEPENDS_${PN} = "libc"

do_configure[prefuncs] += "do_configure_autogen"
do_configure_autogen() {
    sh autogen.sh
}
