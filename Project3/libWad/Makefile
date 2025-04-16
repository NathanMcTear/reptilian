CFLAGS = -Wall -std=c++17
TARGET = liblibWad.a
OBJS = Wad.o


all: $(TARGET)

$(TARGET): $(OBJS)
	ar cr $@ $^

%.o: %.cpp %.h
	g++ $(CFLAGS) -c $< -o $@

clean: 
	rm -f *.o $(TARGET)