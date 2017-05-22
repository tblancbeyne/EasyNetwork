SOURCES = Layer.cpp main.cpp Neuron.cpp Network.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main
LIBS =

%.o: %.cpp Layer.hpp Neuron.hpp Network.hpp
	$(CXX) -std=c++14 -o $@ -c $< -Wall

all: $(OBJECTS)
	$(CXX) -std=c++14 -o $(TARGET) $(OBJECTS) $(LIBS) -Wall

x: all
	./$(TARGET)

clean:
	rm -rf $(OBJECTS)

superclean: clean
	rm -rf $(TARGET)
