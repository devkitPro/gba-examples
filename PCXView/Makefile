#---------------------------------------------------------------------------------
# Generic makefile for GBA projects
#
# Tab stops set to 4
#	|	|	|	|
#	0	1	2	3
#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# TARGET	- the name of the output, if this ends with _mb generates a multiboot image
# BUILD		- the directory where object files & intermediate files will be placed
# SOURCES	- a list of directories containing source code
# INCLUDES	- a list of directories containing extra header files
#				should include the build directory
#---------------------------------------------------------------------------------
TARGET		:=	PCXView_mb
BUILD		:=	build
SOURCES		:=	src data
INCLUDES	:=	$(BUILD)

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
THUMB	:=	-mthumb -mthumb-interwork
ARM		:=	-marm -mthumb-interwork

ARCH	:=	$(THUMB)

CFLAGS	:=	-save-temps -g -Wall -O3\
			-mcpu=arm7tdmi -mtune=arm7tdmi\
 			-fomit-frame-pointer\
			-ffast-math

CFLAGS	+=	$(INCLUDE)

ASFLAGS	:=	$(ARCH)

LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	/c/devkitARM_r8/bin:/bin
#---------------------------------------------------------------------------------
# absolute path required since this makefile uses the build directory
# as the working directory
#---------------------------------------------------------------------------------
LIBGBA	:=	/c/projects/gba/libgba

#---------------------------------------------------------------------------------
# the prefix on the compiler executables
#---------------------------------------------------------------------------------
PREFIX			:=	arm-elf-
#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=	-lgba

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBGBA)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export CC		:=	$(PREFIX)gcc
export CXX		:=	$(PREFIX)g++
export AR		:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy


#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PCXFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pcx)))

export OFILES	:=	$(PCXFILES:.pcx=.o) $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)

	export LD	:=	$(CC)

else

	export LD	:=	$(CXX)

endif

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) *.elf

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES)

#---------------------------------------------------------------------------------
%.gba: %.elf
	@echo built ... $(notdir $@)
	@$(OBJCOPY) -O binary $< $@
	@gbafix $@

#---------------------------------------------------------------------------------
%_mb.elf:
	@echo linking multiboot
	@$(LD) -specs=gba_mb.specs $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
%.elf:
	@echo linking cartridge
	@$(LD)  $(LDFLAGS) -specs=gba.specs $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
# Compile Targets for C/C++
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%_arm.o : %_arm.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD $(ARM) $(CFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%.o : %.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD $(THUMB) $(CFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%_arm.o : %_arm.c
	@echo $(notdir $<)
	@$(CC) -MMD $(ARM) $(CFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%.o : %.c
	@echo $(notdir $<)
	@$(CC) -MMD $(THUMB) $(CFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%.o : %.s
	@echo $(notdir $<)
	@$(CC) -MMD $(ASFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
# canned command sequence for binary data
#---------------------------------------------------------------------------------
define bin2o
	cp $(<) $(*).tmp
	alignbin $(*).tmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
	--rename-section .data=.rodata,readonly,data,contents,alloc \
	--redefine-sym _binary_$*_tmp_start=$*\
	--redefine-sym _binary_$*_tmp_end=$*_end\
	--redefine-sym _binary_$*_tmp_size=$*_size\
	$(*).tmp $(@)
	echo "extern const u8" $(*)"[];" > $(*).h
	echo "extern const u32" $(*)_len[]";" >> $(*).h
	echo "#define" $(*)_size "(u32)" $(*)_len >> $(*).h
	rm $(*).tmp
endef

#---------------------------------------------------------------------------------
%.o	:	%.pcx
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
