#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:\\raid\jwilliams\microblaze\projects\mbvanilla\mbvanilla.xmp
#################################################################

# Name of the Microprocessor system
# The hardware specification of the system is in file :
# \\raid\jwilliams\microblaze\projects\mbvanilla\mbvanilla.mhs
# The software specification of the system is in file :
# mbvanilla.mss

SYSTEM = mbvanilla

MHSFILE = mbvanilla.mhs

MSSFILE = mbvanilla.mss

MVSFILE = mbvanilla.mvs

FPGA_ARCH = virtex2

DEVICE = xc2v1000fg456-4

LANGUAGE = vhdl

PLATGEN_OPTIONS = -a $(FPGA_ARCH)  -l $(LANGUAGE) 

LIBGEN_OPTIONS = -a $(FPGA_ARCH) \
                   $(MBLAZE_LIBG_OPT)

SIM_CMD = vsim

SIMGEN_OPTIONS = -a $(FPGA_ARCH) -l $(LANGUAGE)

MBLAZE_OUTPUT = mblaze/code/executable.elf

LIBRARIES =  \
       mblaze/lib/libxil.a 

# External Targets

all:
	@echo "Makefile to build a Microprocessor system :"
	@echo "Run make with any of the following targets"
	@echo "  make libs     : Configures the sw libraries for this system"
	@echo "  make program  : Compiles the program sources for all the processor instances"
	@echo "  make netlist  : Generates the netlist for this system ($(SYSTEM))"
	@echo "  make bits     : Runs Implementation tools to generate the bitstream"
	@echo "  make init_bram: Initializes bitstream with BRAM data"
	@echo "  make download : Downloads the bitstream onto the board"
	@echo "  make sim      : Generates simulation models and runs simulator"
	@echo "  make netlistclean: Deletes netlist"
	@echo "  make hwclean  : Deletes implementation dir"
	@echo "  make libsclean: Deletes sw libraries"
	@echo "  make programclean: Deletes compiled ELF files"
	@echo "  make simclean : Deletes simulation dir"
	@echo "  make clean    : Deletes all generated files/directories"
	@echo " "
	@echo "  make <target> : (Default)"
	@echo "      Creates a Microprocessor system using default initializations"
	@echo "      specified for each processor in MSS file"


bits: implementation/$(SYSTEM).bit

netlist: implementation/$(SYSTEM).ngc

libs: $(LIBRARIES)

program: $(MBLAZE_OUTPUT) 

bsp: 
	@echo "To generate BSP, please specify VxWorks5_4 as OS for atleast one processor instance"

download: implementation/download.bit dummy
	@echo "*********************************************"
	@echo "Downloading Bitstream onto the target board"
	@echo "*********************************************"
	impact -batch etc/download.cmd

init_bram: implementation/download.bit

sim: simulation/$(SYSTEM)_init.vhd
	cd simulation; $(SIM_CMD) -do $(SYSTEM).do &

simmodel: simulation/$(SYSTEM)_init.vhd

clean: hwclean libsclean programclean simclean
	rm -f bram_init.sh
	rm -f _impact.cmd

hwclean: netlistclean
	rm -rf implementation synthesis xst hdl
	rm -rf xst.srp $(SYSTEM).srp

netlistclean:
	rm -f implementation/$(SYSTEM).bit implementation/$(SYSTEM).ncd implementation/$(SYSTEM)_bd.bmm 

simclean: 
	rm -rf simulation

libsclean: MBLAZE_LIBSCLEAN 

programclean: MBLAZE_PROGRAMCLEAN 

#################################################################
# TARGETS/MACROS FOR PROCESSOR MBLAZE
#################################################################

MBLAZE_SOURCES = code/mbvanilla.c code/xflash.c code/xflash_cfi.c code/xflash_g.c code/xflash_geometry.c code/xflash_intel.c code/xflash_intel_l.c 

MBLAZE_CC = mb-gcc
MBLAZE_CC_SIZE = mb-size
MBLAZE_CC_OPT = -O1
MBLAZE_CFLAGS = 
MBLAZE_CC_SEARCH = # -B
MBLAZE_LIBPATH = -L./mblaze/lib/ # -L
MBLAZE_INCLUDES = -I./mblaze/include/ # -I
MBLAZE_LFLAGS = # -l

MBLAZE_CC_FLOW =  # -[E|S|c]
MBLAZE_CC_PREPROC_FLAG = # -Wp,
MBLAZE_CC_ASM_FLAG = # -Wa,
MBLAZE_CC_LINKER_FLAG = # -Wl,
MBLAZE_LINKER_SCRIPT = #-Wl,-T -Wl,
MBLAZE_CC_DEBUG_FLAG =  -g 
MBLAZE_MODE = xmdstub
MBLAZE_LIBG_OPT = -$(MBLAZE_MODE) mblaze
MBLAZE_CC_SOFTMUL_FLAG= # -mno-xl-soft-mul 
MBLAZE_CC_GLOBPTR_FLAG= # -mxl-gp-opt
MBLAZE_CC_START_ADDR_FLAG= -Wl,-defsym -Wl,_TEXT_START_ADDR=0xFFE00000
MBLAZE_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
MBLAZE_OTHER_CC_FLAGS= $(MBLAZE_CC_FLOW) $(MBLAZE_CC_GLOBPTR_FLAG)  \
                  $(MBLAZE_CC_START_ADDR_FLAG) $(MBLAZE_CC_STACK_SIZE_FLAG)  \
                  $(MBLAZE_CC_SOFTMUL_FLAG) -mxl-barrel-shift \
                  $(MBLAZE_CC_PREPROC_FLAG) $(MBLAZE_CC_ASM_FLAG) $(MBLAZE_CC_LINKER_FLAG)  \
                  $(MBLAZE_LINKER_SCRIPT) $(MBLAZE_CC_DEBUG_FLAG)  \

$(MBLAZE_OUTPUT) : $(MBLAZE_SOURCES) $(LIBRARIES) mblaze/lib/compiler.opt
	$(MBLAZE_CC) $(MBLAZE_CFLAGS) $(MBLAZE_CC_OPT) $(MBLAZE_SOURCES) -o $(MBLAZE_OUTPUT)  \
	$(MBLAZE_OTHER_CC_FLAGS) $(MBLAZE_INCLUDES) $(MBLAZE_LIBPATH)  \
	-xl-mode-$(MBLAZE_MODE)  \
	$(MBLAZE_LFLAGS) 
	$(MBLAZE_CC_SIZE) $(MBLAZE_OUTPUT) 

MBLAZE_LIBSCLEAN:
	rm -rf mblaze/lib/

MBLAZE_PROGRAMCLEAN:
	rm -f $(MBLAZE_OUTPUT) 

#################################################################
# TARGETS/MACROS FOR XILINX IMPLEMENTATION FLOW
#################################################################


implementation/download.bit: implementation/$(SYSTEM).bit  $(MBLAZE_OUTPUT) 
	@cp -f implementation/$(SYSTEM)_bd.bmm .
	@echo "*********************************************"
	@echo "Initializing BRAM contents of the bitstream"
	@echo "*********************************************"
	./bram_init.sh 
	@rm -f $(SYSTEM)_bd.bmm

implementation/$(SYSTEM).bit: implementation/$(SYSTEM).ngc etc/fast_runtime.opt etc/bitgen.ut xpsxflow.opt
	@echo "Copying Xilinx Implementation tool scripts.."
	@cp -f etc/bitgen.ut implementation/
	@cp -f etc/fast_runtime.opt implementation/
	@cp -f data/$(SYSTEM).ucf implementation/$(SYSTEM).ucf
	@echo "*********************************************"
	@echo "Running Xilinx Implementation tools.."
	@echo "*********************************************"
	xflow -wd implementation -p $(DEVICE) -implement fast_runtime.opt $(SYSTEM).ngc
	cd implementation; bitgen -w -f bitgen.ut $(SYSTEM)

implementation/$(SYSTEM).ngc: $(MHSFILE) platgen.opt
	@echo "*********************************************"
	@echo "Creating system netlist for hardware specification.."
	@echo "*********************************************"
	platgen $(PLATGEN_OPTIONS) -s 2 $(MHSFILE)
	echo "Running iSE XST..."
	xst -ifn synthesis/$(SYSTEM).scr

$(LIBRARIES): $(MHSFILE) $(MSSFILE) libgen.opt
	@echo "*********************************************"
	@echo "Creating software libraries..."
	@echo "*********************************************"
	libgen $(LIBGEN_OPTIONS) $(MSSFILE)

simulation/$(SYSTEM)_init.vhd: simulation/$(SYSTEM)_sim.bmm  $(MBLAZE_OUTPUT) 
	simgen $(SIMGEN_OPTIONS) -i $(MVSFILE)

simulation/$(SYSTEM)_sim.bmm: hdl/$(SYSTEM).vhd $(MVSFILE) simgen.opt
	@echo "*********************************************"
	@echo "Creating simulation models..."
	@echo "*********************************************"
	platgen -bmm $(PLATGEN_OPTIONS) -s 0 $(SYSTEM).mhs 
	simgen $(SIMGEN_OPTIONS)  $(MVSFILE)

hdl/$(SYSTEM).vhd: $(MHSFILE)
	@echo "*********************************************"
	@echo "Creating HDL files..."
	@echo "*********************************************"
	platgen -s 0 $(PLATGEN_OPTIONS) $(MHSFILE)

dummy:
	@echo ""

