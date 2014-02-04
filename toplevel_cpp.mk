CPPFLAGS += \
	-DIMG_ADDRESS=$(IMG_ADDRESS)	\
	-DIMG_SIZE=$(IMG_SIZE)		\
	-DOF_OFFSET=$(OF_OFFSET)	\
	-DOF_ADDRESS=$(OF_ADDRESS)	\
	-DMEM_BANK=$(MEM_BANK)	\
	-DMEM_SIZE=$(MEM_SIZE)	\
	-DIMAGE_NAME="\"$(IMAGE_NAME)\""	\
	-DLINUX_KERNEL_ARG_STRING="\"$(LINUX_KERNEL_ARG_STRING)\"" \
	-DTOP_OF_MEMORY=$(TOP_OF_MEMORY)	\
	-DMACH_TYPE=$(MACH_TYPE)		\

ASFLAGS += -DTOP_OF_MEMORY=$(TOP_OF_MEMORY)	\
	-DMACH_TYPE=$(MACH_TYPE)		\

ifeq ($(CONFIG_UPLOAD_3RD_STAGE),y)
CPPFLAGS += -DCONFIG_UPLOAD_3RD_STAGE
#Below : just a place holder. Not actually ised.
CPPFLAGS += -DJUMP_ADDR=0x00000000
ASFLAGS += -DJUMP_ADDR=0x00000000
else
CPPFLAGS += -DJUMP_ADDR=$(JUMP_ADDR)
ASFLAGS += -DJUMP_ADDR=$(JUMP_ADDR)
endif

ifeq ($(CONFIG_LONG_TEST),y)
CPPFLAGS += -DCONFIG_LONG_TEST
endif

ifeq ($(CONFIG_DEBUG),y)
CPPFLAGS += -DCONFIG_DEBUG
endif

ifeq ($(CONFIG_NO_CONSOLE_OUTPUT),y)
CPPFLAGS += -DCONFIG_NO_CONSOLE_OUTPUT
endif

ifeq ($(CONFIG_HW_INIT),y)
CPPFLAGS += -DCONFIG_HW_INIT
endif

ifeq ($(CONFIG_USER_HW_INIT),y)
CPPFLAGS += -DCONFIG_USER_HW_INIT
endif

