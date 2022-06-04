CC = g++

# C++ �����Ϸ� �ɼ�
CFLAGS = -g -Wall -O2

# ��Ŀ �ɼ�
LDFLAGS = -lws2_32

# �ҽ� ���� ���丮
SRC_DIR = ./src

# ������Ʈ ���� ���丮
OBJ_DIR = ./obj

# �����ϰ��� �ϴ� ���� ���� �̸�
TARGET = main

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

.PHONY: client
client: $(OBJ_DIR)/client_test.o $(OBJ_DIR)/common.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/client_test.o $(OBJ_DIR)/common.o -o client $(LDFLAGS)

.PHONY: server
server: $(OBJ_DIR)/server_test.o $(OBJ_DIR)/common.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/server_test.o $(OBJ_DIR)/common.o -o server $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)