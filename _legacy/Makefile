# WINDOWS ONLY!!!!

include .env

SRC_PATH = src
OUT_PATH = bin

INC = -I. -I$(VULKAN_SDK_PATH)/include -I$(TINYOBJ_PATH) -I$(GLFW_PATH)/include -I$(GLM_PATH)
LIB = -L$(VULKAN_SDK_PATH)/Lib -L$(GLFW_PATH)/lib-mingw-w64

CFLAGS = -std=c++17 -Wall -Wno-unknown-pragmas -O2
LDFLAGS = -lvulkan-1 -lglfw3dll

# ACQUIRE SHADER SOURCES AND SET AS DEPENDENCY
VERTSRC = $(wildcard *.vert)
VERTOBJ = $(VERTSRC:.vert=.spv)
FRAGSRC = $(wildcard *.frag)
FRAGOBJ = $(VERTSRC:.frag=.spv)

TARGET = $(OUT_PATH)/xiv.exe

$(TARGET): $(VERTOBJ) $(FRAGOBJ)
$(TARGET): $(SRC_PATH)/*.cpp $(SRC_PATH)/*.h
	g++ $(CFLAGS) $(INC) -o $(TARGET) $(SRC_PATH)/*.cpp $(LIB) $(LDFLAGS)

# MAKE SHADER TARGETS
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: $(TARGET)
	./$(OUT_PATH)/xiv.exe

clean:
	rm -f $(OUT_PATH)/xiv.exe
	rm -f *.spv