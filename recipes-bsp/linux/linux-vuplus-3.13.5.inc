DESCRIPTION = "Linux kernel for ${MACHINE}"
LICENSE = "GPL-2.0-only"
SECTION = "kernel"
KV = "3.13.5"
PR_INC = ".7"

inherit kernel machine_kernel_pr

SRC_URI[md5sum] = "19e9956653437b99b4fa6ec3e16a3e99"
SRC_URI[sha256sum] = "ef7fb307582ff243aacff8a13025fe028634aaf650ada309991ae03622962f61"

LIC_FILES_CHKSUM = "file://${WORKDIR}/linux/COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

FILESEXTRAPATHS:prepend := "${THISDIR}/linux-vuplus-${KV}:"

SRC_URI += "http://downloads.openpli.org/archive/vuplus/stblinux-${KV}.tar.bz2 \
	file://nfs-max-rwsize-8k.patch \
	file://build-with-gcc12-fixes.patch \
	file://rt2800usb_fix_warn_tx_status_timeout_to_dbg.patch \
	file://linux-tcp_output.patch \
	file://linux-3.13-gcc-4.9.3-build-error-fixed.patch \
	file://rtl8712-fix-warnings.patch \
	file://0001-Support-TBS-USB-drivers-3.13.patch \
	file://0001-STV-Add-PLS-support.patch \
	file://0001-STV-Add-SNR-Signal-report-parameters.patch \
	file://0001-stv090x-optimized-TS-sync-control.patch \
	file://0002-mips-kernel-fpu-gcc7.patch \
	file://0003-mips-kernel-ilog2-gcc7.patch \
	file://2-2-stv090x-on-tuning-lock-return-correct-tuned-paramaters-like-freq-sr-fec-rolloff-etc.patch \
	file://hvr-9x5-19x5-22x5-kernel-3.13.patch \
	${@bb.utils.contains("MACHINE_FEATURES", "dvbproxy", "file://linux_dvb_adapter.patch", "", d)} \
	file://defconfig \
	file://fix-build-with-binutils-2.41.patch \
	"

S = "${WORKDIR}/linux"

export OS = "Linux"
KERNEL_IMAGETYPE = "vmlinux"
KERNEL_OUTPUT = "vmlinux"
KERNEL_OUTPUT_DIR = "."
KERNEL_OBJECT_SUFFIX = "ko"
KERNEL_IMAGEDEST = "tmp"

KERNEL_EXTRA_ARGS = "EXTRA_CFLAGS=-Wno-attribute-alias"
KERNEL_PACKAGE_NAME = "kernel"

FILES:${KERNEL_PACKAGE_NAME}-image = "/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}.gz"

do_configure:prepend() {
	if ${@bb.utils.contains("MACHINE_FEATURES", 'dvbproxy', 'true', 'false', d)}; then
		sed -i "s/bmem=220M@512M/bmem=384M@512M/g" ${WORKDIR}/defconfig
	fi
}

kernel_do_install:append() {
    ${STRIP} ${D}/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}-${KERNEL_VERSION}
    gzip -9c ${D}/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}-${KERNEL_VERSION} > ${D}/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}.gz
    rm ${D}/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}-${KERNEL_VERSION}
}

pkg_postinst:kernel-image () {
	if [ -d /proc/stb ] ; then
		flash_eraseall /dev/mtd2
		nandwrite -p /dev/mtd2 /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}.gz
	fi
	rm -f /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}.gz
	true
}

do_rm_work() {
}
# extra tasks
addtask kernel_link_images after do_compile before do_install
