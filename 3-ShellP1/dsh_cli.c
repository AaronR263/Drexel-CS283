#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h> 
#include "dshlib.h"

// move to lib file probably
void print_cmd_list(command_list_t *clist) 
{
    printf(CMD_OK_HEADER, clist->num);

    for (int i = 0; i < clist->num; i++) 
    {
        command_t cmd = clist->commands[i];

        if (cmd.args[0] != '\0')
            printf("<%d> %s  [%s]\n", i + 1, cmd.exe, cmd.args);
        else
            printf("<%d> %s\n", i + 1, cmd.exe);
    }
}


typedef struct {
    int count;
    char character;
} rle_pair;

static rle_pair dragon_data[] = {
    {80, ' '}, {1, '@'}, {4, '%'}, {24, ' '}, {1, '\n'},
        {77, ' '}, {6, '%'}, {26, ' '}, {1, '\n'},
        {76, ' '}, {5, '%'}, {27, ' '}, {1, '\n'},
        {73, ' '}, {2, '%'}, {7, '%'}, {11, ' '}, {1, '@'}, {15, ' '}, {1, '\n'},
        {64, ' '}, {8, '%'}, {8, ' '}, {8, '%'}, {6, '%'}, {8, ' '}, {1, '@'}, {7, ' '}, {1, '\n'},
        {39, ' '}, {7, '%'}, {3, '%'}, {1, '@'}, {9, ' '}, {9, '%'}, {5, '%'}, {1, '@'}, {5, '%'}, {8, ' '}, {1, '\n'},
        {34, ' '}, {25, '%'}, {6, ' '}, {24, '%'}, {23, ' '}, {1, '\n'},
        {32, ' '}, {28, '%'}, {1, '@'}, {5, ' '}, {16, '%'}, {3, ' '}, {15, '%'}, {12, ' '}, {1, '\n'},
        {29, ' '}, {29, '%'}, {5, ' '}, {19, '%'}, {20, ' '}, {1, '\n'},
        {27, ' '}, {30, '%'}, {1, '@'}, {7, ' '}, {22, '%'}, {9, ' '}, {3, '%'}, {3, ' '}, {1, '\n'},
        {26, ' '}, {32, '%'}, {2, ' '}, {29, '%'}, {16, ' '}, {1, '\n'},
        {28, ' '}, {33, '%'}, {2, ' '}, {28, '%'}, {15, ' '}, {1, '\n'},
        {27, ' '}, {33, '%'}, {2, ' '}, {27, '%'}, {14, ' '}, {1, '\n'},
        {6, ' '}, {9, '%'}, {6, '@'}, {11, ' '}, {24, '%'}, {8, ' '}, {26, '%'}, {6, ' '}, {2, '%'}, {2, ' '}, {1, '\n'},
        {4, ' '}, {12, '%'}, {9, ' '}, {17, '%'}, {11, ' '}, {18, '%'}, {4, ' '}, {4, '%'}, {1, ' '}, {1, '\n'},
        {2, ' '}, {13, '%'}, {10, ' '}, {16, '%'}, {14, ' '}, {16, '%'}, {3, ' '}, {5, '%'}, {1, ' '}, {1, '\n'},
        {1, ' '}, {15, '%'}, {11, ' '}, {15, '%'}, {16, ' '}, {15, '%'}, {2, ' '}, {6, '%'}, {1, ' '}, {1, '\n'},
        {2, ' '}, {16, '%'}, {1, '@'}, {10, ' '}, {14, '%'}, {19, ' '}, {14, '%'}, {1, ' '}, {7, '%'}, {1, ' '}, {1, '\n'},
        {3, ' '}, {17, '%'}, {11, ' '}, {14, '%'}, {22, ' '}, {14, '%'}, {8, '%'}, {1, ' '}, {1, '\n'},
        {4, ' '}, {18, '%'}, {12, ' '}, {13, '%'}, {24, ' '}, {14, '%'}, {9, '%'}, {1, ' '}, {1, '\n'},
        {5, ' '}, {19, '%'}, {13, ' '}, {12, '%'}, {27, ' '}, {14, '%'}, {10, '%'}, {1, ' '}, {1, '\n'},
        {6, ' '}, {20, '%'}, {14, ' '}, {11, '%'}, {30, ' '}, {14, '%'}, {11, '%'}, {1, ' '}, {1, '\n'},
        {7, ' '}, {21, '%'}, {15, ' '}, {10, '%'}, {33, ' '}, {14, '%'}, {12, '%'}, {1, ' '}, {1, '\n'},
        {8, ' '}, {22, '%'}, {16, ' '}, {9, '%'}, {36, ' '}, {14, '%'}, {13, '%'}, {1, ' '}, {1, '\n'},
        {9, ' '}, {23, '%'}, {17, ' '}, {8, '%'}, {39, ' '}, {14, '%'}, {14, '%'}, {1, ' '}, {1, '\n'},
        {10, ' '}, {24, '%'}, {18, ' '}, {7, '%'}, {42, ' '}, {14, '%'}, {15, '%'}, {1, ' '}, {1, '\n'},
        {11, ' '}, {25, '%'}, {19, ' '}, {6, '%'}, {45, ' '}, {14, '%'}, {16, '%'}, {1, ' '}, {1, '\n'}
};


void print_rle_data(rle_pair data[]) {
    int data_size = sizeof(dragon_data) / sizeof(rle_pair);
    for (int i = 0; i < data_size; i++) {
        rle_pair pair = data[i];
        for (int j = 0; j < pair.count; j++) {
            putchar(pair.character);
        }
    }
}

/* This is a temporary implementation
 * Prints output of implemented commands
 */
int execute_commands(command_list_t *clist) {

    for (int i = 0; i < clist->num; i++) {
        command_t cmd = clist->commands[i];

        //temporary implementation
        if (strcmp(cmd.exe, "dragon") == 0) {
            print_rle_data(dragon_data);
            return 0;
        }

        if (strcmp(cmd.exe, "cd") == 0) {
            if (cmd.args[0] != '\0') {
                printf("\nArgs:%s\n", cmd.args);
                chdir(cmd.args);
            }
            return 0;
        }
    }
    return 1;
}
/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char cmd_buff[ARG_MAX]; // is this enough room?
    int rc = 0;
    command_list_t clist;

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            exit(0);
        }

        // Check if buff is empty
        if (cmd_buff[0] == '\0') {
            printf(CMD_WARN_NO_CMD);
            continue;
        } else {
            rc = build_cmd_list(cmd_buff, &clist);

            switch (rc) {
                case OK:
                    print_cmd_list(&clist);
                    break;
                case ERR_TOO_MANY_COMMANDS:
                    printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                    break;
                case ERR_CMD_OR_ARGS_TOO_BIG:
                    // printf(ERR_CMD_OR_ARGS_TOO_BIG);
                    break;
                default:
                    break;
            }
        }  

        execute_commands(&clist);
 
    }

    //printf(M_NOT_IMPL);
    //exit(EXIT_NOT_IMPL);
}