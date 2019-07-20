CFLAGS:= -Iinclude -I376.1/include
LDFLAGS:= -lACE
SRC:= src/*.cpp
ARCH:=arm-linux-
ARCH:=
CC:=$(ARCH)g++

STRIP:=$(ARCH)strip

tcpclient.exe: $(SRC) examples/client.cpp
	$(CC) $(CFLAGS) $(LDFLAGS)  $+ -o $@
	$(STRIP) $@
	@ls -sh $@

login3761.exe: $(SRC) examples/login3761.cpp
	$(CC) $(CFLAGS) $(LDFLAGS)  $+ -o $@
	$(STRIP) $@
	@ls -sh $@

arm_tcpclient.exe: *.cpp
	$(CC) $(CFLAGS) $(LDFLAGS)  $+ -o $@
	$(STRIP) $@
	@ls -sh $@

clean:
	rm *.exe
