SECTION = "base"
LICENSE = "CLOSED"
PRIORITY = "required"

PR = "r7"

SRC_URI = "http://downloads.openpli.org/archive/vuplus/vmlinuz-initrd_${MACHINE}_${SRCDATE}.tar.gz"

FILES_${PN} = "/boot"

INHIBIT_PACKAGE_STRIP = "1"
PACKAGE_ARCH := "${MACHINE_ARCH}"
