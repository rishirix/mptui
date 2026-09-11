#include <stdio.h>
#include <mpv/client.h>

int main(int argc, char *argv[]){
	if(argc<2){
		printf("No Argument passed\n");
		return 1;
	}
	mpv_handle *player=mpv_create();
	if(mpv_initialize(player) < 0){
		printf("Failed to initialize mpv\n");
		return 1;
	}
	const char *cmd[] = {"loadfile",argv[1],NULL};
	mpv_command(player,cmd);
	while(1){
		mpv_event *event = mpv_wait_event(player,-1);
		if(event->event_id==MPV_EVENT_END_FILE){
			break;
		}
	}
	mpv_destroy(player);
	return 0;
}
