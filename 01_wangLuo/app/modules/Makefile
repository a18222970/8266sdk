################新建目录##############
# GEN_LIBS = libmodules.a 就是指定该文件夹要生成的库文件的名字为libmodules.a 
ifndef PDIR
GEN_LIBS = libmodules.a
endif


INCLUDES := $(INCLUDES) -I $(PDIR)include
INCLUDES += -I ./
PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

