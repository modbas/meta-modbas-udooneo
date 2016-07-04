FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

COMPATIBLE_MACHINE = "udooneo"

SRC_URI_append = "file://php5.conf"

do_install_append() {
    install -m 0644 ${WORKDIR}/php5.conf ${D}/etc/apache2/conf.d
}
