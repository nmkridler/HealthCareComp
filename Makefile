all: test

CFLAGS=-fPIC -g -Wall `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv` -lGLEW -lglut -lGL
INCLUDE=-I/usr/local/include/boost 
test:  *.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o $@  $(LIBS) 
%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o test

