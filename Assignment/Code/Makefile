#####################################################################################
# Specify:
# (1) The folder containing the source files + name of the compiled program
# (2) Name of the program
#####################################################################################
SRC := src
BIN := program

OBJ := obj
MKDIR := mkdir -p
RM := rm -rf
SRCs := $(shell find $(SRC) -name "*.cpp")
OBJs := $(subst $(SRC), $(OBJ), $(SRCs:.cpp=.o))

CXX := g++ -std=c++17
CPPFLAGS := -ICode/include -ICode/include/ann -ICode/include/tensor -ICode/include/sformat -ICode/demo -ICode/src
CFLAGS := -pthread #-Wall
LDLIBS := -lm -lpthread 
#############################################################################################
# Note: 
# (1) Use -Iinclude/tensor: because put xtensor and its headers inside of folder tensor
# (2) Use -Iinclude/sformat: because put sformat and its headers inside of folder sformat
# (3) Use -Iinclude/ann: because put header files of ann inside of folder ann
# (4) Use -Idemo: because put header files of demos inside of this folder
#############################################################################################

all: $(BIN)

$(BIN): $(OBJs)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(OBJs) -o $@ $(LDLIBS)

$(OBJs): $(SRCs)
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $(subst $(OBJ), $(SRC), $(@:.o=.cpp)) -o $@
# Here: repeat here for other other source codes

# Clean rule to remove generated files
clean:
	$(RM) $(BIN)
	$(RM) $(OBJ)
