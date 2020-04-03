SRCDIR = src/
BINDIR = bin/
INCDIR = include/
TESTDIR = src/test/
TOOLDIR = src/tools/
TESTBINDIR = bin/test/
TOOLBINDIR = bin/tools/
DEPDIR = dep/

OBJ = obj/common.o \
      obj/mm_io.o

TESTS = $(TESTBINDIR)result \
		$(TESTBINDIR)structs \
		$(TESTBINDIR)block \
		$(TESTBINDIR)partition

TOOLS = $(TOOLBINDIR)permute  \
		$(TOOLBINDIR)sort \
		$(TOOLBINDIR)convert_blocked \
		$(TOOLBINDIR)convert \
		$(TOOLBINDIR)expanded \
		$(TOOLBINDIR)double2float \
		$(TOOLBINDIR)pprint \
		$(TOOLBINDIR)one2zero \
		$(TOOLBINDIR)ipermute \
		$(TOOLBINDIR)prepare

INCFLAGS = -I$(INCDIR) -I$(DEPDIR)
LINKFLAGS =

# CC should be a C compiler and CXX a C++ compiler
CC = clang
CXX = clang++
FLAGSOPT = -O3 -fno-rtti -fno-exceptions -funroll-loops 
FLAGS = $(INCFLAGS) -std=c++11 -g -fopenmp $(FLAGSOPT)
FLAGSDEBUG = $(INCFLAGS) -std=c++11 -g -fopenmp
FLAGSC = $(INCFLAGS) -fopenmp
HEADERS = $(INCDIR)*.h $(INCDIR)matrix/*.h $(INCDIR)io/*.h $(INCDIR)test/*.h

# An ugly solution to generate output directories because git cannot keep track
# of them
IGNORE := $(shell mkdir -p obj obj lib bin bin/test bin/tools)

all: test tools spmv

spmv: csbr cgbr csr cswr

test: $(TESTS)

tools: $(TOOLS)

clean:
	rm -rf obj lib bin

###############################
# Test targets
###############################

$(TESTBINDIR)block: $(TESTDIR)block.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) -DITERATIONS=200 \
		$< lib/common.a -o $@

$(TESTBINDIR)structs: $(TESTDIR)structs.cpp $(INCDIR)typedefs.h
	$(CXX) $(FLAGSDEBUG) $< -o $@

$(TESTBINDIR)result: $(TESTDIR)result.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) $< lib/common.a -o $@

$(TESTBINDIR)partition: src/drivers/blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Cgbr<float,uint32_t,uint16_t>" -DPARTITION_DEBUG \
		-DVALTYPE=float -DITYPE=uint32_t -DSITYPE=uint16_t \
		$< lib/common.a -o $@

###############################
# Tools
###############################

$(TOOLBINDIR)sort: $(TOOLDIR)sort.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) $(TOOLDIR)sort.cpp lib/common.a -o $@

$(TOOLBINDIR)permute: $(TOOLDIR)permute.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) $(TOOLDIR)permute.cpp lib/common.a -o $@

$(TOOLBINDIR)ipermute: $(TOOLDIR)permute.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) $(TOOLDIR)permute.cpp lib/common.a -o $@ -DINVPERM

$(TOOLBINDIR)expanded: $(TOOLDIR)expanded.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) $(TOOLDIR)expanded.cpp lib/common.a -o $@

$(TOOLBINDIR)convert_blocked: $(TOOLDIR)convert_blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) $(TOOLDIR)convert_blocked.cpp lib/common.a -o $@

$(TOOLBINDIR)convert: $(TOOLDIR)convert.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) $(TOOLDIR)convert.cpp lib/common.a -o $@

$(TOOLBINDIR)prepare: $(TOOLDIR)prepare.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGSDEBUG) -O2 $(TOOLDIR)prepare.cpp lib/common.a -o $@

$(TOOLBINDIR)double2float: $(TOOLDIR)double2float.c
	$(CC) $(FLAGSC) $< -o $@

$(TOOLBINDIR)one2zero: $(TOOLDIR)one2zero.c
	$(CC) $(FLAGSC) $< -o $@

$(TOOLBINDIR)pprint: $(TOOLDIR)pprint.c
	$(CC) $(FLAGSC) $< -o $@

###############################
# Driver programs targets
###############################

csbr: $(BINDIR)csbr $(BINDIR)csbr2

$(BINDIR)csbr: src/drivers/blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Csbr<float,uint32_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

$(BINDIR)csbr2: src/drivers/blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Csbr2<float,uint32_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

cgbr: $(BINDIR)cgbr $(BINDIR)cgbr2

$(BINDIR)cgbr: src/drivers/blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Cgbr<float,uint32_t,uint16_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

$(BINDIR)cgbr2: src/drivers/blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Cgbr2<float,uint32_t,uint16_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

bcsr: $(BINDIR)bcsr

$(BINDIR)bcsr: src/drivers/blocked.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Bcsr<float,uint32_t>" \
		$< lib/common.a -o $@

csr: $(BINDIR)csr $(BINDIR)csr2

$(BINDIR)csr: src/drivers/csr.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Csr<float,uint32_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

$(BINDIR)csr2: src/drivers/csr.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Csr2<float,uint32_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

cswr: $(BINDIR)cswr

$(BINDIR)cswr: src/drivers/cswr.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DMATRIXTYPE="Cswr<float,uint32_t,uint8_t>" -DVALTYPE=float -DITYPE=uint32_t \
		$< lib/common.a -o $@

bench: $(BINDIR)bench $(BINDIR)pbench

$(BINDIR)bench: src/drivers/bench.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		-DVALTYPE=float -DINDEXTYPE=uint32_t -DSINDEXTYPE=uint16_t \
		$< lib/common.a -o $@

$(BINDIR)pbench: src/drivers/pbench.cpp lib/common.a $(HEADERS)
	$(CXX) $(FLAGS) \
		 -DVALTYPE=float -DINDEXTYPE=uint32_t -DSINDEXTYPE=uint16_t \
		$< lib/common.a -o $@

###############################
# Object files and libraries
###############################

obj/common.o: src/common.cpp
	$(CXX) $(FLAGS) -c $< -o $@

obj/mm_io.o: $(DEPDIR)mm_io/mm_io.c
	$(CC) $(FLAGSC) -c $< -o $@

lib/common.a: $(OBJ)
	ar rcs $@ $^

