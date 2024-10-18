#include <stdio.h>
#include <stdlib.h>
//string libary super important
#include <string.h>
#include <unistd.h>
//this is for the rest of it
#include <sys/types.h>
#include <sys/wait.h>
//color
#include <curses.h>

void motd() {
    //color (foreground, background)
    attron(COLOR_PAIR(1));
    printw("\nWelcome to kelkBash!\n");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    printw("       .\n");
    printw("      \":\"\n");
    printw("    ___:____     |\"\\/\"|\n");
    printw("  ,'        `.    \\  /\n");
    printw("  |  O        \\___/  |\n");
    printw("~^~^~^~^~^~^~^~^~^~^~^~^~\n");
    printw("This Is Version 1.0! 10/15/2024\nIntellectual Property of lukaskelk.com\n\n");
    attroff(COLOR_PAIR(2));

    refresh();                                  
}

int main() {
    
    //color stuff
    initscr();      
    start_color();         
    init_pair(1, COLOR_CYAN, COLOR_BLACK); 
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_WHITE);
    
    // Call the function to print the MOTD
    motd();
    //define input as 8-bit character 0 - 255
    char input[256]; 
    
    //buffer to hold current working directory
    char cwd[1024]; 
     //buffer to hold the arguments when we fork
    char *commands[100];

    int count = 0;

    while(1){
        

    
        char path[1024];
        //self explainy
        getcwd(path, sizeof(path));

        printw("kelkBash Ω %s$ ", path);

        refresh(); 

        //get input from user
        getnstr(input, sizeof(input) - 1);;
        
        //remove null character from the end of the input
        input[strcspn(input, "\n")] = '\0';

        //token will take the input and split it into "tokens" basically a collection of strings seprated by spaces
        //if we call strtok again with NULL instead of input we will move to the next token in the total string
        char *token = strtok(input, " ");

        //exit command
        if (strcmp(input, "exit") == 0) {
            printf("CLOSING PEACE OUT kelkBasexih...\n");
            //leaving the loop which ends the bash
            break;
        }

        if (strncmp(input, "cd",2) == 0) {
            //next token should be the path
            token = strtok(NULL, " ");
            chdir(token);
            //we need to continue so that it dosent get stuck in the command
            continue; 
        }

        //initally background run is set to zero we default to not in the background
        int bg = 0;
            //one beofre the null character check if its &
        if (input[strlen(input) - 1] == '&') {
            //If it is meant to be background command set the toggle for our check later
            bg = 1; 
            //we remove the ampersand and replace it back with the null character
            input[strlen(input) - 1] = '\0';
        }

        //FOOOOOOOOOOOOORK we create a child process to excute a command
        pid_t pid = fork();
        //if == 0 we a child
        if (pid == 0) {
            int i = 0;
            while (token != NULL) {
                //assign the next i to be the token
                commands[i] = token;
                //calling strtok again using null will continue out tokenizatoin from where we left off so it will give the next
                //word in the command
                token = strtok(NULL, " ");
                i++;
            }
            //make the last element NULL
            commands[i] = NULL;
            //execute the commands from commands[0] to commands[length]
            execvp(commands[0], commands);
        } 
        //check for parent porcess
        else if (pid > 0)  {
            
            if (bg == 0) {
                //color (foreground, background)
                init_pair(1, COLOR_CYAN, COLOR_BLACK); 
                init_pair(2, COLOR_YELLOW, COLOR_BLACK);
                init_pair(3, COLOR_CYAN, COLOR_WHITE);
                    if(count == 5){
                        clear();
                        count = 0;
                    } else {count++;}

                //no background execution
                attron(COLOR_PAIR(1));
                printw("----Starting program----\n");
                refresh();
                wait(NULL);
                move(getcury(stdscr) + 1, 0);
                printw("----Program ended----\n");
                attroff(COLOR_PAIR(1));
                refresh();

                

                
            } else {                
                if(count == 5){
                    clear();
                    count = 0;
                } else {count++;}
                clear();
                attron(COLOR_PAIR(1));
                printw("Started background process with PID: %d\n", pid);
                refresh();
                attroff(COLOR_PAIR(1));
                //The shell will continue its main loop

            }
        }
    }
    //for curses
    endwin(); 
    return 0;
}
