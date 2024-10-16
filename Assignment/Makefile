#####################################################################################
# Specify:
# (1) The folder containing the source files + name of the compiled program
# (2) Name of the program
#####################################################################################
SRC := Code/src
BIN := unit_test_program
BIN_LOADDATA := test_load_data
xt_lib := $(SRC)/tensor/xtensor_lib.cpp
XT_LIB_OBJ := $(xt_lib:.cpp=.o)

# Define the build directory
OBJ_DIR := Self_Cache_Build
MKDIR := mkdir -p
RM := rm -rf
SRCs := $(shell find $(SRC) -name "*.cpp")
OBJs := $(patsubst $(SRC)/%.cpp, $(OBJ_DIR)/%.o, $(SRCs))

# Compiler and flags
CXX := g++ -std=c++17
CPPFLAGS := -ICode/include -ICode/include/ann -ICode/include/tensor -ICode/include/sformat -ICode/demo -ICode/src -ISelf_test
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
$(BIN): $(BUILD_DIR) $(OBJs) $(UNIT_TEST_OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(OBJs) -o $@ $(LDLIBS)

# Rule for building the object files from source
$(OBJ_DIR)/%.o: $(SRC)/%.cpp | $(OBJ_DIR)
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule for compiling the unit test object
$(UNIT_TEST_OBJ): $(UNIT_TEST_SOURCE) | $(TEST_DIR)
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

build_unit_test:
#Create object file for unit test
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $(UNIT_TEST_SOURCE) -o $(UNIT_TEST_OBJ)
#Create object file for main program
	$(CXX) $(CFLAGS) $(CPPFLAGS) -g $(UNIT_TEST_SOURCE) -o $(TEST_DIR)/$(BIN) $(LDLIBS)

build_loaddata_test:
#build xt_lib and unit_test_source
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $(UNIT_TEST_SOURCE) -o $(UNIT_TEST_OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c  $(xt_lib) -o $(XT_LIB_OBJ)
#build main program
	$(CXX) $(CFLAGS) $(CPPFLAGS) -g $(UNIT_TEST_SOURCE) $(xt_lib) -o $(TEST_DIR)/$(BIN_LOADDATA) $(LDLIBS)

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
	@echo "SRCs: $(SRCs)"
	@echo "OBJs: $(OBJs)"
	@echo "UNIT_TEST_OBJ: $(UNIT_TEST_OBJ)"

# Clean rule to remove generated files
clean:
	$(RM) $(BIN)
	$(RM) $(BUILD_DIR)
