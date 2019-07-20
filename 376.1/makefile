CFLAGS:= -Iinclude
test.exe: test/*.cpp
	g++ $(CFLAGS) $+ -o $@

armtest.exe: test/*.cpp
	arm-linux-g++ $(CFLAGS) $+ -o $@

clean:
	rm -rf *.exe
