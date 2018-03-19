CPATH=$(shell pwd)

### include path
DIR_INC += -I/usr/include
DIR_INC += -I/usr/local/include
#DIR_INC += -I/usr/local/include/log4cplus
DIR_INC += -I$(SF_ROOT)/include
DIR_INC += -I$(SF_ROOT)/include/server_frame_tools
DIR_INC += -I$(SF_ROOT)/include/server_frame_network
DIR_INC += -I$(SF_ROOT)/include/server_frame_business

DIR_INC += -I$(CPATH)

### local path
DIR_SRC =$(CPATH)/source
DIR_OBJ =$(CPATH)/obj


### library path
#LIB_S += -L./libs
LIB_S += -L/usr/local/lib
#LIB_S += -L$(CPATH)/../lib

### dynamic link libraries
#LIB_S += -lpthread
#LIB_S += -lboost_system
#LIB_S += -lboost_thread
#LIB_S += -lboost_timer
#LIB_S += -lboost_filesystem
#LIB_S += -lrt
#LIB_S += -lluajit
#LIB_S += -lcurl
#LIB_S += -lserver_frame_business
#LIB_S += $(CPATH)/libs/liblog4cplus.a
#LIB_S += -llog4cplus
### static link libraries

### macros

MACROS =

SRC = $(wildcard $(DIR_SRC)/*.cpp)
SRC += $(wildcard $(CPATH)/*.cpp)
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC}))

PROJECT_NAME=plugin_iptables

TARGET = lib$(PROJECT_NAME).so

CXX = g++
CFLAGS := $(MACROS) $(DIR_INC) -rdynamic -Wall -fPIC -fexceptions
LD_FLAGS := $(MACROS) -rdynamic -shared -Wall -fPIC -Wl,-z,defs -fexceptions
LD = ld


all: $(DIR_OBJ) $(TARGET)
#	mkdir -p $(TARGET_DIR)
#	cp -f $(TARGET) $(TARGET_DIR)/
#	mkdir -p $(TARGET_INC_DIR)
#	cp -rf $(CPATH)/include/*.h* $(TARGET_INC_DIR)/
	
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
release: LD_FLAGS+=-O2
release: rebuild
