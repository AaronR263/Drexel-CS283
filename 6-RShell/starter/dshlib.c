#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

char* skip_spaces(char* str) {
    while (*str == SPACE_CHAR)
            str++;
    return str;
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

// clean up 
void parse_cmd_into_argv(cmd_buff_t *cmd_buff) {
    
    int i = 0;
    int at_end = 0;
    cmd_buff->out_file = NULL;
    cmd_buff->in_file = NULL;

    //skip leading spaces
    while (cmd_buff->_cmd_buffer[i] == SPACE_CHAR) {
        i++;
    }

    if (cmd_buff->_cmd_buffer[i] == '\0')
        at_end = 1;

    while (!at_end && cmd_buff->argc <= 8) {
        
        int in_quote = 0;
        int in_redirect_out = 0;
        int in_redirect_in = 0;

        if (cmd_buff->_cmd_buffer[i] == '"') {
            in_quote = 1;
            i++; 
        } 
        else if (cmd_buff->_cmd_buffer[i] == '>') {
            in_redirect_out = 1;
            i++;
        }
        else if (cmd_buff->_cmd_buffer[i] == '<') {
            in_redirect_in = 1;
            i++;
        }

        // set argument start in cmd_buff
        if (!in_redirect_in && !in_redirect_out) {
            cmd_buff->argv[cmd_buff->argc] = &(cmd_buff->_cmd_buffer[i]);
            cmd_buff->argc++;
        }
        
        while (!in_quote && cmd_buff->_cmd_buffer[i] != SPACE_CHAR && !in_redirect_out && !in_redirect_in && cmd_buff->_cmd_buffer[i] != '\0') {
            i++;
            if (cmd_buff->_cmd_buffer[i] == '"')
                in_quote = 1;  
        }
        
        if (in_redirect_in || in_redirect_out) {
            //skip spaces
            while (cmd_buff->_cmd_buffer[i] == SPACE_CHAR) {
                i++;
            }

            if (in_redirect_in) {
                // set direct_in ponter from buffer
                cmd_buff->in_file = &cmd_buff->_cmd_buffer[i];
                
            } else if (in_redirect_out) { 
                cmd_buff->out_file = &cmd_buff->_cmd_buffer[i];
            }
            
            // read until end of word. /0 is set below. This should also accomodate quotes though, need to refactor to account for this
            while (cmd_buff->_cmd_buffer[i] != SPACE_CHAR && cmd_buff->_cmd_buffer[i] != '\0') {
                i++;
            }
            in_redirect_in = 0;
            in_redirect_out = 0;
        }

        // ignores spaces
        while (in_quote && cmd_buff->_cmd_buffer[i] != '\0') {
            i++;
            if (cmd_buff->_cmd_buffer[i] == '"')
                in_quote = 0;
        }

        if (cmd_buff->_cmd_buffer[i] == '\0') {
            at_end = 1;
        } else {
            cmd_buff->_cmd_buffer[i] = '\0';
            i++;
            while (cmd_buff->_cmd_buffer[i] == SPACE_CHAR) {
                i++;
            }

            if (cmd_buff->_cmd_buffer[i] == '\0') {
                at_end = 1;
            }
        }
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;
}

void dup_redirect_in(cmd_buff_t *cmd_buff) {
    if (cmd_buff->in_file != NULL) {
        int out_fd = open(cmd_buff->in_file, O_RDONLY, 0644);
        dup2(out_fd, STDIN_FILENO);
    }
}

void dup_redirect_out(cmd_buff_t *cmd_buff) {
    if (cmd_buff->out_file != NULL) {
        int in_fd = open(cmd_buff->out_file, O_WRONLY | O_CREAT, 0644);
        dup2(in_fd, STDOUT_FILENO);
    }
}

int execute_built_in_commands(cmd_buff_t *cmd_buff) {

    // redirect here if necesarry.
    dup_redirect_in(cmd_buff);
    dup_redirect_out(cmd_buff);

    // built in commadns
    if (strcmp(cmd_buff->argv[0], EXIT_CMD) == 0) {
        exit(0);
    }

    if (strcmp(cmd_buff->argv[0], "cd") == 0) {
        chdir(cmd_buff->argv[1]);
        return OK;
    }

    return 33; // means no built in commands found
}


void execute_external_cmds(cmd_buff_t *cmd_buff) {

    // Execute external command
    int f_result, c_result;

    f_result = fork();
    if (f_result < 0){
        perror("fork error");
        exit(1);
    }

    if (f_result == 0) {
        //The child will now exec
        int exec_rc;

        // redirect here if necesarry.
        dup_redirect_in(cmd_buff);
        dup_redirect_out(cmd_buff);

        // printf("argv[0]: '%s'\n", cmd_buff->argv[0]);
        // printf("argv[1]: %p\n", (void*)cmd_buff->argv[1]);
        exec_rc = execvp(cmd_buff->argv[0], cmd_buff->argv);
        if (exec_rc < 0){
            perror("fork error");
            exit(42);   
        }

    } else {
        //This will be where the parent picks up
        wait(&c_result);
        //printf("[p] The child exit status is %d\n", WEXITSTATUS(c_result));
    }
}


int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;

    // Check if amt of commands exceeds CMD_MAX
    int num_pipes = count_pipes(cmd_line);
    if (num_pipes > CMD_MAX - 1) {
        return ERR_TOO_MANY_COMMANDS;
    }

    int at_end = 0;
    while (!at_end) {
        
        cmd_line = skip_spaces(cmd_line);

        cmd_buff_t *cmd_buff = malloc(sizeof(cmd_buff_t)); // memory leak here, will fix
        cmd_buff->argc = 0;

        // Read command into cmd_buff._cmd_buffer
        int i = 0;
        while (*cmd_line != PIPE_CHAR && *cmd_line != '\0') {
            if (i == (EXE_MAX - 1))
                return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd_buff->_cmd_buffer[i++] = *cmd_line++;
        }
        cmd_buff->_cmd_buffer[i] = '\0';

        // parse command into argv array
        parse_cmd_into_argv(cmd_buff);
        memcpy(&clist->commands[clist->num], cmd_buff, sizeof(cmd_buff_t)); // argvs will point to old buffer
        clist->num += 1;

        // Check if cmd_line ended
        if (*cmd_line == '\0') 
            break;
        else 
            cmd_line++;
        
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
    //cmd_buff_t cmd_buff = {0}; //switch to malloc?
   // cmd_buff.argc = 0;

    char cmd_line_buff[ARG_MAX];

    printf("%s", SH_PROMPT);

    // Read user input into _cmd_buffer 
    if (fgets(cmd_line_buff, ARG_MAX, stdin) == NULL) {
        printf("\n");
        return WARN_NO_CMDS;
    }

    //remove the trailing \n from cmd_buff
    cmd_line_buff[strcspn(cmd_line_buff, "\n")] = '\0';
    
    // Check if buff is empty
    if (cmd_line_buff[0] == '\0') {
        printf(CMD_WARN_NO_CMD);
        return WARN_NO_CMDS;
    } 
    
    // build a command list
    command_list_t command_list = { 0 };
    if (build_cmd_list(cmd_line_buff, &command_list) == ERR_TOO_MANY_COMMANDS) {
        printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        return ERR_TOO_MANY_COMMANDS;
    }

    if (command_list.num == 1) {
        if (( rc = execute_built_in_commands(&command_list.commands[0]) ) == OK)
            return OK;
    
        execute_external_cmds(&command_list.commands[0]);
        
    } else {
        // make its own function
        // set up pipe loop and everything
        int pipes[command_list.num - 1][2];
        pid_t pids[command_list.num];

        // create all pipes
        for (int i = 0; i < command_list.num - 1; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // now we have all pipes, so we need to fork and dup
        for (int i = 0; i < command_list.num; i++) {
           
            // create a process for each command
            pids[i] = fork();
            if (pids[i] == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            
            if (pids[i] == 0) {
                // map stdout to the write side of pipe
                // for all except last pipe
                if (i < command_list.num -1)
                    dup2(pipes[i][1], STDOUT_FILENO);

            
                // maps stdin of current process to read side of 
                // the pipe for the previous process. 
                // aka mapping previous processes stdout to current 
                // proc stdin
                if (i > 0)
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                
                // redirects stdin for 1st cmd if necesary
                if (i == 0) 
                    dup_redirect_in(&command_list.commands[i]);

                //redirects stdout for last command if necesary
                if (i == command_list.num - 1) 
                    dup_redirect_out(&command_list.commands[i]);

                for (int j = 0; j < command_list.num - 1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // here we check for redirects. If its the first command can use < to take in input
                // if is the last command can use > to redirect output. 
                // I think you should just ignore it if its not in the first or last. (maybe you can do both but later problem)
                 // Execute command
                execvp(command_list.commands[i].argv[0], command_list.commands[i].argv);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }

        // close pipes for parent
        for (int i = 0; i < command_list.num - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        // wait for children before continuing
        for (int i = 0; i < command_list.num; i++) {
            waitpid(pids[i], NULL, 0);
        }
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

    // STILL NEED TO FREE cmd_buffs
    
    // to past test cases, wont be necesarry when loop is fully implemented
    printf("%s", SH_PROMPT);

    return OK;
}
