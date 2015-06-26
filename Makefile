BOOST_UBLAS_INCLUDE=/usr/include/
#BOOST_UBLAS_LINK=/usr/lib/x86_64-linux-gnu/
BOOST_UBLAS_CXXFLAGS=-I$(BOOST_UBLAS_INCLUDE)
#BOOST_UBLAS_LDFLAGS="-L$(BOOST_UBLAS_LINK) -lboost_ublas

CXXFLAGS=-O3 -Wall -std=c++11 $(BOOST_UBLAS_CXXFLAGS)
CXX=g++
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS= $(SRCS:$(SRCDIR)/%.cpp=$(TARGET_DIR)/%.o)
LDFLAGS="$(BOOST_UBLAS_LDFLAGS)"
TARGET_DIR=bin
TARGET=$(TARGET_DIR)/benchmark

$(TARGET): $(TARGET_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

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

