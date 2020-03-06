CPATH=$(shell pwd)

### include path
DIR_INC =-I$(CPATH)/../include
DIR_INC += -I$(CPATH)/include
DIR_INC += -I$(CPATH)/../server_frame_tools/include
DIR_INC += -I/usr/include
DIR_INC += -I/usr/local/include
DIR_INC += -I/usr/local/boost/include
#DIR_INC += -I/usr/local/include/log4cplus
#DIR_INC += -I/usr/local/include/luajit-2.1

### local path
DIR_SRC =$(CPATH)/source
DIR_OBJ =$(CPATH)/obj


### library path
#LIB_S += -L./libs
LIB_S += -L/usr/local/lib
LIB_S += -L/usr/local/boost/lib
LIB_S += -L$(CPATH)/../lib

### dynamic link libraries
LIB_S += -lpthread
LIB_S += -lboost_system
LIB_S += -lboost_thread
LIB_S += -lboost_timer
LIB_S += -lboost_filesystem
#LIB_S += -lrt
#LIB_S += -lluajit
#LIB_S += -lcurl
LIB_S += -lserver_frame_tools

### static link libraries

### macros

MACROS = -DSERVER_FRAME_NETWORK_EXPORTS -DBOOST_SPIRIT_THREADSAFE -DBOOST_ALL_DYN_LINK

SRC = $(wildcard $(DIR_SRC)/*.cpp)
SRC += $(wildcard $(CPATH)/*.cpp)
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 

PROJECT_NAME=server_frame_network

TARGET = lib$(PROJECT_NAME).so 
TARGET_DIR := $(CPATH)/../lib
TARGET_INC_DIR := $(CPATH)/../include/$(PROJECT_NAME)

CXX = g++
CFLAGS := $(MACROS) $(DIR_INC) -Wall -fPIC -fexceptions
LD_FLAGS := $(MACROS) -shared -Wall -fPIC -Wl,-z,defs -fexceptions
LD = ld


all: $(DIR_OBJ) $(TARGET)
	mkdir -p $(TARGET_DIR)
	cp -f $(TARGET) $(TARGET_DIR)/
	mkdir -p $(TARGET_INC_DIR)
	cp -rf $(CPATH)/include/*.h* $(TARGET_INC_DIR)/
	
$(DIR_OBJ):
	mkdir -p $(DIR_OBJ)

$(TARGET): $(OBJ)
	$(CXX) $(LD_FLAGS) $(OBJ) $(LIB_S) -o $@
	
	
$(DIR_OBJ)/%.o : $(DIR_SRC)/%.cpp 
	@echo $(DIR_OBJ)
	$(CXX) $(CFLAGS) -o $@ -c $<
	
$(DIR_OBJ)/%.o : $(CPATH)/%.cpp
	@echo $(DIR_OBJ)
	$(CXX) $(CFLAGS) -o $@ -c $<
 
clean:
	rm -f $(DIR_OBJ)/*.o 
	rm -f $(TARGET)
	
rebuild : clean all

debug : CFLAGS+=-g
debug : LD_FLAGS+=-g
debug : rebuild

release: CFLAGS+=-O2
release: LD_CFLAGS+=-O2
release: rebuild
