require linux-vuplus-3.13.5.inc

MACHINE_KERNEL_PR:append = "${PR_INC}.4"

SRC_URI += " \
	file://brcm_s3_wol.patch \
	file://linux_mtd_bbt_maxblock.patch \
"

COMPATIBLE_MACHINE = "vusolose"
