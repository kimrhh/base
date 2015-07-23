DESCRIPTION = "Asio is a cross-platform C++ library for network and low-level \
    I/O programming that provides developers with a consistent asynchronous \
    model using a modern C++ approach."
HOMEPAGE = "http://think-async.com/Asio"
LICENSE = "BSL-1.0"
DEPENDS = "boost-dev libpthread openssl openssl-dev"

PV_ = "${@d.get('PV', True).replace('.', '-')}"
SRC_URI = "https://github.com/chriskohlhoff/asio/archive/asio-${PV_}.tar.gz"
S = "${SRCDIR}/asio-asio-${PV_}/asio"

require conf/fetch/sourceforge.conf
inherit autotools autotools-autoreconf c++
