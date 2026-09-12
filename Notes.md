# Project Overview
`mptui` in its current state is a C based music player for terminal with termios and libmpv for terminal based commands for music playback using arguments.

## Architecture
It uses a `player.c` file for controlling the playback using mpv and `terminal.c` file for switching the terminal to RAW mode for keypressess and interactivity.
Finally the `main.c` binds all the other src files into one.
It uses `Makefile` to build the project into a executable binary.

## What I Learned
- argc is the count of arguments and argv is the strings passed at the count
- pointer is concept which is used to point towards a memory address where the actual value of a variable resides, for the is_paused we used the pointer because if we use another variable or pass variable as argument it will update the copy and not the actual value but using a pointer helps change the actual value at the address
- a memory leak is when a program **allocates** memory but never **frees** it, so that memory stays occupied even when its no longer being used.
- raw mode is mode in terminal which parses the keypress directly rather than the user pressing return to execute the registered keypress, we used it for playback controls ease of use.
- A header file is declaration of the functions used in a C source file and it exists to prove that the defined function exists. It also tells the compiler what **arguments** the function takes and what it **returns**, so the compiler can catch mistakes when its called wrongly. 
- a race condition is when multiple threads try to read/write the same variable simultaneously causing unpredictable results.
- Separation of concerns refer to a situation where separating functions makes it easier to manage code. Practical example in this case is if needed only certain functions need to be modified not the whole code base as a whole.

## SRC Code explanations
### main.c
```C
#include <stdio.h>
#include <unistd.h>
#include "terminal.h"
#include "player.h"
```
These contains the header files which are needed to call functions, the headers in the quotes are user-defined and the headers in arrow brackets are system headers.

`<stdio.h>` this header is the standard I/O header responsible for basic operations like input and output in the src file.
`<unistd.h>` this header contains all the symbolic constants and data types and functions like `read()` which is used to read the keypress in our case.
`"terminal.h"` this header is from the `terminal.c` file and declares all the functions used in it with arguments it takes and parameters it process.
`"player.h"` this header is from the `player.c` file and does the same thing as the **terminal.h** header.

```C
int main(int argc, char *argv[]){
	terminal_set();
	if(argc<2){
		printf("No Argument passed\n");
		return 1;
	}
```
The main function takes arguments as `argc` and `argv` which are argument count and argument vector respectively, argument count is understandable from the word but in case of argument vector, it represents the strings passed for that particular count.
Example:
```C
mptui hello
```
In the above example we can see the argc as 2 . When `argv[0]`it is the first argument which is `mptui` in this case and `argv[1]` is hello, so if the condition was to print all the argv then it would print `mptui hello` again.

The `terminal_set()` is a function call from the `terminal.h` header which is a function in `terminal.c`. Next we have a condition where if the argc is less than 2 then print no argument passed which means its an empty and exit the program with code 1 representing error encountered.

```C
mpv_handle *player=mpv_create();
	if(mpv_initialize(player) < 0){
		printf("Failed to initialize mpv\n");
		return 1;
	}
```
This part is where the music player instance is created and initialized, the `mpv_handle` type is used to create a pointer `player` and then instance is created with `mpv_create()` function, then comes the condition where if the player is not initialized then return exit code and error.

```C
enable_raw_mode();
	player_load(player,argv[1]);
```
Now the enable raw mode function is called to switch the terminal to use raw inputs along with loading the initialized instance of player and play the file passed as the argument for the player.

```C
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
```
Now this part is actually where the functions from the `player.c` are called for playback controls, although barebone at the moment it can **seek**, **pause** and **quit** the player.
First we set a variable which tells if the player is paused, then we set a variable that tells if the player is running, Now with the help of `while` loop, we start the player process to play the music and check for any events happening during the duration of the event.
We create a pointer to `event` which contains the function `mpv_wait_event` and this takes arguments as the player and a predefined code in `libmpv`. Here `0` means dont wait and key instantly for keypress and then take appropriate action, if it would have been `-1` it would freeze till the event ends and would not take any event handles.
Next we have variable `key` with char datatype, which is going to `read()` and store the raw input from the keyboard, the 0 as arg here is from any input into the terminal and then reference to the key meaning store the keypress in the key variable. Following it is the conditional statement and switch case conditions, which handles the explicitly defined keypresses. Finally the last if condition checks the event with the current **event_id**, if the event is where the event has reached the end of the file it breaks the loop and moves to the next statement.

```C
mpv_destroy(player);
```
The final function call from the explicitly used `libmpv`, we close the instance of the player and destroy the process and free the memory. And then return the exit code `0` which means program executed successfully.

### terminal.c
```C
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

void terminal_set(){
	tcgetattr(0,&original);
	atexit(restore_terminal);
}
```
The includes are pretty straight-forward, the actual job starts from the `structs`. The struct `termios` has predefined datatypes to handle the terminal state we create a variable with the predefined struct and save it **original**. Now the function `restore_terminal` is created to restore the terminal to its original state when the job is done, it uses a function from the predefined `termios` library which is `tcsetattr` and flushes the original default settings of terminal into the current terminal instance.
Now we have another default struct termios **raw** which stores the same default settings as the original one but this time we introduce the function `enable_raw_mode` which does the following:
- gets the current default attributes of the **raw** termios
- sets the flag to disable `echo` and `ICANON`
- sets the attribute to the **raw** terminal mode and flushes the settings into the raw mode.
Finally we create the function, `terminal_set` which sets the terminal to default settings by getting the values from the original termios struct and then call the function `restore_terminal` upon exit.

### player.c
```C
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
```
The header files are self-explanatory, the functions are where it all lies, the function `player_load` takes the argument of player and filename and then the parameters defined in the function which is `cmd` does one job which is load the filename passed as argument from the main function and **loadfile** using the defined `libmpv` which is command defined in it. The `mpv_command` then takes 2 arguments which are player and the command passed to the player.
Then comes the function `player_pause` which handles pausing the player, the arguments are simple to understand, it takes the pointer to player and pointer to **is_paused** variable defined in main.c which controls and checks if the player is paused or playing. Inside the function it is a conditional statement.
Finally the `player_seek` function which takes pointer to player and seconds from the main function, it then converts the integer seconds to characters for the mpv command to understandand process the seek command from the `libmpv`.

