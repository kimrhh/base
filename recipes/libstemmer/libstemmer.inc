DESCRIPTION = ""
LICENSE = ""

inherit make c

SRC_URI = "http://snowball.tartarus.org/dist/libstemmer_c.tgz"
S = "${SRCDIR}/libstemmer_c/"
FILES_${PN} += "${libdir}"

do_install() {
    install -d ${D}/${libdir}
    install -m 0644 ${S}/libstemmer.o ${D}/${libdir}/libstemmer.so
}
