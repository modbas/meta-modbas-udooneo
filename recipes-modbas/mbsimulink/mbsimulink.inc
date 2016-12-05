SUMMARY = "MODBAS RT-Linux Simulink"
LICENSE = "LGPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=e6a600fd5e1d9cbde2d983680233ad02"

SRC_URI = "file://libmbrt-1.0.tgz \
           file://COPYING \
          "

S = "${WORKDIR}/libmbrt-1.0"

DEPENDS = "glib-2.0 pkgconfig apache2"
#PACKAGES = "${PN} ${PN}-dbg"

inherit autotools

do_install() {
    autotools_do_install
    install -d ${D}/home/root/bin
    install -m 0755 ${S}/debian/bin/*.sh ${D}/home/root/bin
    install -d ${D}/usr/share/apache2/htdocs/mbrtmonitor
    install -m 0644 ${S}/debian/var/www/html/mbrtmonitor/* ${D}/usr/share/apache2/htdocs/mbrtmonitor
}

FILES_${PN} += "/home/root/bin/*"
FILES_${PN} += "/usr/share/apache2/htdocs/mbrtmonitor/*"
