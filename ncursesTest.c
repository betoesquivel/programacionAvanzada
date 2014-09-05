#include <ncurses.h>

int main(){
    initscr();/* Start curses mode */
    printw("Hello World !!!");/* Print Hello World*/
    int c = getch();/* Wait for user input */
    printw("Hello %d", c);
    refresh();
    endwin();/* End curses mode*/

    return 0;
}
