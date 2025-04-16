CFLAGS = -Wall -std=c++17
TEST = testlibwad
TARGET_DIR = libWad
TARGET = liblibWad.a

all: test

test: $(TEST).cpp $(TARGET_DIR)/$(TARGET)
	g++ $(CFLAGS) -o test $< -L ./$(TARGET_DIR) -llibWad

$(TARGET_DIR)/$(TARGET): $(TARGET_DIR)/*.cpp $(TARGET_DIR)/*.h
	@$(MAKE) -C $(TARGET_DIR)

clean: 
	@$(MAKE) -C $(TARGET_DIR) clean
	rm -f test *.o