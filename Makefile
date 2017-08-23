BUILD_DIR=    build/
INSTALL_DIR=  /usr/local/bin

BIN_DIR=    bin/
DAT_DIR=    dat/
ETC_DIR=    etc/
INC_DIR=    inc/
LIB_DIR=    lib/
OBJ_DIR=    obj/
DOC_DIR=    share/doc/
MAN_DIR=    share/man/
SRC_DIR=    src/
TST_DIR=    tst/

CC=         gcc
CFLAGS=     -I$(INC_DIR)
LNKFLAGS=   -lX11
CCX=        g++
CCXFLAGS=   -I$(INC_DIR)
SH=         bash

DEPS=	$(wildcard $(INC_DIR)*.h)
SRCS= 	$(wildcard $(SRC_DIR)*.c)
LIBS=	$(DEPS:$(INC_DIR)%.h=$(LIB_DIR)%.o)
OBJS=	$(SRCS:$(SRC_DIR)%.c=$(BUILD_DIR)$(OBJ_DIR)%.o)
TGTS=	$(BUILD_DIR)$(BIN_DIR)battleship


all: $(BUILD_DIR) $(TGTS)

check: all
	echo $(TST_DIR)
	echo "No Tests Implemented"

docs:
	echo $(DOC_DIR)
	echo $(MAN_DIR)
	echo "No Documentation Implemented"

install: $(TGTS)
	cp $(TGTS) $(INSTALL_DIR)

clean:
	rm -rf $(BUILD_DIR)

# Build target

$(TGTS): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LNKFLAGS) -o $@

$(BUILD_DIR)$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)$(BIN_DIR)
	mkdir -p $(BUILD_DIR)$(DAT_DIR)
	mkdir -p $(BUILD_DIR)$(OBJ_DIR)


