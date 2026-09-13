#ifndef PLAYLIST_H
#define PLAYLIST_H

#define MAX_SONGS 1000
#define MAX_PATH 512

extern char playlist[MAX_SONGS][MAX_PATH];
extern char song_count;
extern int current_song;

void playlist_load(const char *dirpath);

#endif
