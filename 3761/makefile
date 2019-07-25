CC := g++
CFLAGS := -I./include\
   		  -I./frame\
   		  -I./common
LDFLAGS := -lACE
OBJ:= lib/qgdw_3761_api.o\
	  lib/con3761adp.o\
	  lib/fmprint.o\
	  lib/TcpClient.o

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
	rm $(OBJ) *.o */*.o *.exe
