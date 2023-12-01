# Joseph Kehoe
# This is very basic. It is just here to show you a simple sfml graphics display that you can use.
# I expect better from you!

CXX = g++
CPPFILES= test.cpp
CPPFILES1= test1.cpp
CPPFLAGS= -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -I/opt/homebrew/Cellar/sfml/2.6.1/include
EXE= prog2
EXE1= prog

OBJS=$(subst .cpp,.o,$(SRCS))

#This rule says that each .o file depends on a .cpp file of the same name
#This is actually built into Make but anyways...
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(DEBUGFLAGS)


ALL: $(OBJS)
	$(CXX) $(CPPFILES) -o $(EXE1) $(LDFLAGS)
	$(CXX) $(CPPFILES1) -o $(EXE) $(LDFLAGS)


#$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)
#g++ test.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o prog  -lsfml-graphics -lsfml-window -lsfml-system
#g++ test.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o prog -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system