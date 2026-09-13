#include <libgen.h>
#include <curses.h>

static WINDOW *header_win;
static WINDOW *center_win;
static WINDOW *bottom_win;

void tui_init(){
	initscr();
	cbreak();
	noecho();

	header_win=newwin(4,COLS,0,0);
	center_win=newwin(LINES-6,COLS,4,0);
	bottom_win=newwin(2,COLS,LINES-2,0);
}

void tui_draw(const char *filename){
	box(header_win,0,0);
	box(center_win,0,0);
	box(bottom_win,0,0);

	char *base = basename((char *)filename);
	mvwprintw(header_win,1,1,"%s",base);
	mvwprintw(bottom_win,1,1,"[p] pause [h] rewind [l] forward [q] quit");

	wrefresh(header_win);
	wrefresh(center_win);
	wrefresh(bottom_win);

}

void tui_update_status(int is_paused){
	werase(bottom_win);
	box(bottom_win,0,0);
	if(is_paused==0){
		mvwprintw(bottom_win,1,1,"[p] pause [h] rewind [l] forward [q] quit | PLAYING");
	}
	else{
		mvwprintw(bottom_win,1,1,"[p] pause [h] rewind [l] forward [q] quit | PAUSED");

	}
	wrefresh(bottom_win);
}

void tui_cleanup(){
	endwin();
}
