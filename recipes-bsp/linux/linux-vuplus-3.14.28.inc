DESCRIPTION = "Linux kernel for ${MACHINE}"
LICENSE = "GPLv2"
SECTION = "kernel"
KV = "3.14.28"
PR_INC = ".4"

SRC_URI[md5sum] = "1a62540fdd7f23aa2c1ebf119ca71e91"
SRC_URI[sha256sum] = "a464515d14dda772047ddba7900858f535057294903341e2c1774009eaf95005"

LIC_FILES_CHKSUM = "file://${WORKDIR}/linux/COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-vuplus-${KV}:"

KSRC_VER = "3.14-1.8"
SRC_URI += "http://downloads.openpli.org/archive/vuplus/stblinux-${KSRC_VER}.tar.bz2 \
	file://bcm_genet_disable_warn.patch \
	file://linux_dvb-core.patch \
	file://rt2800usb_fix_warn_tx_status_timeout_to_dbg.patch \
	file://usb_core_hub_msleep.patch \
	file://rtl8712_fix_build_error.patch \
	file://0001-Support-TBS-USB-drivers.patch \
	file://0001-STV-Add-PLS-support.patch \
	file://0001-STV-Add-SNR-Signal-report-parameters.patch \
	file://0001-stv090x-optimized-TS-sync-control.patch \
	file://2-2-stv090x-on-tuning-lock-return-correct-tuned-paramaters-like-freq-sr-fec-rolloff-etc.patch \
	${@bb.utils.contains("MACHINE_FEATURES", "dvbproxy", "file://linux_dvb_adapter.patch", "", d)} \
	file://kernel-gcc6.patch \
	file://kernel-gcc7.patch \
	file://kernel-gcc8.patch \
	file://0003-mips-kernel-ilog2-gcc7.patch \
	file://0003-3.x-uaccess-dont-mark-register-as-const.patch \
	file://0004-make-yyloc-declaration-extern.patch \
	file://kernel-add-support-for-gcc9.patch \
	file://0006-makefile-disable-warnings.patch \
	file://${MACHINE}_defconfig \
	"

inherit kernel machine_kernel_pr

S = "${WORKDIR}/linux"

export OS = "Linux"
KERNEL_IMAGETYPE = "zImage"
KERNEL_OUTPUT = "arch/${ARCH}/boot/${KERNEL_IMAGETYPE}"
KERNEL_OBJECT_SUFFIX = "ko"
KERNEL_IMAGEDEST = "tmp"

FILES_${KERNEL_PACKAGE_NAME}-image = "/${KERNEL_IMAGEDEST}/zImage"

do_configure_prepend() {
	oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}_defconfig ${WORKDIR}/defconfig
}

kernel_do_install_append() {
        install -d ${D}/${KERNEL_IMAGEDEST}
        install -m 0755 ${KERNEL_OUTPUT} ${D}/${KERNEL_IMAGEDEST}
}

kernel_do_compile() {
        unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS MACHINE
        oe_runmake ${KERNEL_IMAGETYPE_FOR_MAKE} ${KERNEL_ALT_IMAGETYPE} CC="${KERNEL_CC}" LD="${KERNEL_LD}"
}

pkg_postinst:kernel-image () {
        if [ -d /proc/stb ] ; then
            DEST="/dev/${MTD_KERNEL}"
            if [ -f /proc/cmdline ]; then
                args=`cat /proc/cmdline`
                for x in ${args};
                do
                    case "$x" in
                        root=*)
                            ROOT_DEST="${x#root=}"
                        ;;
                        kernel=*)
                            KERNEL_DEST="${x#kernel=}"
                        ;;
                    esac
                done
            fi

            if echo ${ROOT_DEST} | grep -qi "UUID="; then
                DEVICE=$(blkid | sed -n "/${ROOT_DEST#*=}/s/\([^:]\+\):.*/\\1/p")
                if [ x${DEVICE} != x ]; then
                    grep "^${DEVICE}" /proc/mounts | cut -d " " -f 2
                    ROOT_DEST=`grep "^${DEVICE}" /proc/mounts | cut -d " " -f 2`
                fi
            elif echo ${ROOT_DEST} | grep -q "^/dev/mmcblk"; then
                ROOT_DEST=/boot
            else
                ROOT_DEST=`grep "^${ROOT_DEST}" /proc/mounts | cut -d " " -f 2`
            fi

            if [ -f "${ROOT_DEST}/${KERNEL_DEST}" ]; then
                echo "Kernel is located at ${ROOT_DEST}/${KERNEL_DEST}"
                cp -f /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE} ${ROOT_DEST}/${KERNEL_DEST}
            else
                echo "Kernel should be on flash"
                dd if=/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE} of=/dev/${MTD_KERNEL}
            fi
        fi
        rm -f /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}
        true
}

pkg_postrm_kernel-image () {
}
