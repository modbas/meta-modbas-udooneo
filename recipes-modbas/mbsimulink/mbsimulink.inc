SUMMARY = "MODBAS RT-Linux Simulink"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://COPYING;md5=05b4ac8839393b3870e9e8bb6fec8aa8"

SRC_URI = "file://libgtrt-1.0.tgz \
           file://COPYING \
          "

S = "${WORKDIR}/libgtrt-1.0"

DEPENDS = "glib-2.0 pkgconfig"

inherit autotools