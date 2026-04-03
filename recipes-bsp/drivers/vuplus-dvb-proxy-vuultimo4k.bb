require vuplus-dvb-proxy.inc

KV = "3.14.28"

SRCDATE = "20250706"
SRCDATE_PR = "r1"

pkg_postinst_${PN}_append () {
	if [ ! -f $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko ]; then
		ln -s /lib/modules/${KERNEL_VERSION}/extra/dvb-bcm7444.ko $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko
	fi
}

SRC_URI[md5sum] = "c24a4f8fcf0ba588802f17e7754630d9"
SRC_URI[sha256sum] = "b58f8c81cc115e4556376275952d99a4ac96e374fb921ad2a9a52756c67cddd3"
