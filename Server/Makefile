# Makefile
.SUFFIXES:
.SECONDARY:
.PHONY: all clean send
#-------------------------------------------------------------------------------
# ToolChains
#-------------------------------------------------------------------------------
CCompiler := powerpc-eabi-gcc
CppCompiler := powerpc-eabi-g++
Linker := powerpc-eabi-g++
NameList := powerpc-eabi-nm

#-------------------------------------------------------------------------------
# Environment
#-------------------------------------------------------------------------------
DevKitPro := $(DEVKITPRO)
PortLibs := $(DevKitPro)/portlibs/ppc
Wups := $(DevKitPro)/wups
Wut := $(DevKitPro)/wut
Wums := $(DevKitPro)/wums
User := $(DevKitPro)/User

MachineDependent := -DESPRESSO -mcpu=750 -meabi -mhard-float -D__WIIU__
#-------------------------------------------------------------------------------
# Directories
#-------------------------------------------------------------------------------
TopDir := $(CURDIR)

Target := Example

SourceDir := $(TopDir)/Source
IncludeDir := $(TopDir)/Include
BuildDir := $(TopDir)/Build
DistDir := $(TopDir)/Dist

#-------------------------------------------------------------------------------
# Macros
#-------------------------------------------------------------------------------
include $(TopDir)/Tools.mk

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
BuildObjectDir := $(BuildDir)/Object
BuildDependenceDir := $(BuildDir)/Dependence

CppFile := $(shell find $(SourceDir) -type f -name '*.cpp')
CppRelative := $(shell realpath --relative-to=$(SourceDir) $(CppFile))
BuildObjectCppFile := $(patsubst %.cpp,$(BuildObjectDir)/Cpp/%.o,$(CppRelative))

BuildElfFile := $(BuildDir)/$(Target).elf
DistWpsFile := $(DistDir)/$(Target).wps

SendPluginScript := $(TopDir)/SendPlugin.sh
Logger := udplogserver

#-------------------------------------------------------------------------------
# Libraries
#-------------------------------------------------------------------------------
LibraryEntries := Logger Debug IO Network wups wut notifications mappedmemory kernel
LibraryDirs := $(PortLibs)/lib $(Wups)/lib $(Wut)/lib $(Wums)/lib $(User)/Lib
LibraryIncludeDirs := $(PortLibs)/include $(Wups)/include $(Wut)/include $(Wums)/include $(User)/Include
LibraryDirFlags := $(foreach dir,$(LibraryDirs),-L$(dir))
LibraryFlags := $(foreach entry,$(LibraryEntries),-l$(entry))

#-------------------------------------------------------------------------------
# Includes
#-------------------------------------------------------------------------------
IncludeDirs := $(IncludeDir) $(LibraryIncludeDirs)
IncludeFlags := $(foreach dir,$(IncludeDirs),-I$(dir))

#-------------------------------------------------------------------------------
# Cpp Flags
#-------------------------------------------------------------------------------
CppFlags := $(MachineDependent) $(IncludeFlags) $(LibraryFlags) -Wall -O3 -ffunction-sections -std=c++23

#-------------------------------------------------------------------------------
# Linker Flags
#-------------------------------------------------------------------------------
LinkerScript := -T$(Wums)/share/libmappedmemory.ld -T$(Wums)/share/libkernel.ld -T$(Wups)/share/wups.ld
Specs := -specs=$(Wut)/share/wut.specs -specs=$(Wups)/share/wups.specs
LinkerFlags := $(LinkerScript) $(Specs) -g

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------
all: $(DistWpsFile)

$(BuildObjectDir)/Cpp/%.o: $(SourceDir)/%.cpp
	@echo $(notdir $<)
	$(call cpp2o,$<,$@,$(BuildDependenceDir)/$*.d,$(CppFlags))

$(BuildDir)/%.elf: $(BuildObjectCppFile)
	@echo linking ... $(notdir $@)
	@$(call o2elf,$^,$@,$(LinkerFlags),$(LibraryDirFlags),$(LibraryFlags),$(BuildDir)/$*.map)
	@$(call elf2lst,$@,$(BuildDir)/$*.lst)

$(DistDir)/%.wps: $(BuildDir)/%.elf
	@echo building ... $(notdir $@)
	@$(call elf2wps,$<,$@)

-include $(BuildDependenceDir)/*.d

clean:
	@echo clean ...
	@rm -rf $(BuildDir) $(DistDir)

run: $(DistWpsFile)
	@echo sending ... $(notdir $<)
	@$(SendPluginScript) $<
	@$(Logger)