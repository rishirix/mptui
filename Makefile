build:
	gcc main.c terminal.c player.c -lmpv -o mptui
install:
	chmod 755 mptui && cp mptui /usr/local/bin/mptui
