#####################################################################################
# Specify:
# (1) The folder containing the source files + name of the compiled program
# (2) Name of the program
#####################################################################################
SRC := Code/src
BIN := unit_test_program
BIN_LOADDATA := test_load_data
ANN_DIR := $(SRC)/ann
LAYERS_DIR := $(ANN_DIR)/layer

# Source files
LAYER_SRC := $(wildcard $(LAYERS_DIR)/*.cpp)
xt_lib := $(SRC)/tensor/xtensor_lib.cpp
FUNCTION_LIB := $(ANN_DIR)/functions.cpp

# Define the build directory
OBJ_DIR := Self_Cache_Build
MKDIR := mkdir -p
RM := rm -rf

# Object files
LAYER_OBJ := $(patsubst $(LAYERS_DIR)/%.cpp, $(OBJ_DIR)/$(LAYERS_DIR)/%.o, $(LAYER_SRC))
XT_LIB_OBJ := $(OBJ_DIR)/$(SRC)/tensor/xtensor_lib.o
FUNCTION_OBJ := $(OBJ_DIR)/$(ANN_DIR)/functions.o

# Compiler and flags
CXX := g++ -std=c++17
CPPFLAGS := -ICode/include -ICode/include/ann -ICode/include/tensor -ICode/include/sformat -ICode/demo -ICode/src -ISelf_test -g
CFLAGS := -pthread #-Wall
LDLIBS := -lm -lpthread

# Unit test source
TEST_DIR := Self_test
LMS_DIR := Self_LMS_sample_test
UNIT_TEST_SOURCE := $(file)
UNIT_TEST_OBJ := $(OBJ_DIR)/$(TEST_DIR)/$(basename $(notdir $(UNIT_TEST_SOURCE))).o

#############################################################################################
# Notes: 
# (1) Use -Iinclude/tensor: xtensor and headers are inside the tensor folder
# (2) Use -Iinclude/sformat: sformat and headers are inside the sformat folder
# (3) Use -Iinclude/ann: header files of ann are inside the ann folder
# (4) Use -Idemo: header files of demos are inside this folder
#############################################################################################

# Rule to build the main binary
$(BIN): $(OBJ_DIR) $(LAYER_OBJ) $(XT_LIB_OBJ) $(FUNCTION_OBJ) $(UNIT_TEST_OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LAYER_OBJ) $(XT_LIB_OBJ) $(FUNCTION_OBJ) $(UNIT_TEST_OBJ) -o $@ $(LDLIBS)

# Rule for building the object files from source
$(OBJ_DIR)/%.o: $(SRC)/%.cpp | $(OBJ_DIR)
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule for building layer object files from source
$(OBJ_DIR)/$(LAYERS_DIR)/%.o: $(LAYERS_DIR)/%.cpp | $(OBJ_DIR)/$(LAYERS_DIR)
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule to compile xtensor_lib.cpp
$(OBJ_DIR)/$(SRC)/tensor/xtensor_lib.o: $(SRC)/tensor/xtensor_lib.cpp | $(OBJ_DIR)/$(SRC)/tensor
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule to compile functions.cpp
$(OBJ_DIR)/$(SRC)/ann/functions.o: $(SRC)/ann/functions.cpp | $(OBJ_DIR)/$(SRC)/ann
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule for compiling the unit test object
$(UNIT_TEST_OBJ): $(UNIT_TEST_SOURCE) | $(OBJ_DIR)
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

# Create ann object directory if it doesn't exist
$(OBJ_DIR)/$(SRC)/ann:
	$(MKDIR) -p $(OBJ_DIR)/$(SRC)/ann

# Create layer object directory if it doesn't exist
$(OBJ_DIR)/$(LAYERS_DIR):
	$(MKDIR) $(OBJ_DIR)/$(LAYERS_DIR)

# Create tensor object directory if it doesn't exist
$(OBJ_DIR)/$(SRC)/tensor:
	$(MKDIR) $(OBJ_DIR)/$(SRC)/tensor

# Building unit tests
build_unit_test: $(OBJ_DIR) $(UNIT_TEST_OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(UNIT_TEST_OBJ) -o $(TEST_DIR)/$(BIN) $(LDLIBS)

# Building load data test
build_loaddata_test: $(OBJ_DIR) $(XT_LIB_OBJ) $(UNIT_TEST_OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(UNIT_TEST_OBJ) $(XT_LIB_OBJ) -o $(TEST_DIR)/$(BIN_LOADDATA) $(LDLIBS)

# Building ANN test layer
build_ann_test_layer: $(OBJ_DIR)/$(LAYERS_DIR) $(LAYER_OBJ) $(XT_LIB_OBJ) $(FUNCTION_OBJ) $(UNIT_TEST_OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(UNIT_TEST_OBJ) $(XT_LIB_OBJ) $(LAYER_OBJ) $(FUNCTION_OBJ) -o $(TEST_DIR)/$(BIN) $(LDLIBS)

# Run the demos
run_demo: $(BIN)
	./$(BIN) demo

# Run LMS tests
run_lms_test: $(BIN)
	./$(BIN) lms

# Run all tests
run_all_test: $(BIN)
	./$(BIN) test

# Check Environment Setup
check_env_setup:
	@echo "Log for Checking environment setup"
	@echo "CXX: $(CXX)"
	@echo "CPPFLAGS: $(CPPFLAGS)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "BIN: $(BIN)"
	@echo "UNIT_TEST_OBJ: $(UNIT_TEST_OBJ)"

# Clean rule to remove generated files
clean:
	$(RM) $(BIN)
#	$(RM) $(OBJ_DIR)
