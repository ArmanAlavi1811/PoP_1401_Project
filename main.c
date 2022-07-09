#include "project.h"

typedef struct text_box{
    char lable[11];
    int lable_len;
    int hegiht;
    int width;
    int startx;
    int lable_startx;
    int starty;
    int lable_starty;

} Text_box;

typedef struct form{
    char name[11];
    int num_of_boxes;
    Text_box *first_box;

} Form;



void empty_buffer(){

    while(getch() != '\n');

}

int is_valid_name_char(char c){

    if ((c >= 'a' && c <= 'z') || (c <= '9' && c >= '0') || c == '\n' || c == '-' || c == '_' || c == KEY_BACKSPACE) return 1;
    return 0;
}

int is_valid_input(int start, int end){
    // checks to see if a an input is inside a given interval
    char my_input;
    int x,y;

    printw("please choose an option\n");
    getyx(stdscr, y,x);

    my_input = getch() - '0';

    while(my_input < start || my_input > end){
        move(y,x);
        printw("The given input was invalid, try again\n");
        refresh();
        my_input = getch() - '0';
    }

    return my_input;
}



void starting_menu(){
    move(LINES - 7, 0);
    printw("1- Create a new form format\n");
    printw("2- Change an existing form format\n");
    printw("3- Use a form\n");
    // printw("\n");
    // printw("\n");
    // printw("\n");
    // printw("\n");
    printw("0- exit\n");
    printw("\n");
}


WINDOW *create_newwin(int height, int width, int starty, int startx)
{	
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);

    wrefresh(local_win);
    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    	/* box(local_win, ' ', ' '); : This won't produce the desired
  	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window.  */
    
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
 	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window */
    
    wrefresh(local_win);
    delwin(local_win);
}


void box_creator(Form *my_form, int boxes){


    char *lable_name = malloc (11 * sizeof(char));
    int count = 0;
    char c;
    int ch;
    initscr();
    Text_box *new_box = malloc(sizeof(Text_box));
    keypad(stdscr, TRUE);
    
    Forever{
        move(LINES - 3, 0);
        printw("Please Enter a name for your text box (at least one is required with at most 10 chars)\n");

        
        move(LINES - 2, count);
        c = getch();

        if(!is_valid_name_char(c)) continue;


        if (c == '\n'){
            
            if (count > 10){
                
                noecho();
                erase();
                move(LINES - 3, 0);
                printw("char limit exceeded, press ENTER to continue");
                refresh();
                while ((c = getch()) != '\n');           

                erase();
                lable_name[0] = '\0';
                count = 0;
                echo();
                continue;

            }

            else break;
        }

        lable_name[count++] = c;
        refresh();
    }

    lable_name[count] = '\0';
    
    new_box->lable_len = strlen(lable_name);
    strcpy(new_box->lable, lable_name);
    free(lable_name);
    my_form->num_of_boxes++;
    my_form->first_box = new_box;
    erase();

    move(LINES - 4,0);

    printw("please enter the heghit and width of the text box:%s\n" ,my_form->first_box->lable);
    refresh();

    printw("h: "); scanw("%d" ,&my_form->first_box->hegiht);
    printw("w: "); scanw("%d" ,&my_form->first_box->width);

    int height = my_form->first_box->hegiht;
    int width = my_form->first_box->width;

    int stary = my_form->first_box->startx = (LINES - height) / 2;
    int starx = my_form->first_box->starty = (COLS - width) / 2;

    erase();
    endwin();

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    getch();

    char *eraser = malloc(((my_form->first_box->lable_len) + 1) * sizeof(char));

    Loop_LT(count, 0,my_form->first_box->lable_len){
        eraser[count] = ' ';
    }

    eraser[count] = 0;

    WINDOW *my_win = create_newwin(height, width, stary, starx);
    wrefresh(my_win);

    noecho();
    raw();

    mvprintw(0,0, "Press F2 to save");

    while((ch = getch()) != KEY_F(2))
    {
	switch(ch)
	{	
	    case KEY_LEFT:

        if (starx - 1 < my_form->first_box->lable_len) break;
		destroy_win(my_win);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len - 1,"%s",eraser);
		my_win = create_newwin(height, width, stary,--starx);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",my_form->first_box->lable);
		break;
    
	    case KEY_RIGHT:
        if (starx + 1 + width > COLS) break;
		destroy_win(my_win);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",eraser);
		my_win = create_newwin(height, width, stary,++starx);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",my_form->first_box->lable);
		break;

	    case KEY_UP:
        if (stary - 1 < 2) break;
        destroy_win(my_win);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",eraser);
		my_win = create_newwin(height, width, --stary,starx);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",my_form->first_box->lable);
		break;

	    case KEY_DOWN:
        if (stary + 1 + height > LINES) break;
		destroy_win(my_win);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",eraser);
		my_win = create_newwin(height, width, ++stary,starx);
        mvprintw(stary + (height/2), starx - my_form->first_box->lable_len,"%s",my_form->first_box->lable);
		break;	
	}
    }

    my_form->first_box->startx = starx;
    my_form->first_box->starty = starx;
    my_form->first_box->lable_startx = starx - my_form->first_box->lable_len;
    my_form->first_box->lable_starty = stary + (height/2);


    //erase();
    endwin();

    initscr();
    noecho();

    
    Loop_LT (count, 0, my_form->num_of_boxes){
        mvprintw(0,0, "Form name: %s", my_form->name);
        mvprintw(my_form->first_box->lable_starty, my_form->first_box->lable_startx, "%s", my_form->first_box->lable);
        my_win = create_newwin(my_form->first_box->hegiht, my_form->first_box->width, my_form->first_box->starty, my_form->first_box->startx);
        getch();
        destroy_win(my_win);
        erase();
    }


}


void creator(){

    Form *new_form = malloc(sizeof(Form));
    char *form_name = malloc(11*sizeof(char));
    int count = 0;
    char c;

    erase();
    initscr();
    keypad(stdscr, TRUE);
    echo();

    Forever{
        move(LINES - 3, 0);
        printw("please choose a name for your form(at most 10 characters)\n");
        refresh();

        move(LINES - 2, count);
        c = getch();


        if(!is_valid_name_char(c)) continue;

        if (c == '\n'){
            
            if (count > 10){
                
                noecho();
                erase();
                move(LINES - 3, 0);
                printw("char limit exceeded, press ENTER to continue");
                refresh();
                while ((c = getch()) != '\n');           

                erase();
                form_name[0] = '\0';
                count = 0;
                echo();
                continue;

            }

            else break;
        }

        form_name[count++] = c;
        refresh();

    }

    form_name[count] = '\0';

    strcpy(new_form->name, form_name);
    free(form_name);
    new_form->num_of_boxes = 0;

    erase();
    endwin();

    box_creator(new_form, 0);

}



int main(){
    
    Forever{
        initscr();
        erase();
        raw();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();

        starting_menu();
        refresh();
        int initial_input = is_valid_input(0, 3);

        if(initial_input == 1){
            endwin();
            creator();
        }

        elif(initial_input == 2){
            erase();
            printw("%c" ,KEY_BACKSPACE);
            getch();
            continue;
        }

        elif(initial_input == 3){
            continue;
        }

        elif (initial_input == 0){

            erase();
            move(0,0);
            printw("\nI hate linux\n");
            refresh();
            //sleep(3);
            endwin();
            return 0;
        }

    }

}