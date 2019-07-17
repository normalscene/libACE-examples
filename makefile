tcpclient.exe: *.cpp
	g++ -lACE $+ -o $@
	strip $@

arm_tcpclient.exe: *.cpp
	arm-linux-g++ -lACE $+ -o $@
	arm-linux-strip $@


clean:
	rm *.exe
