DESCRIPTION = ""
LICENSE = ""
RECIPE_TYPES = "native"

SRC_URI = "https://bitbucket.org/pypa/setuptools/get/${PV}.tar.gz"
FILES_${PN} += "/lib/python/"

do_compile() {
        python setup.py build
}

do_install() {
        python setup.py install --home ${D}
}