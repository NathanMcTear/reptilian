CFLAGS = -Wall -std=c++17
TEST = testlibwad
TARGET_DIR = libWad
TARGET = libWad.a

all: test

test: $(TEST).cpp $(TARGET_DIR)/$(TARGET)
	g++ $(CFLAGS) -o test $< -L ./$(TARGET_DIR) -lWad

$(TARGET_DIR)/$(TARGET): $(TARGET_DIR)/*.cpp $(TARGET_DIR)/*.h
	@$(MAKE) -C $(TARGET_DIR)

clean: 
	@$(MAKE) -C $(TARGET_DIR) clean
	rm -f test *.o