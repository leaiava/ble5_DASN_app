#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/lea/ti/simplelink_cc2640r2_sdk_5_10_00_02/source;/home/lea/ti/simplelink_cc2640r2_sdk_5_10_00_02/kernel/tirtos/packages;/home/lea/ti/simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack
override XDCROOT = /home/lea/ti/ccs1040/xdctools_3_62_01_15_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/lea/ti/simplelink_cc2640r2_sdk_5_10_00_02/source;/home/lea/ti/simplelink_cc2640r2_sdk_5_10_00_02/kernel/tirtos/packages;/home/lea/ti/simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack;/home/lea/ti/ccs1040/xdctools_3_62_01_15_core/packages;..
HOSTOS = Linux
endif
