build:
	gcc main.c tui.c terminal.c player.c -lmpv -lncursesw -o mptui
install:
	chmod 755 mptui && cp mptui /usr/local/bin/mptui
