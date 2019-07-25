CC := g++
CFLAGS := -I.
LDFLAGS := -lACE
OBJ:= qgdw_3761_api.o\
	  con3761adp.o\
	  fmprint.o\
	  TcpClient.o

main.exe: $(OBJ) ztest/main.o
	@$(CC) $(CFLAGS) $(LDFLAGS) $+ -o $@
	@ls -1shS $@

test.exe: $(OBJ) ztest/test.o
	@$(CC) $(CFLAGS) $(LDFLAGS) $+ -o $@
	@ls -1shS $@

3761.exe: $(OBJ) ztest/3761.o
	@$(CC) $(CFLAGS) $(LDFLAGS) $+ -o $@
	@ls -1shS $@

%.o:%.cpp
	$(CC) $(CFLAGS)  $+ -c -o $@

clean:
	rm *.o *.exe
