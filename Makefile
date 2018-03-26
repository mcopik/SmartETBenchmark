BOOST_UBLAS_INCLUDE=/usr/include/
BOOST_UBLAS_CXXFLAGS=-I$(BOOST_UBLAS_INCLUDE) -D NDEBUG -DBOOST_UBLAS_NDEBUG

BLAS_INCLUDE=/opt/Intel/mkl/include
BLAS_LINK=/opt/Intel/mkl/lib/intel64/
BLAS_CXXFLAGS=-isystem $(BLAS_INCLUDE)
BLAS_LDFLAGS=-L$(BLAS_LINK) -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread

BLAZE_INCLUDE=/home/mcopik/Projekty/RWTH/Seminar/blaze-2.3
BLAZE_CXXFLAGS=-I$(BLAZE_INCLUDE)
BLAZE_LINK=/home/mcopik/Projekty/RWTH/Seminar/blaze-2.3/lib
BLAZE_LDFLAGS=-L$(BLAZE_LINK) -lblaze

BLITZ_INCLUDE=/home/mcopik/Projekty/RWTH/Seminar/blitz_install/include
BLITZ_INCLUDE_ICC=/home/mcopik/Projekty/RWTH/Seminar/blitz_icc/include
BLITZ_CXXFLAGS=-I$(BLITZ_INCLUDE)
BLITZ_CXXFLAGS_ICC=-I$(BLITZ_INCLUDE_ICC)

BLITZ_LINK=/home/mcopik/Projekty/RWTH/Seminar/blitz_install/lib
BLITZ_LINK_ICC=/home/mcopik/Projekty/RWTH/Seminar/blitz_icc/lib
BLITZ_LDFLAGS=-L$(BLITZ_LINK) -lblitz
BLITZ_LDFLAGS_ICC=-L$(BLITZ_LINK_ICC) -lblitz

CXXFLAGS=-O3 -mtune=core-avx2 -inline-forceinline  -Wall -std=c++11 $(BOOST_UBLAS_CXXFLAGS) $(BLAZE_CXXFLAGS) $(BLAS_CXXFLAGS) $(BLITZ_CXXFLAGS_ICC)
CXX=icc
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS= $(SRCS:$(SRCDIR)/%.cpp=$(TARGET_DIR)/%.o)
#LDFLAGS=$(BOOST_UBLAS_LDFLAGS) $(BLAZE_LDFLAGS) $(BLAS_LDFLAGS) $(BLITZ_LDFLAGS)
LDFLAGS=$(BOOST_UBLAS_LDFLAGS) $(BLAZE_LDFLAGS) $(BLAS_LDFLAGS) $(BLITZ_LDFLAGS_ICC) -fopenmp
TARGET_DIR=bin
TARGET=$(TARGET_DIR)/benchmark

$(TARGET): $(TARGET_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

all:    $(TARGET)

debug:	CXXFLAGS += -g
debug:	$(TARGET)

$(OBJS): $(TARGET_DIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $<  -o $@
.c.o:
	$(CXX) $(CXXFLAGS) -c $<  -o $@

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

clean:
	rm -f $(OBJS) $(TARGET)

