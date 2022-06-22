CC = g++

# C++ �����Ϸ� �ɼ�
CFLAGS = -g -Wall -O2

# ��Ŀ �ɼ�
LDFLAGS = -lws2_32 -lsecur32

# �ҽ� ���� ���丮
SRC_DIR = ./src

# ������Ʈ ���� ���丮
OBJ_DIR = ./obj

# �����ϰ��� �ϴ� ���� ���� �̸�
TARGET = main

BUILD_DIR = ./bin
# Make �� �ҽ� ���ϵ�
# wildcard �� SRC_DIR ���� *.cc �� �� ���ϵ� ����� �̾Ƴ� �ڿ�
# notdir �� ���� �̸��� �̾Ƴ���.
# (e.g SRCS �� foo.cc bar.cc main.cc �� �ȴ�.)
SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

OBJS = $(SRCS:.cpp=.o)

# OBJS ���� object ���ϵ� �̸� �տ� $(OBJ_DIR)/ �� ���δ�.
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: main

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

.PHONY: ssl_client
ssl_client: $(OBJ_DIR)/ssl_client.o $(OBJ_DIR)/common.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/ssl_client.o $(OBJ_DIR)/common.o -o ssl_client $(LDFLAGS)
	size $@.exe
	mv $@.exe bin

.PHONY: ssl_server
ssl_server: $(OBJ_DIR)/ssl_server.o $(OBJ_DIR)/common.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/ssl_server.o $(OBJ_DIR)/common.o -o ssl_server $(LDFLAGS)
	size $@.exe
	mv $@.exe bin

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET) $(BUILD_DIR)/*.exe

-include $(DEPS)