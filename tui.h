#ifndef TUI_H
#define TUI_H

void tui_init();
void tui_draw(const char *filename);
void tui_update_status(int is_paused);
void tui_update_header(const char *filename);
void tui_cleanup();

#endif
