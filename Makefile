RM = rm -f
CXX = g++
CXXFLAGS = -g
LDLIBS = -lpthread -lX11 -lHalf -lIlmImf
SRCS = main.cpp HDR.cpp Tonemapper.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
EXE = $(BIN_DIR)/hdr

all: run

run: $(EXE)
	$(EXE)

build: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(LDLIBS)

$(OBJ_DIR)/main.o: $(OBJ_DIR)/HDR.o $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/main.o -c $(SRC_DIR)/main.cpp $(LDLIBS)

$(OBJ_DIR)/HDR.o: $(SRC_DIR)/HDR.cpp
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/HDR.o -c $(SRC_DIR)/HDR.cpp $(LDLIBS)

$(OBJ_DIR)/Tonemapper.o: $(SRC_DIR)/Tonemapper.cpp
	$(CXX) $(CXXFLAGS) -o $(OBJ_DIR)/Tonemapper.o -c $(SRC_DIR)/Tonemapper.cpp $(LDLIBS)

clean:
	$(RM) $(EXE) $(OBJS)
