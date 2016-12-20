#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/CCS6/tirtos_tivac_2_16_01_14/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/CCS6/ccsv6/ccs_base;C:/Users/csxwin/workspace_v6.1.2/LAB9B/.config
override XDCROOT = D:/CCS6/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/CCS6/tirtos_tivac_2_16_01_14/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/CCS6/ccsv6/ccs_base;C:/Users/csxwin/workspace_v6.1.2/LAB9B/.config;D:/CCS6/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
