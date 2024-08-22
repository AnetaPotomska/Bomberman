PROJECTNAME=potomane
LD=g++
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++17 -g
LIBS=-lncurses
SRC_DIR=src/
DOC_DIR=doc/
BUILD_DIR=build/
C_FILES=$(wildcard $(SRC_DIR)*.cpp)
O_FILES=$(patsubst src/%.cpp, build/%.o, ${C_FILES})

all: compile

run: compile
	./$(PROJECTNAME)

doc: doc/index.html

doc/index.html: Doxyfile examples/README.md $(wildcard $(SRC_DIR)*)
	doxygen Doxyfile

compile: $(PROJECTNAME)

$(PROJECTNAME): $(O_FILES)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(CXXFLAGS) $^ $(LIBS) -o $@ 

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

clean:
	@rm -rf $(BUILD_DIR) $(DOC_DIR)
	@rm -rf $(SRC_DIR)*.o
	@rm ${PROJECTNAME}

count:
	wc -l src/*