#####################################################
# You may need to change the parameters under here. #
#####################################################

# Step 1: Choose a compiler. By default, use clang++

# If you use clang++, make sure the following line does not start with a comment (#)
#CXX=clang++
# If you use g++, uncomment the following line
CXX=g++

# Set default compiler parameters
# -Wall 	shows all warnings when compiling, always use this!
# -std=c++11 	enables the C++11 standard mode
CXXFLAGS = -Wall -std=c++11

# Step 2: If you use gcc 4.6, we need different parameters. Uncomment the following line.
#CXXFLAGS = -Wall -std=c++0x

# Step 3: If you use clang++ under Mac OS X, remove these comments
#CXXFLAGS += -stdlib=libc++
#LFLAGS += -stdlib=libc++
#LIB_CXXFLAGS = $(CXXFLAGS) -Iinclude
# To compile, run 'make' in the same directory as this file

APP_CXXFLAGS = $(CXXFLAGS) -Iinclude

# List of objects for the library
LIBOBJS = obj/tester.o obj/codesys.o obj/matrixcontainer.o obj/codefinder.o

# Name of the library
LIBRARY = lib/liball.a
#########################
# Settings for the apps # 
#########################
LFLAGS += -Llib -lall

PROGRAMOBJS = obj/program.o
PROGRAMAPP = bin/program

# This is the first target. It will be built when you run 'make' or 'make build'
build: $(LIBRARY) $(PROGRAMAPP)

# Create the library by using 'ar'
$(LIBRARY) : $(LIBOBJS)
	ar cr $(LIBRARY) $(LIBOBJS)

# Compile each source file of the librar

obj/tester.o: src/tester.cpp
	$(CXX) $(APP_CXXFLAGS) -c src/tester.cpp -o obj/tester.o

obj/codesys.o: src/codesys.cpp
	$(CXX) $(APP_CXXFLAGS) -c src/codesys.cpp -o obj/codesys.o

obj/codefinder.o: src/codefinder.cpp
	$(CXX) $(APP_CXXFLAGS) -c src/codefinder.cpp -o obj/codefinder.o

obj/matrixcontainer.o: src/matrixcontainer.cpp
	$(CXX) $(APP_CXXFLAGS) -c src/matrixcontainer.cpp -o obj/matrixcontainer.o

# Rules for linking the test apps

$(PROGRAMAPP): $(PROGRAMOBJS)
	$(CXX) $(PROGRAMOBJS) -o $(PROGRAMAPP) $(LFLAGS)

# Compile each source file of the apps
obj/program.o: src/program.cpp
	$(CXX) $(APP_CXXFLAGS) -c src/program.cpp -o obj/program.o

doc: 
	doxygen	

clean: 
	rm -rf $(PROGRAMAPP) $(PROGRAMOBJS) $(LIBRARY) rm $(LIBOBJS)


