#include "project.h"

typedef struct text_box{
    
    char lable[11];
    int lable_len;

    int hegiht;
    int width;

    //startx and starty show top left side of the text box.
    int startx;
    int starty;

    //lable_startx and lable_starty show the starting position of the lable
    int lable_startx;
    int lable_starty;

} Text_box;

typedef struct form{
    char name[11];
    int num_of_boxes;
    Text_box t_boxes[10];
    int forms_filled;
    char answers[10][50];

    int is_fillable; // If a form is fillable, it is a format rather than a filled form.
    int is_editable; // If a form is editable, it is a filled form rather than a format.

} Form;

int is_valid_input(int start, int end){
    // checks to see if a an input is inside a given interval
    char my_input;
    int x,y;

    printw("Please choose an option\n");
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
    
    //Menu shown at the start of the program
    move(LINES - 7, 0);
    printw("1- Create a new form format\n");
    printw("2- Delete a box from a form format\n");
    printw("3- Use a form\n");
    printw("0- exit\n");
    printw("\n");
}

void using_menu(){

    //Menu shown when the user chooses to use a form format, for procedures like filling a form or editing one. 
    move(LINES - 8, 0);
    printw("1- Fill a form\n");
    printw("2- Find a specific element in an filled form\n");
    printw("3- Edit a filled form\n");
    printw("4- delete a filled form\n");
    printw("0- return\n");
    printw("\n");
}

void t_box_menu(){
    //Menu shown during creation of the text boxes.
    move(LINES - 5, 0);

    printw("1- Create another text box\n");
    printw("2- Save the form");
    printw("\n");
}

void save_menu_list(){

    //Menu shown during filling or editing a form.
    move(LINES - 4, 0);
    printw("1- fill another text box\n");
    printw("2- Save the filled box\n");
    printw("\n");
}

WINDOW *make_window(int height, int width, int starty, int startx){

    WINDOW *window;
    
    window = newwin(height, width, starty, startx);
    box(window, 0 , 0);
    wborder(window, ' ', ' ', '*', '*','*','*','*','*');

    wrefresh(window);
    return window;
}

void delete_window(WINDOW *window){

    wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
   
    wrefresh(window);
    delwin(window);
}

void box_creator(Form *my_form){

    //Box creator creates text boxes and puts them in the form it received.

    int box_num = my_form->num_of_boxes;
    char *lable_name = malloc (11 * sizeof(char));
    int count = 0;
    char c;
    int ch;
    int is_new_box;

    initscr();
    echo();
    noraw();
    keypad(stdscr, TRUE);
    
    Forever{
        move(LINES - 3, 0);
        printw("Please Enter a name for your text box (at least 4 chars is required with at most 10 chars)\n");

        
        move(LINES - 2, count);
        c = getch();

        if (c == '\n'){
            
            if (count > 10 || count < 4){
                
                noecho();
                erase();
                move(LINES - 3, 0);
                printw("Invalid number of characters, press ENTER to try again");
                refresh();
                while ((c = getch()) != '\n');           

                erase();
                lable_name[0] = '\0';
                count = 0;
                echo();
                continue;

            }

            else {
                is_new_box = 1;
                lable_name[count] = '\0';

                Loop_LT (count, 0, my_form->num_of_boxes){
                    if ( !strcmp(my_form->t_boxes[count].lable, lable_name) ){
                        
                        is_new_box = 0;
                        noecho();
                        erase();
                        refresh();
                        mvprintw(LINES - 3, 0, "The text box already exists, Press ENTER to try again\n");
                        while ((c = getch()) != '\n');
                        erase();
                        count = 0;
                        lable_name[0] = '\0';
                        echo();
                        break;
                    } 
                }

                if (!is_new_box){
                    continue;
                }
                
                else break;
            }
        }   

        lable_name[count++] = c;
        refresh();
    }


    refresh();
    


    int lable_len = strlen(lable_name);
    
    erase();

    move(LINES - 4,0);

    printw("Please enter the width(greater than 6 and smaller than 45) of the text box: %s (For n character maximum, the width should be n + 2) \n" ,lable_name);
    refresh();

    int height = 3;
    int width;

    printw("w: "); 
    scanw("%d" ,&width);

    while (width < 6 || width > 45 ){

        mvprintw(LINES - 4, 0, "Invalid width number\n");
        printw("Please enter the width(greater than 6 and smaller than 45) of the text box: %s (For n character maximum, the width should be n + 2) \n" ,lable_name);
        printw("w: ");
        scanw("%d" ,&width);
    }

    int stary = (LINES - height) / 2;
    int starx = (COLS - width) / 2;

    erase();
    endwin();

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    char *eraser = malloc((lable_len + 3) * sizeof(char)); //eraser is used to delete what was written on the screen. Here we use it to delete the name of the text box when we move it.

    Loop_LT(count, 0,lable_len + 2){
        eraser[count] = ' ';
    }

    eraser[count] = 0;

    WINDOW *used_window;
    WINDOW *my_win = make_window(height, width, stary, starx);
    wrefresh(my_win);

    noecho();
    raw();

    mvprintw(0,0, "Press F2 to save");

    Loop_LT (count, 0, my_form->num_of_boxes){

        mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].lable_startx, "%s", my_form->t_boxes[count].lable);
        printw(": ");
        used_window = make_window(my_form->t_boxes[count].hegiht, my_form->t_boxes[count].width, my_form->t_boxes[count].starty, my_form->t_boxes[count].startx);
        wrefresh(used_window);

    }    

    while((ch = getch()) != KEY_F(2)){

        //Everytime the user presses one of the arrow keys, the position of the box changes and the user can save it by pressing F2. We move the box by creating 
        //A window, deleting the old window, deleting the old lable and printing the new lable.
        switch(ch){
                
            case KEY_LEFT:
    
                if (starx - 1 < lable_len + 2) break;
                delete_window(my_win);
                mvprintw(stary + (height/2), starx - lable_len - 3,"%s",eraser);
                my_win = make_window(height, width, stary,--starx);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",lable_name);
                printw(": ");
               
                break;
                        
            case KEY_RIGHT:

                if (starx + 1 + width > COLS) break;
                delete_window(my_win);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",eraser);
                my_win = make_window(height, width, stary,++starx);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",lable_name);
                printw(": ");

                break;

            case KEY_UP:

                if (stary - 1 < 2) break;
                delete_window(my_win);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",eraser);
                my_win = make_window(height, width, --stary,starx);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",lable_name);
                printw(": ");
            
                break;

            case KEY_DOWN:

                if (stary + 1 + height > LINES - 4) break;
                delete_window(my_win);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",eraser);
                my_win = make_window(height, width, ++stary,starx);
                mvprintw(stary + (height/2), starx - lable_len - 2,"%s",lable_name);
                printw(": ");
                
                break;	
        }
    }

    Text_box *new_box = malloc(sizeof(Text_box));

    strcpy(new_box->lable, lable_name);
    new_box->hegiht = height;
    new_box->lable_len = lable_len;
    new_box->lable_startx = starx - lable_len - 2;
    new_box->lable_starty = stary + (height/2);
    new_box->startx = starx;
    new_box->starty = stary;
    new_box->width = width;

    my_form->t_boxes[box_num] = *new_box;
    my_form->num_of_boxes++;

    endwin();

    initscr();
    noecho();

    erase();
    endwin();

    Forever{
        initscr();
        erase();
        raw();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();

        t_box_menu();
        refresh();
        int initial_input = is_valid_input(1,2);

        if(initial_input == 1){

            if (my_form->num_of_boxes >= 10){
                erase();
                refresh();
                mvprintw(0,0, "Number of Text Boxes is at max");
                getch();
                continue;
            }

            erase();
            endwin();
            box_creator(my_form);
        }

        elif (initial_input == 2){

            FILE *New_Form;

            char form_name[30];
            strcpy(form_name, my_form->name);

            strcat(form_name,".txt");

            New_Form = fopen(form_name, "w");
            fwrite(my_form, sizeof(Form),1,New_Form);
            fclose(New_Form);
            
            return;
        }

    }
}

void form_creator(){

    //Form creator makes a new blank form and passes it to box_creator.

    Form *new_form = malloc(sizeof(Form));
    char *form_name = malloc(11*sizeof(char));
    new_form->is_fillable = 1;
    new_form->is_editable = 0;
    int count = 0;
    char c;

    erase();
    initscr();
    keypad(stdscr, TRUE);
    echo();
    noraw();

    Forever{
        move(LINES - 3, 0);
        printw("Please Enter a name for your form (at least 4 chars is required with at most 10 chars)\n");
        refresh();

        move(LINES - 2, count);
        c = getch();

        if (c == '\n'){
            
            if (count > 10 || count < 4){
                
                noecho();
                erase();
                move(LINES - 3, 0);
                printw("Invalid number of characters, press ENTER to try again");
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
    new_form->forms_filled = 0;

    erase();
    endwin();

    box_creator(new_form);

}

void editor (Form *my_form, int mode){

    //Editor is the core of our program. It works in 3 modes: EDIT, FILL, and FIND. EDIT is for deleting boxes from a format, FILL is for filling a form or editing an already filled form,
    //and FIND is for finding forms that have the same values we want.

    initscr();
    refresh();
    keypad(stdscr, TRUE);
    noraw();
    echo();
    int count; 
    WINDOW *used_window;

    char *form_num = malloc((strlen(my_form->name) + 5) * sizeof(char));
    strcpy(form_num, my_form->name);
    form_num[strlen(my_form->name)] = '#';
    form_num[strlen(my_form->name) + 1] = my_form->forms_filled + '0';
    strcat(form_num, ".txt");

    //We create the name of a file form by attaching #i.txt to it. i is the number of the form, determined by how many forms with the same format were filled before. 
    
    char f[15];
    int choice;
    int done = 0;
    int done_2 = 0;

    Forever{

        Loop_LT (count, 0, my_form->num_of_boxes){

            mvprintw(0,0, "Form name: %s", my_form->name);
            mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].lable_startx, "%s", my_form->t_boxes[count].lable);
            printw(": ");

            used_window = make_window(my_form->t_boxes[count].hegiht, my_form->t_boxes[count].width, my_form->t_boxes[count].starty, my_form->t_boxes[count].startx);
            
            //If the given name by the user matches a text box's lable, its borders change.
            if ( !strcmp(my_form->t_boxes[count].lable, f) ){
                wborder(used_window, ' ',' ','#','#','#','#','#','#');
                done = 1;
                choice = count;
            }

            mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].startx + 1 , "%s" ,my_form->answers[count]);
            wrefresh(used_window);
        }        
    
        if (done){
            raw();
            noecho();
            move(LINES - 2,0);

            printw("Enter to confirm, backspace to go back\n");

            Forever{

                int c = getch();
                if(c == KEY_BACKSPACE){
                    make_window(my_form->t_boxes[choice].hegiht, my_form->t_boxes[choice].width, my_form->t_boxes[choice].starty, my_form->t_boxes[choice].startx);
                    done = 0;
                    noraw();
                    echo();
                    break;

                }

                elif(c == '\n'){
                    done_2 = 1;
                    noraw();
                    echo();                    
                    break;
                }

                else continue;


            }
        }

        if(done_2) break;

        move(LINES - 2, 0 );
        printw("type the name of the lable you want\n");
        scanw("%s" ,f);
        move(LINES - 1, 0);
        clrtoeol();
        move(LINES - 1, 0);

    }

    move(LINES - 2, 0);
    clrtoeol();
    

    if (mode == EDIT){
        
        mvprintw(LINES - 2, 0,"By deleting this text box, forms that were previously made by this format might no function correctly. Do you want to continue?(Y/N)\n");
        char c = getch();

        if (c == 'y' || c == 'Y'){

            Loop_LT(count, choice, my_form->num_of_boxes){
                my_form->t_boxes[count] = my_form->t_boxes[count + 1];
            }

            my_form->num_of_boxes--;

            char changed_format[30];
            
            strcpy(changed_format, my_form->name);
            strcat(changed_format, ".txt");

            FILE *changed_format_file = fopen(changed_format, "w");

            Forever{
                erase();
                refresh();

                fwrite(my_form, sizeof(Form), 1, changed_format_file);

                fclose(changed_format_file);
                return;
            }
        }

        elif (c == 'n' || c == 'N') return;

    }

    int width = my_form->t_boxes[choice].width;

    char answer[50] = {0};

    Forever{
        raw();
        echo();

        mvprintw(LINES - 2,0, "fill the tbox(less than %d chars)\n" ,width - 1);


        scanw("%s",answer);

        if (strlen(answer) == 0 && mode != FIND){
            mvprintw(LINES - 2,0, "The input cannot be empty, please give a new answer. press a key to continue\n");
            getch();
            continue;
        }

        if(strlen(answer) > width - 2){

            raw();
            noecho();
            clrtoeol();
            move(LINES - 1, 0);
            printw("char lim exceeded, press anything");
            getch();
            move(LINES - 1, 0);
            clrtoeol();
            refresh();

            answer[0] = '\0';

            continue;
        }

        else{

            strcpy(my_form->answers[choice], answer);
            //We add the answer in the 2D array answers in my form and its index is the same as the index of the box.
            //Side note: my first idea was to save all the answers in a text box and the answer for text box[n] would be in (n)th line.
            //I ended up creating a text file that was about 10 gigabytes which filled my Linux's allocated storage completely and crashed my PC :). So I used an array instead.

            Loop_LT (count, 0, my_form->num_of_boxes){
                
                mvprintw(0,0, "Form name: %s", my_form->name);
                mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].lable_startx, "%s", my_form->t_boxes[count].lable);
                printw(": ");
                
                used_window = make_window(my_form->t_boxes[count].hegiht, my_form->t_boxes[count].width, my_form->t_boxes[count].starty, my_form->t_boxes[count].startx);
                mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].startx + 1 , "%s" ,my_form->answers[count]);
                wrefresh(used_window);
            }

            move(LINES - 1, 0);
            clrtoeol();
            refresh();
        }

        strcpy(my_form->answers[choice], answer);
        endwin();
        break;
    }

    initscr();
    my_form->is_fillable = 0;
    my_form->is_editable = 1;

    Loop_LT (count, 0, my_form->num_of_boxes){
        mvprintw(0,0, "Form name: %s", my_form->name);
        mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].lable_startx, "%s", my_form->t_boxes[count].lable);
        printw(": ");
        used_window = make_window(my_form->t_boxes[count].hegiht, my_form->t_boxes[count].width, my_form->t_boxes[count].starty, my_form->t_boxes[count].startx);
        mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].startx + 1 , "%s" ,my_form->answers[count]);
        wrefresh(used_window);
    }

    if (mode == FIND){
        //After we have filled our form, there is no need to to save it to a file in FIND mode, so the function ends here.
        return;
    }


    Forever{

        refresh();
        raw();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();

        save_menu_list();
        refresh();

        int initial_input = is_valid_input(1,2);

        if(initial_input == 1){
            clear();
            endwin();
            editor(my_form, FILL);
        }

        elif(initial_input == 2){

            FILE *filled = fopen(form_num, "w");

            fwrite_unlocked(my_form, sizeof(Form),1, filled);

            fclose(filled);
            return;
        }

    }
    
}

void print_form(Form *my_form){

    int count;
    WINDOW *used_window;
    Loop_LT (count, 0, my_form->num_of_boxes){
        

        mvprintw(0,0, "Form name: %s", my_form->name);
        mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].lable_startx, "%s", my_form->t_boxes[count].lable);
        printw(": ");
        used_window = make_window(my_form->t_boxes[count].hegiht, my_form->t_boxes[count].width, my_form->t_boxes[count].starty, my_form->t_boxes[count].startx);
        mvprintw(my_form->t_boxes[count].lable_starty, my_form->t_boxes[count].startx + 1 , "%s" ,my_form->answers[count]);
        wrefresh(used_window);
    }
}

void use(){

    Forever{


        initscr();
        erase();
        refresh();
        raw();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();

        using_menu();
        refresh();
        int initial_input = is_valid_input(0, 4);

        if (initial_input == 1){ //Filling a form.

            erase();
            endwin();

            initscr();
            raw();
            Form *my_form = (Form *)malloc(sizeof(Form));
            FILE *E;   

            echo();

            move(0,0);
            printw("Enter the name of the file\n");
            char filename[15];
            scanw("%s" ,filename);
            strcat(filename, ".txt");
            erase();
            refresh();
            E = fopen(filename,"r");

            if (E == NULL){
                mvprintw(0,0, "there was a problem opening the file, press anything to abort the operation\n");
                getch();
                return;

            }

            fread(my_form, sizeof(Form), 1,  E);

            if (!my_form->is_fillable){
                erase();
                refresh();
                mvprintw(0,0,"The file is not fillable, press anything to continue");
                getch();

                endwin();
                return;
            }
            
            fclose(E);

            E = fopen(filename, "w");
            //We have to save our form again because the number of filled forms in that format has changed.
            
            my_form->forms_filled++;
            fwrite(my_form, sizeof(Form), 1, E);
            fclose(E);
            
            editor(my_form, FILL);

            erase();
            refresh();
            endwin();
            return;

        }

        elif(initial_input == 2){// finding elements

            erase();
            endwin();

            initscr();
            raw();
            Form *my_form = (Form *)malloc(sizeof(Form));
            FILE *E;   

            echo();

            move(0,0);
            printw("Enter the name of the file\n");
            char filename[15];
            scanw("%s" ,filename);
            strcat(filename, ".txt");
            erase();
            refresh();
            E = fopen(filename,"r");

            if (E == NULL){

                mvprintw(0,0, "there was a problem opening the file, press anything to abort the operation\n");
                getch();
                return;

            }

            fread(my_form, sizeof(Form), 1,  E);

            if (!my_form->is_fillable){
                erase();
                refresh();
                mvprintw(0,0,"This is not a form foramt, press anything to continue");
                getch();

                endwin();
                return;
            }

            editor(my_form, FIND);
            endwin();

            Forever{

                erase();
                refresh();
                raw();
                noecho();

                move(LINES - 4, 0);
                printw("1- Fill Another Box\n");
                printw("2- Search\n");

                initial_input = is_valid_input(1,2);

                if(initial_input == 1){
                    erase();
                    refresh();
                    editor(my_form, FIND);
                }

                elif(initial_input == 2){
                    break;
                }
            }   



            int count;

            erase();
            refresh();
            move(0,0);

            Loop_LTE(count, 1, my_form->forms_filled){
                
                //After entring the boxes we want to search for, we go through all the forms that were created with the same format.

                char serach_file[30];
                strcpy(serach_file, my_form->name);
                strcat(serach_file, "#");
                
                char form_num[2] = {0};
                form_num[0] = count + '0';

                strcat(serach_file, form_num);
                strcat(serach_file, ".txt");

                FILE *Searched = fopen(serach_file, "r");

                if (Searched == NULL){

                    continue;
                }

                Form *searched_form = (Form *)malloc(sizeof(Form));

                fread(searched_form, sizeof(Form), 1, Searched);

                int count2;
                int is_same = 1;

                if (my_form->num_of_boxes != searched_form->num_of_boxes) continue;

                Loop_LT(count2, 0, my_form->num_of_boxes){
                    if(!strlen(my_form->answers[count2])){
                        continue;
                    }

                    if(strcmp(my_form->answers[count2], searched_form->answers[count2])){
                        is_same = 0;
                        break;
                    }

                }

                if (is_same){
                    
                    int c;
                    curs_set(1);
                    
                    print_form(searched_form);
                    
                    attron(A_BLINK);
                    attron(A_BOLD);
                    mvprintw(1,0,"form_name: %s" ,serach_file);
                    attroff(A_BOLD);
                    attroff(A_BLINK);
                    move(1,0);

                    mvprintw(LINES - 2, 0, "press anything to see the next file, press F2 to exit\n");
                    c = getch();
                    if (c == KEY_F(2)){
                        return;
                    }
                    curs_set(0);
                }
            }


        }

        elif (initial_input == 3){//Editing a filled form

            erase();
            endwin();

            initscr();
            raw();
            Form *my_form = (Form *)malloc(sizeof(Form));
            FILE *E;   

            echo();

            move(0,0);
            printw("Enter the name of the file\n");
            char filename[15];
            scanw("%s" ,filename);
            strcat(filename, ".txt");
            erase();
            refresh();
            E = fopen(filename,"r");

            if (E == NULL){
                mvprintw(0,0, "there was a problem opening the file, press anything to abort the operation\n");
                getch();
                return;

            }

            fread(my_form, sizeof(Form), 1,  E);

            if (!my_form->is_editable){
                erase();
                refresh();
                mvprintw(0,0,"The file is not editable, press anything to continue");
                getch();

                endwin();
                return;
            }
            
            fclose(E);
            
            editor(my_form, FILL);

            erase();
            refresh();
            endwin();
            return;


        }
        

        elif (initial_input == 4){//Deleting a filled form

            erase();
            endwin();

            initscr();
            raw();
            Form *my_form = (Form *)malloc(sizeof(Form));
            FILE *E;   

            echo();

            move(0,0);
            printw("Enter the name of the file\n");
            char filename[20];
            scanw("%s" ,filename);
            strcat(filename, ".txt");
            erase();
            refresh();
            E = fopen(filename,"r");

            if (E == NULL){
                mvprintw(0,0, "there was a problem opening the file, press anything to abort the operation\n");
                getch();
                return;

            }

            fread(my_form, sizeof(Form), 1,  E);

            if (my_form->is_fillable){
                erase();
                refresh();
                mvprintw(0,0,"The file is not removable, press anything to continue");
                getch();

                endwin();
                fclose(E);
                return;
            }
            
            fclose(E);

            char filename2[20] = "rm ";
            strcat(filename2, filename);


            system(filename2);

            erase();
            refresh();
            endwin();
            return;

        }

        elif (initial_input == 0){
            return;
        }
    }
}

int main(){
    
    Forever{
        
        initscr();
        curs_set(0);
        erase();
        refresh();
        raw();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();

        starting_menu();
        refresh();
        int initial_input = is_valid_input(0, 3);

        if(initial_input == 1){//Create a form
            endwin();
            form_creator();
        }

        elif(initial_input == 2){//Delete a text bos from a form

            erase();
            endwin();

            initscr();
            raw();
            Form *my_form = (Form *)malloc(sizeof(Form));
            FILE *E;   

            echo();

            move(0,0);
            printw("Enter the name of the file\n");
            char filename[15];
            scanw("%s" ,filename);
            strcat(filename, ".txt");
            erase();
            refresh();
            E = fopen(filename,"r");

            if (E == NULL){

                mvprintw(0,0, "there was a problem opening the file, press anything to abort the operation\n");
                getch();
                endwin();

            }

            else{

                fread(my_form, sizeof(Form), 1,  E);

                if (!my_form->is_fillable){
                    erase();
                    refresh();
                    mvprintw(0,0,"This is not a form foramt, press anything to continue");
                    getch();

                    endwin();
                }
                else{
                    editor(my_form, EDIT);
                }

            }
        }

        elif(initial_input == 3){// Use form formats
            erase();
            endwin();

            use();
            
        }

        elif (initial_input == 0){//Exit

            erase();
            move(0,0);
            printw("\nExiting...\n");
            refresh();
            sleep(3);
            endwin();
            return 0;
        }

    }

}