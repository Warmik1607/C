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
