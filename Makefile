all: simple die large server
simple: simple.c cgi.c
	gcc -Wall -g -o $@ $^
large: large.c cgi.c
	gcc -Wall -g -o $@ $^
die: die.c
	gcc -Wall -g -o $@ $^
server: server.c
	gcc -Wall -g -o $@ $^
clean:
	rm simple large die server
tests:
	./test.sh msg1 msg1.out
	./test.sh msg2 msg2.out
	./test.sh msg3 msg3.out
	./test.sh realmessages realmessages.out
	./test.sh abaaba abaaba.out

