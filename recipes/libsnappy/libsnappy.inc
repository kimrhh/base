DESCRIPTION = "nappy is a compression/decompression library. It does not aim for maximum compression, or compatibility with any other compression library; instead, it aims for very high speeds and reasonable compression."
LICENSE = "BSD-3"

inherit c++ autotools library

SRC_URI = "https://github.com/google/snappy/releases/download/${PV}/snappy-${PV}.tar.gz"
S = "${SRCDIR}/snappy-${PV}"

LIBRARY_VERSION = "1"
DEPENDS_${PN} = "libc libgcc-s libm libstdc++"
RDEPENDS_${PN} = "libc libgcc-s libm libstdc++"
