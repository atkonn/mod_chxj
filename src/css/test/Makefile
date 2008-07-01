all: example-1

example-1: example-1.c
	gcc -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -o example-1 example-1.c ../src/.libs/libscss.a -I../include `apr-1-config --includes` `apr-1-config --libs` -lapr-1

clean:
	rm example-1
