#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int execute_commands(command_list_t *clist) {

    for (int i = 0; i < clist->num; i++) {
        command_t cmd = clist->commands[i];


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


int count_pipes(char* cmd_line)
{
    int num_pipes = 0;
    for (int i = 0; cmd_line[i] != '\0'; i++) {
        if (cmd_line[i] == '|')
            num_pipes++;
    }
    return num_pipes;
}

char* skip_spaces(char* str) {
    while (*str == SPACE_CHAR)
            str++;
    return str;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    int at_end = 0;
    clist->num = 0;

    // Remove any leading spaces
    cmd_line = skip_spaces(cmd_line);

    // Check if amt of commands exceeds CMD_MAX
    int num_pipes = count_pipes(cmd_line);
    if (num_pipes > CMD_MAX - 1) {
        return ERR_TOO_MANY_COMMANDS;
    }
            
    while (!at_end) {
        
        command_t cmd = {0};

        // Store exe in command
        int i = 0;
        while (*cmd_line != SPACE_CHAR && *cmd_line != PIPE_CHAR && *cmd_line != '\0') {
            if (i == (EXE_MAX - 1))
                return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd.exe[i++] = *cmd_line++;
        }
        cmd.exe[i] = '\0';

        // Check if cmd_line ended
        if (*cmd_line == '\0') 
            at_end = 1;
        else if (*cmd_line == PIPE_CHAR) {
            clist->num += 1;
            clist->commands[clist->num - 1] = cmd;
            cmd_line++;
            continue;
        } 
            

        cmd_line = skip_spaces(cmd_line);
        
        // Store args in cmd if any
        i = 0;
        while (*cmd_line != PIPE_CHAR && *cmd_line != '\0') {

            if (i == (ARG_MAX - 1))
                return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd.args[i++] = *cmd_line++;
        }
        cmd.args[i] = '\0';

        if (*cmd_line == '\0') {
            at_end = 1;
        } else if (*cmd_line == PIPE_CHAR) {
            cmd_line++;
        }

        // remove any spaces
        cmd_line = skip_spaces(cmd_line);

        clist->num += 1;
        clist->commands[clist->num - 1] = cmd;
        
    }
  
    return OK;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
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
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
 
    int rc = 0;

    // TODO IMPLEMENT MAIN LOOP
    while(1){
        cmd_buff_t cmd_buff = {0};
        cmd_buff.argc = 0;

        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff._cmd_buffer, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff._cmd_buffer[strcspn(cmd_buff._cmd_buffer,"\n")] = '\0';
        

        // Check if buff is empty
        if (cmd_buff._cmd_buffer[0] == '\0') {
            printf(CMD_WARN_NO_CMD);
            continue;
        } else {
            // load single command into cmd_buff

            int i = 0;
            int at_end = 0;

             //skip leading spaces
            while (cmd_buff._cmd_buffer[i] == SPACE_CHAR) {
                i++;
            }

            if (cmd_buff._cmd_buffer[i] == '\0')
                at_end = 1;

            while (!at_end) {
                
                //set argument
                cmd_buff.argv[cmd_buff.argc] = &(cmd_buff._cmd_buffer[i]);
                cmd_buff.argc++;
                
                //skip input
                while (cmd_buff._cmd_buffer[i] != SPACE_CHAR && cmd_buff._cmd_buffer[i] != '\0') {
                    i++;
                }
                    
                if (cmd_buff._cmd_buffer[i] == '\0') {
                    at_end = 1;
                } else {
                    cmd_buff._cmd_buffer[i] = '\0';
                    i++;
                    while (cmd_buff._cmd_buffer[i] == SPACE_CHAR) {
                        i++;
                    }

                    if (cmd_buff._cmd_buffer[i] == '\0') {
                        at_end = 1;
                    }

                }
                

            }
            
            printf("\narg1:%s.\n", cmd_buff.argv[0]);
            printf("\narg2:%s.\n", cmd_buff.argv[1]);
            
            // built in commadns
            // move to lower section. Command should be parsed first
            if (strcmp(cmd_buff.argv[0], EXIT_CMD) == 0) {
                exit(0);
            }

            if (strcmp(cmd_buff.argv[0], "cd") == 0) {
                chdir(cmd_buff.argv[1]);
            }
            switch (rc) {
                case OK:
                    //print_cmd_list(&clist);
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

        //execute_commands(&clist);
 
    }
    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}
