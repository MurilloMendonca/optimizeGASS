CXX=g++
CXXFLAGS=-O3 -pthread
SRC_DIR=newGASS-base
SRCS=$(SRC_DIR)/Individuo.cpp $(SRC_DIR)/GA.cpp $(SRC_DIR)/newGASS.cpp Individuo2.cpp GA.cpp optimizeGASS.cpp
OBJS=$(SRCS:.cpp=.o)
TARGET=optimizeGASS

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)