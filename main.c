#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <mpv/client.h>

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

int main(int argc, char *argv[]){
	tcgetattr(0,&original);
	atexit(restore_terminal);
	if(argc<2){
		printf("No Argument passed\n");
		return 1;
	}
	mpv_handle *player=mpv_create();
	if(mpv_initialize(player) < 0){
		printf("Failed to initialize mpv\n");
		return 1;
	}
	enable_raw_mode();
	const char *cmd[] = {"loadfile",argv[1],NULL};
	const char *seek_cmd[] = {"seek","2",NULL};
	const char *peek_cmd[] = {"seek","-2",NULL};
	mpv_command(player,cmd);
	int is_paused=0;
	int is_running=1;
	while(is_running){
		mpv_event *event = mpv_wait_event(player,0);
		char key;
		if(read(0,&key,1)==1){
			switch (key){
				case 'q':
					is_running=0;
					break;
				case 'p':
					if(is_paused==0){
						mpv_set_property_string(player,"pause","yes");
						is_paused=1;
					}
					else{
						mpv_set_property_string(player,"pause","no");
						is_paused=0;
					}
					break;
				case 'l':
					mpv_command(player,seek_cmd);
					break;
				case 'h':
					mpv_command(player,peek_cmd);
					break;
			}
		}
		if(event->event_id==MPV_EVENT_END_FILE){
			break;
		}
	}
	mpv_destroy(player);
	return 0;
}
