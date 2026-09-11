#include <stdio.h>
#include <unistd.h>
#include "terminal.h"
#include "player.h"


int main(int argc, char *argv[]){
	terminal_set();
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
	player_load(player,argv[1]);
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
					player_pause(player,&is_paused);
					break;
				case 'l':
					player_seek(player,2);
					break;
				case 'h':
					player_seek(player,-2);
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
