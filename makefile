CFLAGS:= -Iinclude -Ilib3761
LDFLAGS:= -lACE
ARCH:=arm-linux-
ARCH:=
CC:=$(ARCH)g++
STRIP:=$(ARCH)strip

SRC:= src/TcpClient.cpp \
	lib3761/con3761adp.cpp\
	lib3761/qgdw_3761_api.cpp

3761.exe: $(SRC) 	examples/3761.cpp
	$(CC) $(CFLAGS) $(LDFLAGS)  $+ -o $@
	$(STRIP) $@
	@ls -sh $@

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
