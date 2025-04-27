Uses UNIX based libraries such as termios (for terminal manipulation), so either compile with a WSL or Mac.

Compile both .c files, e.g. clang main.c classes.c -o <name>.exe

Then to run just type ./<name>.exe. 

There is also an additional debugging mode that shows various attributes about the snake/board, this is activated by adding any additional argument when executing the program. For example './<name>.exe 1' will cause the debugging mode to activate. This mode is really buggy though since it doesn't print onto the terminal using a buffer, causing visual glitches.

The game was made in a modular design with the intention of expansion; whether I actually expand it is another question. 
