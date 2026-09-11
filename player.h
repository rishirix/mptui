#ifndef PLAYER_H
#define PLAYER_H

#include <mpv/client.h>


void player_load(mpv_handle *player, const char *filename);
void player_pause(mpv_handle *player, int *is_paused);
void player_seek(mpv_handle *player, int seconds);

#endif
