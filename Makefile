#Ming Kit Choy
# This is a Makefile for building SFML programs.

CXX = g++
CPPFILES1= test1.cpp
CPPFILES2= test2.cpp
CPPFILES3= test3.cpp

CPPFLAGS= -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -I/opt/homebrew/Cellar/sfml/2.6.1/include
EXE1= prog1
EXE2= prog2
EXE3= prog3

# Automatically generate dependency files
DEPENDENCIES = $(CPPFILES1:.cpp=.d) $(CPPFILES2:.cpp=.d) $(CPPFILES3:.cpp=.d)

# Include the dependency files
-include $(DEPENDENCIES)

# This rule says that each .o file depends on a .cpp file of the same name
# This is actually built into Make but anyways...
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(DEBUGFLAGS)
	$(CXX) -MM -MT $@ $< >> $*.d

ALL: $(EXE1) $(EXE2) $(EXE3)

$(EXE1): $(CPPFILES1:.cpp=.o)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(EXE2): $(CPPFILES2:.cpp=.o)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(EXE3): $(CPPFILES3:.cpp=.o)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(EXE1) $(EXE2) $(EXE3) $(OBJS) $(DEPENDENCIES)

# This rule generates documentation using Doxygen
doxygen:
	doxygen Doxyfile
