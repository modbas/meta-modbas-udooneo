FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"
SRC_URI += " file://patch-3.14.57-rt58.patch "
SRC_URI += " file://0001-enable-flexcan.patch "
SRC_URI += " file://0001-imx-sema4-wait-queue.patch "
SRC_URI += " file://0001-st-core-rt-preempt.patch "

COMPATIBLE_MACHINE = "udooneo"
