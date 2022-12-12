CC = g++

# C++ 컴파일러 옵션
# CXXFLAGS = -g -Wall -O2 "-IC:/Program Files/OpenSSL-Win64/include", -llibssl
CXXFLAGS = -g "-IC:/Program Files/OpenSSL-Win64/include"


# 링커 옵션
LDFLAGS += "-LC:/Program Files/OpenSSL-Win64/lib" -llibcrypto

# 소스 파일 디렉토리
SRC_DIR = .

# 오브젝트 파일 디렉토리
OBJ_DIR = ./obj

# 생성하고자 하는 실행 파일 이름
TARGET = main

# Make 할 소스 파일들
# wildcard 로 SRC_DIR 에서 *.cc 로 된 파일들 목록을 뽑아낸 뒤에
# notdir 로` 파일 이름만 뽑아낸다.
# (e.g SRCS `는 foo.cc bar.cc main.cc 가 된다.)
SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

OBJS = $(SRCS:.cpp=.o)

# OBJS 안의 object 파일들 이름 앞에 $(OBJ_DIR)/ 을 붙인다.
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: main

# $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
# 	$(CC) -c $< -o $@ $(CXXFLAGS) $(LDFLAGS)

# $(TARGET) : $(OBJECTS)
# 	$(CC) $(OBJECTS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

$(OBJ_DIR)/des.o : $(SRC_DIR)/1_des_cbc.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)
des : $(OBJ_DIR)/des.o
	$(CC) $< -o $@ $(LDFLAGS)

$(OBJ_DIR)/aes.o : $(SRC_DIR)/2_aes_128.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)
aes : $(OBJ_DIR)/aes.o
	$(CC) $< -o $@ $(LDFLAGS)


$(OBJ_DIR)/rsa.o : $(SRC_DIR)/3_rsa.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)
rsa : $(OBJ_DIR)/rsa.o
	$(CC) $< -o $@ $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJ_DIR)/*.o des aes rsa

-include $(DEPS)