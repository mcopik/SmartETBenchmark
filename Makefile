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

BLITZ_INCLUDE=/home/mcopik/Projekty/RWTH/Seminar/blitz-0.10
BLITZ_CXXFLAGS=-I$(BLITZ_INCLUDE)
BLITZ_LINK=/home/mcopik/Projekty/RWTH/Seminar/blitz-0.10/lib
BLITZ_LDFLAGS=-L$(BLITZ_LINK) -lblitz

CXXFLAGS=-O3 -Wno-unused-local-typedefs -Wall -std=c++11 $(BOOST_UBLAS_CXXFLAGS) $(BLAZE_CXXFLAGS) $(BLAS_CXXFLAGS) $(BLITZ_CXXFLAGS)
CXX=g++
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS= $(SRCS:$(SRCDIR)/%.cpp=$(TARGET_DIR)/%.o)
LDFLAGS=$(BOOST_UBLAS_LDFLAGS) $(BLAZE_LDFLAGS) $(BLAS_LDFLAGS) $(BLITZ_LDFLAGS)
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

