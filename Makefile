INC_DIR:= ./include  ./include/threadpool ./include/net ./include/log
SRC_DIR:= ./src/ ./src/threadpool ./src/net ./src/log

OBJS:= $(wildcard ./src/*.cpp) $(wildcard ./src/net/*.cpp) $(wildcard ./src/threadpool/*.cpp) $(wildcard ./src/log/*.cpp)
INC_FILE:= $(addprefix -I ,$(INC_DIR))

CXX:= g++
CXXFLAGS:= -std=c++11 -g -Wno-deprecated
LIBS:= -llog4cpp -lpthread 
RM:= rm -rf

TARGET:= ./server/server.exe
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INC_FILE)  $(LIBS)

clean:
	$(RM) $(TARGET)
