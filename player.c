#include <stdio.h>
#include <mpv/client.h>

void player_load(mpv_handle *player, const char *filename){
	const char *cmd[]={"loadfile",filename,NULL};
	mpv_command(player,cmd);
}

void player_pause(mpv_handle *player, int *is_paused){
	if(*is_paused==0){
		mpv_set_property_string(player,"pause","yes");
		*is_paused=1;
	}
	else{
		mpv_set_property_string(player,"pause","no");
		*is_paused=0;
	}
}

void player_seek(mpv_handle *player, int seconds){
	char seconds_str[16];
	snprintf(seconds_str,sizeof(seconds_str),"%d",seconds);
	const char *seek_cmd[]={"seek",seconds_str,NULL};
	mpv_command(player,seek_cmd);
}
