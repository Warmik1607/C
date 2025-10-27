PONG---------------------------------------------------------------------------------------------------------------------------------------------------------
Develop an src/pong.c program for a two-player game,
similar to Pong. To display the graphics, only use ASCII characters (with output
to the terminal). You need to implement the step-by-step
version using only the material of the course studied
so far and the standard library.


Keys:

A/Z and K/M - to move the rackets;
Space Bar - to skip an action at a certain step of the game in step-by-step mode.
After starting the program, it goes to wait for correct input, i.e. one of the players must move their rocket, or miss a move. After that, drawing occurs, and the program goes back to waiting for input. And so the game continues until the game is finished.

Graphics

The field is a 80 x 25-symbol rectangle.
Racket size is 3 symbols;
Ball size is 1 symbol.

UI/UX

When one of the players scores 21 points, congratulations to the winner are displayed on the screen and the game ends.

PONG_INTERACTIVE----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Implement interactive version of the PONG (real-time game mode). To simplify the processing of player actions and display of character graphics, the ncurses library can be used, but the limitations of the main part also apply to the bonus part. The interactive mode code must be in the src/pong_interactive.c file. 

GAME_OF_LIFE--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Develop an src/game_of_life.c program in C – a visualization of The Game of Life. To display the graphics, only use ASCII characters (with output to the terminal). You need to implement interactive version of the Game of life. Also create the options for adjusting the speed of the "game" and the ability to finish it. Library ncurses can be used to simplify the processing of the players' actions and displaying symbolic graphics. ==

Graphics

Treat the playing field - a rectangle of 80 by 25 cells – as a matrix of the state of the "game".
It is assumed that the field is "closed to itself", for example, in the lower right square, the neighbor on the right is the lower left square, and the neighbor on the bottom is the upper right.
Provide for original initialization of the "game" state via stdin. Prepare at least 5 files with initial states for quick launch and initialization through input redirection.


Keys:

A/Z to increase/decrease speed respectively
Space Bar to terminate the game

POLISH_NOTATION-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Develop an src/graph.c program in C for creating graphs of functions defined by arbitrary expressions. To calculate the values ​​of expressions, use Dijkstra's algorithm for translating expressions into Polish notation. The program is assembled using a Makefile. The name of the build stage is all. The executable file must be stored at the root in the build folder named graph. Expressions containing any errors are considered incorrect, including unbalanced or empty brackets, as well as their incorrect nesting, the presence of invalid characters, functions or operators, and any deviations from the rules for constructing mathematical expressions. If the expression cannot be converted to a correct mathematical form for plotting, output n/a. 

Conditions

The size of the field for displaying graphs is a rectangle of 25 by 80 points.
Definition area - [0; 4 Pi]
Range of values - [-1; 1]
Orientation of the coordinate plane - the ordinate axis is directed downward, the abscissa axis is directed to the right.
The center of coordinates is middle of the left border (point {0, 12}).
You don't need to draw axes.
Nothing but the graph should be displayed.
Values are rounded according to the rules of mathematics.

Graphics

The field should be displayed using Star symbol for the chart line and "." for the rest of the screen space.

Modules

Expression parsing and creating a list of lexical units.
Translation into Polish notation and calculation using this notation.
Drawing a graph in the terminal.
Only <math.h>, <stdio.h>, <stdlib.h>, <string.h> and your own modules can be used.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
