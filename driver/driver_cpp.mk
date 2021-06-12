# Memory support

# RAM type

ifeq ($(CONFIG_SDRAM),y)
CPPFLAGS += -DCONFIG_SDRAM
endif

ifeq ($(CONFIG_SDDRC),y)
CPPFLAGS += -DCONFIG_SDDRC
endif

ifeq ($(CONFIG_DDRC),y)
CPPFLAGS += -DCONFIG_DDRC
endif

ifeq ($(CONFIG_UMCTL2),y)
CPPFLAGS += -DCONFIG_UMCTL2
endif

ifeq ($(CONFIG_PUBL),y)
CPPFLAGS += -DCONFIG_PUBL
endif

ifeq ($(CONFIG_LPDDR2),y)
CPPFLAGS += -DCONFIG_LPDDR2
endif

ifeq ($(CONFIG_SAMA5D2_LPDDR2),y)
CPPFLAGS += -DCONFIG_SAMA5D2_LPDDR2
endif

ifeq ($(CONFIG_LPDDR1),y)
CPPFLAGS += -DCONFIG_LPDDR1
endif

ifeq ($(CONFIG_LPDDR3),y)
CPPFLAGS += -DCONFIG_LPDDR3
endif

ifeq ($(CONFIG_DDR2),y)
CPPFLAGS += -DCONFIG_DDR2
endif

ifeq ($(CONFIG_DDR3),y)
CPPFLAGS += -DCONFIG_DDR3
endif

ifeq ($(CONFIG_MEM_CLOCK_533), y)
CPPFLAGS += -DCONFIG_MEM_CLOCK=533
endif

ifeq ($(CONFIG_MEM_CLOCK_400), y)
CPPFLAGS += -DCONFIG_MEM_CLOCK=400
endif

ifeq ($(CONFIG_MT47H128M16RT_25E_C), y)
CPPFLAGS += -DCONFIG_MT47H128M16RT_25E_C
endif

ifeq ($(CONFIG_MT41K256M16TW_107), y)
CPPFLAGS += -DCONFIG_MT41K256M16TW_107
endif

ifeq ($(CONFIG_IS43LD16128B_25BLI), y)
CPPFLAGS += -DCONFIG_IS43LD16128B_25BLI
endif

ifeq ($(CONFIG_MT52L256M32D1PF_107_WT_B), y)
CPPFLAGS += -DCONFIG_MT52L256M32D1PF_107_WT_B
endif

ifeq ($(CONFIG_EDB5432BEBH_1DAAT_F_D), y)
CPPFLAGS += -DCONFIG_EDB5432BEBH_1DAAT_F_D
endif

ifeq ($(CONFIG_DDR_EXT_TEMP_RANGE), y)
CPPFLAGS += -DCONFIG_DDR_EXT_TEMP_RANGE
endif

# Support for PSRAM on SAM9263EK EBI1

ifeq ($(CONFIG_PSRAM),y)
CPPFLAGS += -DCONFIG_PSRAM
endif

# 16 bit operation

ifeq ($(CONFIG_SDRAM_16BIT),y)
CPPFLAGS += -DCONFIG_SDRAM_16BIT
endif

# SDRAM/DDR/DDR2 size

ifeq ($(CONFIG_RAM_8MB),y)
CPPFLAGS += -DCONFIG_RAM_8MB
endif

ifeq ($(CONFIG_RAM_32MB),y)
CPPFLAGS += -DCONFIG_RAM_32MB
endif

ifeq ($(CONFIG_RAM_64MB),y)
CPPFLAGS += -DCONFIG_RAM_64MB
endif

ifeq ($(CONFIG_RAM_128MB),y)
CPPFLAGS += -DCONFIG_RAM_128MB
endif

ifeq ($(CONFIG_RAM_256MB),y)
CPPFLAGS += -DCONFIG_RAM_256MB
endif

ifeq ($(CONFIG_RAM_512MB),y)
CPPFLAGS += -DCONFIG_RAM_512MB
endif

# Boot flash type

ifeq ($(CONFIG_DATAFLASH),y)
CPPFLAGS += -DCONFIG_DATAFLASH
endif

ifeq ($(CONFIG_NANDFLASH),y)
CPPFLAGS += -DCONFIG_NANDFLASH
endif

ifeq ($(CONFIG_SDCARD),y)
CPPFLAGS += -DCONFIG_SDCARD
endif

ifeq ($(CONFIG_FLASH),y)
CPPFLAGS += -DCONFIG_FLASH
ASFLAGS += -DCONFIG_FLASH
endif

ifeq ($(CONFIG_LOAD_LINUX),y)
CPPFLAGS += -DCONFIG_LOAD_LINUX
endif

ifeq ($(CONFIG_LOAD_ANDROID),y)
CPPFLAGS += -DCONFIG_LOAD_ANDROID
endif

ifeq ($(CONFIG_LINUX_IMAGE), y)
CPPFLAGS += -DCONFIG_LINUX_IMAGE
endif

ifeq ($(CONFIG_OF_LIBFDT),y)
CPPFLAGS += -DCONFIG_OF_LIBFDT
endif

# Dataflash support
ifeq ($(CONFIG_DATAFLASH_RECOVERY),y)
CPPFLAGS += -DCONFIG_DATAFLASH_RECOVERY
endif

ifeq ($(CONFIG_SMALL_DATAFLASH),y)
CPPFLAGS += -DCONFIG_SMALL_DATAFLASH
endif

ifeq ($(MEMORY),dataflash)
CPPFLAGS += -DAT91C_SPI_CLK=$(SPI_CLK)
CPPFLAGS += -DAT91C_SPI_PCS_DATAFLASH=$(SPI_BOOT)
endif

ifeq ($(MEMORY),dataflashcard)
CPPFLAGS += -DAT91C_SPI_CLK=$(SPI_CLK)
CPPFLAGS += -DAT91C_SPI_PCS_DATAFLASH=$(SPI_BOOT)
endif

# NAND flash support

ifeq ($(CONFIG_NANDFLASH_SMALL_BLOCKS),y)
CPPFLAGS += -DCONFIG_NANDFLASH_SMALL_BLOCKS
endif

ifeq ($(CONFIG_ENABLE_SW_ECC), y)
CPPFLAGS += -DCONFIG_ENABLE_SW_ECC
endif

ifeq ($(CONFIG_USE_PMECC), y)
CPPFLAGS += -DCONFIG_USE_PMECC
endif

ifeq ($(CONFIG_ON_DIE_ECC), y)
CPPFLAGS += -DCONFIG_ON_DIE_ECC
endif

ifeq ($(CONFIG_NANDFLASH_RECOVERY),y)
CPPFLAGS += -DCONFIG_NANDFLASH_RECOVERY
endif

ifeq ($(CONFIG_PMC_COMMON),y)
CPPFLAGS += -DCONFIG_PMC_COMMON
ASFLAGS += -DCONFIG_PMC_COMMON
endif

ifeq ($(CONFIG_PMC_V1),y)
CPPFLAGS += -DCONFIG_PMC_V1
endif

ifeq ($(CONFIG_PMC_V2),y)
CPPFLAGS += -DCONFIG_PMC_V2
endif

ifeq ($(CONFIG_PMC_V3),y)
CPPFLAGS += -DCONFIG_PMC_V3
endif

ifeq ($(CONFIG_PMC_PERIPH_CLK_AT91SAM9),y)
CPPFLAGS += -DCONFIG_PMC_PERIPH_CLK_AT91SAM9
endif

ifeq ($(CONFIG_PMC_PERIPH_CLK_SAM9X5),y)
CPPFLAGS += -DCONFIG_PMC_PCLK_SAM9X5
endif

ifeq ($(CONFIG_PMC_GENERIC_CLK),y)
CPPFLAGS += -DCONFIG_PMC_GENERIC_CLK
endif

ifeq ($(CONFIG_PMC_UTMI_CLK),y)
CPPFLAGS += -DCONFIG_PMC_UTMI_CLK
endif

ifeq ($(CONFIG_PMC_PLL_CLK),y)
CPPFLAGS += -DCONFIG_PMC_PLL_CLK
endif

ifeq ($(CONFIG_PMC_PLL_CLK_SAM9X60),y)
CPPFLAGS += -DCONFIG_PMC_PLL_CLK_SAM9X60
endif

ifeq ($(CONFIG_PMC_MCK_CLK),y)
CPPFLAGS += -DCONFIG_PMC_MCK_CLK
endif

ifeq ($(CONFIG_PMC_MCK_CLK_SAMA7G5),y)
CPPFLAGS += -DCONFIG_PMC_MCK_CLK
endif

ifeq ($(CONFIG_PMECC_CORRECT_BITS_2), y)
CPPFLAGS += -DPMECC_ERROR_CORR_BITS=2
endif

ifeq ($(CONFIG_PMECC_CORRECT_BITS_4), y)
CPPFLAGS += -DPMECC_ERROR_CORR_BITS=4
endif

ifeq ($(CONFIG_PMECC_CORRECT_BITS_8), y)
CPPFLAGS += -DPMECC_ERROR_CORR_BITS=8
endif

ifeq ($(CONFIG_PMECC_CORRECT_BITS_12), y)
CPPFLAGS += -DPMECC_ERROR_CORR_BITS=12
endif

ifeq ($(CONFIG_PMECC_CORRECT_BITS_24), y)
CPPFLAGS += -DPMECC_ERROR_CORR_BITS=24
endif

ifeq ($(CONFIG_PMECC_SECTOR_SIZE_512), y)
CPPFLAGS += -DPMECC_SECTOR_SIZE=512
endif

ifeq ($(CONFIG_PMECC_SECTOR_SIZE_1024), y)
CPPFLAGS += -DPMECC_SECTOR_SIZE=1024
endif

ifeq ($(CONFIG_ONFI_DETECT_SUPPORT), y)
CPPFLAGS += -DCONFIG_ONFI_DETECT_SUPPORT
endif

ifeq ($(CONFIG_USE_ON_DIE_ECC_SUPPORT), y)
CPPFLAGS += -DCONFIG_USE_ON_DIE_ECC_SUPPORT
endif

# Debug related stuff
ifeq ($(CONFIG_DEBUG_INFO),y)
CPPFLAGS += -DBOOTSTRAP_DEBUG_LEVEL=DEBUG_INFO
endif

ifeq ($(CONFIG_DEBUG_LOUD),y)
CPPFLAGS += -DBOOTSTRAP_DEBUG_LEVEL=DEBUG_LOUD
endif

ifeq ($(CONFIG_DEBUG_VERY_LOUD),y)
CPPFLAGS += -DBOOTSTRAP_DEBUG_LEVEL=DEBUG_VERY_LOUD
endif

ifeq ($(CONFIG_DISABLE_WATCHDOG),y)
CPPFLAGS += -DCONFIG_DISABLE_WATCHDOG
endif

ifeq ($(CONFIG_TWI), y)
CPPFLAGS += -DCONFIG_TWI
endif

ifeq ($(CONFIG_TWI0), y)
CPPFLAGS += -DCONFIG_TWI0
endif

ifeq ($(CONFIG_TWI1), y)
CPPFLAGS += -DCONFIG_TWI1
endif

ifeq ($(CONFIG_TWI2), y)
CPPFLAGS += -DCONFIG_TWI2
endif

ifeq ($(CONFIG_TWI3), y)
CPPFLAGS += -DCONFIG_TWI3
endif

ifeq ($(CONFIG_ACT8865), y)
CPPFLAGS += -DCONFIG_ACT8865
endif

ifeq ($(CONFIG_ACT8865_SET_VOLTAGE), y)
CPPFLAGS += -DCONFIG_ACT8865_SET_VOLTAGE
endif

ifeq ($(CONFIG_DISABLE_ACT8865_I2C), y)
CPPFLAGS += -DCONFIG_DISABLE_ACT8865_I2C
endif

ifeq ($(CONFIG_SUSPEND_ACT8945A_CHARGER), y)
CPPFLAGS += -DCONFIG_SUSPEND_ACT8945A_CHARGER
endif

ifeq ($(CONFIG_PM), y)
CPPFLAGS += -DCONFIG_PM
endif

ifeq ($(CONFIG_MACB), y)
CPPFLAGS += -DCONFIG_MACB
endif

ifeq ($(CONFIG_MAC0_PHY), y)
CPPFLAGS += -DCONFIG_MAC0_PHY
endif

ifeq ($(CONFIG_MAC1_PHY), y)
CPPFLAGS += -DCONFIG_MAC1_PHY
endif

ifeq ($(CONFIG_MCP16502),y)
CPPFLAGS += -DCONFIG_MCP16502
endif

ifeq ($(CONFIG_HDMI), y)
CPPFLAGS += -DCONFIG_HDMI
endif

ifeq ($(CONFIG_WM8904), y)
CPPFLAGS += -DCONFIG_WM8904
endif

ifeq ($(CONFIG_LOAD_HW_INFO), y)
CPPFLAGS += -DCONFIG_LOAD_HW_INFO
endif

ifeq ($(CONFIG_HDMI_ON_TWI0), y)
CPPFLAGS += -DCONFIG_HDMI_ON_TWI0
endif

ifeq ($(CONFIG_HDMI_ON_TWI1), y)
CPPFLAGS += -DCONFIG_HDMI_ON_TWI1
endif

ifeq ($(CONFIG_HDMI_ON_TWI2), y)
CPPFLAGS += -DCONFIG_HDMI_ON_TWI2
endif

ifeq ($(CONFIG_HDMI_ON_TWI3), y)
CPPFLAGS += -DCONFIG_HDMI_ON_TWI3
endif

ifeq ($(CONFIG_CODEC_ON_TWI0), y)
CPPFLAGS += -DCONFIG_CODEC_ON_TWI0
endif

ifeq ($(CONFIG_CODEC_ON_TWI1), y)
CPPFLAGS += -DCONFIG_CODEC_ON_TWI1
endif

ifeq ($(CONFIG_CODEC_ON_TWI2), y)
CPPFLAGS += -DCONFIG_CODEC_ON_TWI2
endif

ifeq ($(CONFIG_CODEC_ON_TWI3), y)
CPPFLAGS += -DCONFIG_CODEC_ON_TWI3
endif

ifeq ($(CONFIG_PMIC_ON_TWI0), y)
CPPFLAGS += -DCONFIG_PMIC_ON_TWI0
endif

ifeq ($(CONFIG_PMIC_ON_TWI1), y)
CPPFLAGS += -DCONFIG_PMIC_ON_TWI1
endif

ifeq ($(CONFIG_PMIC_ON_TWI2), y)
CPPFLAGS += -DCONFIG_PMIC_ON_TWI2
endif

ifeq ($(CONFIG_PMIC_ON_TWI3), y)
CPPFLAGS += -DCONFIG_PMIC_ON_TWI3
endif

ifeq ($(CONFIG_EEPROM_ON_TWI0), y)
CPPFLAGS += -DCONFIG_EEPROM_ON_TWI0
endif

ifeq ($(CONFIG_EEPROM_ON_TWI1), y)
CPPFLAGS += -DCONFIG_EEPROM_ON_TWI1
endif

ifeq ($(CONFIG_EEPROM_ON_TWI2), y)
CPPFLAGS += -DCONFIG_EEPROM_ON_TWI2
endif

ifeq ($(CONFIG_EEPROM_ON_TWI3), y)
CPPFLAGS += -DCONFIG_EEPROM_ON_TWI3
endif

ifeq ($(CONFIG_PM_PMIC), y)
CPPFLAGS += -DCONFIG_PM_PMIC
endif

ifeq ($(CONFIG_AUTOCONFIG_TWI_BUS), y)
CPPFLAGS += -DCONFIG_AUTOCONFIG_TWI_BUS
endif

ifeq ($(CONFIG_SECURE), y)
CPPFLAGS += -DCONFIG_SECURE
endif

ifeq ($(CONFIG_BACKUP_MODE), y)
CPPFLAGS += -DCONFIG_BACKUP_MODE
endif

ifeq ($(CONFIG_CPU_HAS_PIO4), y)
CPPFLAGS += -DCONFIG_CPU_HAS_PIO4
endif

ifeq ($(CONFIG_CPU_HAS_HSMCI0), y)
CPPFLAGS += -DCONFIG_CPU_HAS_HSMCI0
endif

ifeq ($(CONFIG_CPU_HAS_QSPI_RICR_WICR), y)
CPPFLAGS += -DCONFIG_CPU_HAS_QSPI_RICR_WICR
endif

ifeq ($(CONFIG_AT91_MCI), y)
CPPFLAGS += -DCONFIG_AT91_MCI
endif

ifeq ($(CONFIG_AT91_MCI0), y)
CPPFLAGS += -DCONFIG_AT91_MCI0
endif

ifeq ($(CONFIG_MCI_SLOTA), y)
CPPFLAGS += -DCONFIG_MCI_SLOTA
endif

ifeq ($(CONFIG_MCI_SLOTB), y)
CPPFLAGS += -DCONFIG_MCI_SLOTB
endif

ifeq ($(CONFIG_AT91_MCI1), y)
CPPFLAGS += -DCONFIG_AT91_MCI1
endif

ifeq ($(CONFIG_AT91_MCI2), y)
CPPFLAGS += -DCONFIG_AT91_MCI2
endif

ifeq ($(CONFIG_SDHC), y)
CPPFLAGS += -DCONFIG_SDHC
endif

ifeq ($(CONFIG_SDHC0), y)
CPPFLAGS += -DCONFIG_SDHC0
endif

ifeq ($(CONFIG_SDHC1), y)
CPPFLAGS += -DCONFIG_SDHC1
endif

ifeq ($(CONFIG_SDHC2), y)
CPPFLAGS += -DCONFIG_SDHC2
endif

ifeq ($(CONFIG_SDHC_NODMA), y)
CPPFLAGS += -DCONFIG_SDHC_NODMA
endif

ifeq ($(CONFIG_SPI_BUS0), y)
CPPFLAGS += -DCONFIG_SPI_BUS0
endif

ifeq ($(CONFIG_SPI_BUS1), y)
CPPFLAGS += -DCONFIG_SPI_BUS1
endif

ifeq ($(CONFIG_SPI_BUS2), y)
CPPFLAGS += -DCONFIG_SPI_BUS2
endif

ifeq ($(CONFIG_SPI0_IOSET_1), y)
CPPFLAGS += -DCONFIG_SPI0_IOSET_1
endif

ifeq ($(CONFIG_SPI0_IOSET_2), y)
CPPFLAGS += -DCONFIG_SPI0_IOSET_2
endif

ifeq ($(CONFIG_SPI1_IOSET_1), y)
CPPFLAGS += -DCONFIG_SPI1_IOSET_1
endif

ifeq ($(CONFIG_SPI1_IOSET_2), y)
CPPFLAGS += -DCONFIG_SPI1_IOSET_2
endif

ifeq ($(CONFIG_SPI1_IOSET_3), y)
CPPFLAGS += -DCONFIG_SPI1_IOSET_3
endif

ifeq ($(CONFIG_SPI), y)
CPPFLAGS += -DCONFIG_SPI
endif

ifeq ($(CONFIG_QSPI_BUS0), y)
CPPFLAGS += -DCONFIG_QSPI_BUS0
endif

ifeq ($(CONFIG_QSPI_BUS1), y)
CPPFLAGS += -DCONFIG_QSPI_BUS1
endif

ifeq ($(CONFIG_QSPI0_IOSET_1), y)
CPPFLAGS += -DCONFIG_QSPI0_IOSET_1
endif

ifeq ($(CONFIG_QSPI0_IOSET_2), y)
CPPFLAGS += -DCONFIG_QSPI0_IOSET_2
endif

ifeq ($(CONFIG_QSPI0_IOSET_3), y)
CPPFLAGS += -DCONFIG_QSPI0_IOSET_3
endif

ifeq ($(CONFIG_QSPI1_IOSET_1), y)
CPPFLAGS += -DCONFIG_QSPI1_IOSET_1
endif

ifeq ($(CONFIG_QSPI1_IOSET_2), y)
CPPFLAGS += -DCONFIG_QSPI1_IOSET_2
endif

ifeq ($(CONFIG_QSPI1_IOSET_3), y)
CPPFLAGS += -DCONFIG_QSPI1_IOSET_3
endif

ifeq ($(CONFIG_QSPI_4B_OPCODES),y)
CPPFLAGS += -DCONFIG_QSPI_4B_OPCODES
endif

ifeq ($(CONFIG_QSPI_XIP), y)
CPPFLAGS += -DCONFIG_QSPI_XIP
endif

ifeq ($(CONFIG_QSPI), y)
CPPFLAGS += -DCONFIG_QSPI
endif

ifeq ($(CONFIG_FLEXCOM), y)
CPPFLAGS += -DCONFIG_FLEXCOM
endif

ifeq ($(CONFIG_USART), y)
CPPFLAGS += -DCONFIG_USART
endif

ifeq ($(CONFIG_RSTC), y)
CPPFLAGS += -DCONFIG_RSTC
endif

ifeq ($(CONFIG_SHDWC), y)
CPPFLAGS += -DCONFIG_SHDWC
endif

ifeq ($(CONFIG_TZC400), y)
CPPFLAGS += -DCONFIG_TZC400
endif

ifeq ($(CONFIG_TZC400_SIMPLE_PROFILE), y)
CPPFLAGS += -DCONFIG_TZC400_SIMPLE_PROFILE
endif

ifeq ($(CONFIG_TZC400_SPECULATIVE_LOAD), y)
CPPFLAGS += -DCONFIG_TZC400_SPECULATIVE_LOAD
endif

ifeq ($(CONFIG_WDTS), y)
CPPFLAGS += -DCONFIG_WDTS
endif
