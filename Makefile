SOURCES = Neuron.cpp main.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main
LIBS = 

%.o: %.cpp
	$(CXX) -std=c++14 -o $@ -c $< -Wall 

all: $(OBJECTS)
	$(CXX) -std=c++14 -o $(TARGET) $(OBJECTS) $(LIBS) -Wall

x: all
	./$(TARGET)

clean:
	rm -rf $(OBJECTS)

superclean: clean
	rm -rf $(TARGET)
