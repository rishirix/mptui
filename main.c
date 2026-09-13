#include <stdio.h>
#include <unistd.h>
#include "terminal.h"
#include "player.h"
#include "tui.h"
#include "playlist.h"


int main(int argc, char *argv[]){
	if(argc<2){
		printf("No Argument passed\n");
		return 1;
	}
	playlist_load(argv[1]);
	terminal_set();
	tui_init();
	tui_draw(playlist[current_song]);
	mpv_handle *player=mpv_create();
	if(mpv_initialize(player) < 0){
		//printf("Failed to initialize mpv\n");
		return 1;
	}
	//player_load(player,argv[1]);
	player_load(player,playlist[current_song]);
	int is_paused=0;
	tui_update_status(is_paused);
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
					tui_update_status(is_paused);
					break;
				case 'l':
					player_seek(player,2);
					break;
				case 'h':
					player_seek(player,-2);
					break;
				case 'n':
					if(current_song>=song_count-1){
						break;
					}
					current_song++;
					player_load(player,playlist[current_song]);
					tui_update_header(playlist[current_song]);
					break;
				case 'b':
					if(current_song<=0){
						break;
					}
					current_song--;
					player_load(player,playlist[current_song]);
					tui_update_header(playlist[current_song]);
					break;
			}
		}
		if(event->event_id==MPV_EVENT_END_FILE){
			mpv_event_end_file *end=event->data;
			if(end->reason==MPV_END_FILE_REASON_EOF){
				break;
			}
		}
	}
	tui_cleanup();
	mpv_destroy(player);
	return 0;
}
