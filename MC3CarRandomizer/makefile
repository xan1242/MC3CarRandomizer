PS2SDK = ../external/ps2sdk/ps2sdk

ENABLELOG ?= 0
DEBUG ?= 0
SUPERDEBUG ?= 0
RELEASE ?= 0

EE_BIN = ../build/MC3CarRandomizer.elf
EE_OBJS = main.o \
includes\minjector.c \
includes\stringhash.c \
includes\bRandom.c \
gprintf.c \
mcRandomizer.c \


NEWLIB_NANO = 1
KERNEL_NOPATCH = 1

DEFINES = -DMINJECTOR_MINIMAL -DMINJECTOR_NO_ADJUST

BASE_ADDRESS = 0x02100000
EE_LINKFILE = linkfile

EE_CFLAGS += -fno-zero-initialized-in-bss $(DEFINES)
EE_LDFLAGS += -Wl,--entry=init -Wl,-Map,../build/MC3CarRandomizer.map -Wl,-exclude-libs,ALL -Wl,'--defsym=BASE_ADDRESS=$(BASE_ADDRESS)' $(DEFINES)
EE_INCS += -Iincludes

ifeq ($(ENABLELOG),1)
  EE_CFLAGS += -D__ENABLELOG
endif

ifeq ($(DEBUG),1)
  EE_CFLAGS += -D__DEBUG
endif

ifeq ($(SUPERDEBUG),1)
  EE_CFLAGS += -D__DEBUG -D__SUPERDEBUG -g
endif

ifeq ($(RELEASE),1)
  EE_CFLAGS += -D__RELEASE
endif

ifeq ($(SUPERDEBUG),0)
  EE_CFLAGS += -fdata-sections -ffunction-sections
  # If needed, add more PCSX2 symbols in to the keep_symbols list and undefined list...
  EE_LDFLAGS += -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,--retain-symbols-file=keep_symbols.txt -Wl,--undefined=CompatibleCRCList
endif

all: $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

# massive PS2SDK pepeg moment - they forgot to add these to the single compile target ($(EE_BIN)) -- this is a quick hack to work around the issue
EE_LDFLAGS += $(EE_INCS) $(EE_CFLAGS)
