DESCRIPTION = "Linux kernel for ${MACHINE}"
LICENSE = "GPLv2"
SECTION = "kernel"
KV = "4.1.20"
PR_INC = ".1"

SRC_URI[md5sum] = "b3e5d84c2d1b16d191a7adfe96951b2f"
SRC_URI[sha256sum] = "74648ddfe77e95e06ca3ab1b398ff26f7dd317767dbab3d27ebdd2b9d33bba51"

LIC_FILES_CHKSUM = "file://${WORKDIR}/linux/COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-vuplus-${KV}:"

KSRC_VER = "4.1-1.9"
SRC_URI += "http://downloads.openpli.org/archive/vuplus/stblinux-${KSRC_VER}.tar.bz2 \
	file://linux_dvb-core.patch \
	file://bcmgenet-recovery-fix.patch \
	${@bb.utils.contains("MACHINE_FEATURES", "dvbproxy", "file://linux_dvb_adapter.patch", "", d)} \
	file://kernel-gcc6.patch \
	file://kernel-gcc7.patch \
	file://kernel-gcc8.patch \
	file://0001-regmap-add-regmap_write_bits.patch \
	file://0002-af9035-fix-device-order-in-ID-list.patch \
	file://0003-Add-support-for-dvb-usb-stick-Hauppauge-WinTV-soloHD.patch \
	file://0004-af9035-add-USB-ID-07ca-0337-AVerMedia-HD-Volar-A867.patch \
	file://0005-Add-support-for-EVOLVEO-XtraTV-stick.patch \
	file://0006-dib8000-Add-support-for-Mygica-Geniatech-S2870.patch \
	file://0007-dib0700-add-USB-ID-for-another-STK8096-PVR-ref-desig.patch \
	file://0008-add-Hama-Hybrid-DVB-T-Stick-support.patch \
	file://0009-Add-Terratec-H7-Revision-4-to-DVBSky-driver.patch \
	file://0010-media-Added-support-for-the-TerraTec-T1-DVB-T-USB-tu.patch \
	file://0011-media-tda18250-support-for-new-silicon-tuner.patch \
	file://0012-media-dib0700-add-support-for-Xbox-One-Digital-TV-Tu.patch \
	file://0013-mn88472-Fix-possible-leak-in-mn88472_init.patch \
	file://0014-staging-media-Remove-unneeded-parentheses.patch \
	file://0015-staging-media-mn88472-simplify-NULL-tests.patch \
	file://0016-mn88472-fix-typo.patch \
	file://0017-mn88472-finalize-driver.patch \
	file://0018-Add-support-for-dvb-usb-stick-Hauppauge-WinTV-dualHD.patch \
	file://0001-dvb-usb-fix-a867.patch \
	file://0001-Support-TBS-USB-drivers-for-4.1-kernel.patch \
	file://0001-TBS-fixes-for-4.1-kernel.patch \
	file://0001-STV-Add-PLS-support.patch \
	file://0001-STV-Add-SNR-Signal-report-parameters.patch \
	file://blindscan2.patch \
	file://0001-stv090x-optimized-TS-sync-control.patch \
	file://0002-log2-give-up-on-gcc-constant-optimizations.patch \
	file://0003-dont-mark-register-as-const.patch \
	file://0004-make-yyloc-declaration-extern.patch \
	file://kernel-add-support-for-gcc9.patch \
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
