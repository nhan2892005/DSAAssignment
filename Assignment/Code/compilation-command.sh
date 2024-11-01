# Compile
INCLUDE1="Code/include"
INCLUDE2="Code/include/tensor"
INCLUDE3="Code/include/sformat"
INCLUDE4="Code/include/ann"
INCLUDE5="Code/demo"
SRC1="Code/src/ann/"
SRC2="Code/src/tensor"
MAIN="Code/src/program.cpp"

echo "################################################"
echo "# Compilation of the assignment: STARTED #######"
echo "################################################"

g++ -std=c++17 -I "$INCLUDE1" -I "$INCLUDE2" -I "$INCLUDE3" -I "$INCLUDE4" -I "$INCLUDE5" $(find $SRC1 -type f -iregex ".*\.cpp") "$SRC2"/*.cpp "$MAIN"  -o program

echo "################################################"
echo "# Compilation of the assignment: END     #######"
echo "# Binary file output: ./program ################"
echo "################################################"