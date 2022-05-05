# WINDOWS ONLY!!!!

include .env

INC = -I. -I$(VULKAN_SDK_PATH)/include -I$(GLFW_PATH)/include -I$(GLM_PATH)
LIB = -L$(VULKAN_SDK_PATH)/Lib -L$(GLFW_PATH)/lib-mingw-w64

CFLAGS = -std=c++17 -Wall -Wno-unknown-pragmas -O2
LDFLAGS = -lvulkan-1 -lglfw3dll

# ACQUIRE SHADER SOURCES AND SET AS DEPENDENCY
VERTSRC = $(wildcard *.vert)
VERTOBJ = $(VERTSRC:.vert=.spv)
FRAGSRC = $(wildcard *.frag)
FRAGOBJ = $(VERTSRC:.frag=.spv)

TARGET = xiv.exe

$(TARGET): $(VERTOBJ) $(FRAGOBJ)
$(TARGET): *.cpp *.h
	g++ $(CFLAGS) $(INC) -o $(TARGET) *.cpp $(LIB) $(LDFLAGS)

# MAKE SHADER TARGETS
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: xiv.exe
	./xiv.exe

clean:
	rm -f xiv.exe
	rm -f *.spv