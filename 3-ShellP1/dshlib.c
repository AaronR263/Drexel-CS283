#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

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

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
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