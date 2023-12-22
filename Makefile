# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -g

# Executable name
TARGET = bin/main

# Source files
SOURCES = $(wildcard *.cpp)

# Object files directory
OBJDIR = obj

# Object files
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Build rule
$(TARGET): $(OBJECTS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Compile rule
$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) bin
