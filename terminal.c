#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios original;
void restore_terminal(){
	tcsetattr(0,TCSAFLUSH,&original);
}
struct termios raw;
void enable_raw_mode(){
	tcgetattr(0,&raw);
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0,TCSAFLUSH,&raw);
}

void terminal_set(){
	tcgetattr(0,&original);
	atexit(restore_terminal);
}
