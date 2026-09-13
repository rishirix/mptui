#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define MAX_SONGS 1000
#define MAX_PATH 512

char playlist[MAX_SONGS][MAX_PATH];
int song_count=0;
int current_song=0;

void playlist_load(const char *dirpath){
	DIR *dir = opendir(dirpath);
	if(dir==NULL){
		return;
	}
	struct dirent *entry;
	while((entry=readdir(dir))!=NULL){
		if(song_count>=MAX_SONGS){
			break;
		}
		if(strstr(entry->d_name,".mp3")){
			char fullpath[MAX_PATH];
			snprintf(fullpath,MAX_PATH, "%s/%s",dirpath,entry->d_name);
			strncpy(playlist[song_count],fullpath,MAX_PATH);
			song_count++;
		}
	}
	closedir(dir);
}
