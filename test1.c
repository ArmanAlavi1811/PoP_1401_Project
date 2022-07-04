#include "project.h"

void empty_buffer(){

    while(getchar() != '\n');

}

int is_valid_input(int start, int end){
    // checks to see if a an input is inside a given interval
    int my_input;

    printf("please choose an option: ");

    scanf("%d" ,&my_input);

    while(my_input < start || my_input > end){
        printf("The given input was invalid, try again: ");
        empty_buffer();
        scanf("%d" ,&my_input);
    }

    return my_input;
}



void starting_menu(){
    printf("1- Create a new form format\n");
    printf("2- Change an existing form format\n");
    printf("3- Use a form\n");
    // printf("\n");
    // printf("\n");
    // printf("\n");
    // printf("\n");
    printf("0- exit\n");
    printf("\n");
}

void creator(){
    int form_x, form_y, form_h, form_w;
    int i,j,k;
    scanf("%d %d %d %d", form_x, form_y, form_h, form_w);

    Loop_LT(j, 0, form_y) printf("\n");
    loop_LTE(i, 0 ,form_x) printf(" ");

    Loop_LT(i, 0, form_w) printf("*");

    

//     **
//    *  *
//    *  *
//    *  *
//     ** 
}

int main(){
    system("cls");
    
    Forever{
        starting_menu();
        int initial_input = is_valid_input(0, 3);

        if(initial_input == 1){
            creator();
            break;
        }

        elif(initial_input == 2){
            continue;
        }

        elif(initial_input == 3){
            continue;
        }

        elif (initial_input == 0){
            printf("I hate linux");
            return 0;
        }


    }
}