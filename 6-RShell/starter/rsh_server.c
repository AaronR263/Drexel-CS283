
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>

//INCLUDES for extra credit
//#include <signal.h>
//#include <pthread.h>
//-------------------------

#include "dshlib.h"
#include "rshlib.h"

// char* skip_spaces(char* str) {
//     while (*str == SPACE_CHAR)
//             str++;
//     return str;
// }

// int count_pipes(char* cmd_line)
// {
//     int num_pipes = 0;
//     for (int i = 0; cmd_line[i] != '\0'; i++) {
//         if (cmd_line[i] == '|')
//             num_pipes++;
//     }
//     return num_pipes;
// }

// void parse_cmd_into_argv(cmd_buff_t *cmd_buff) {
    
//     int i = 0;
//     int at_end = 0;
//     cmd_buff->out_file = NULL;
//     cmd_buff->in_file = NULL;

//     //skip leading spaces
//     while (cmd_buff->_cmd_buffer[i] == SPACE_CHAR) {
//         i++;
//     }

//     if (cmd_buff->_cmd_buffer[i] == '\0')
//         at_end = 1;

//     while (!at_end && cmd_buff->argc <= 8) {
        
//         int in_quote = 0;
//         int in_redirect_out = 0;
//         int in_redirect_in = 0;

//         if (cmd_buff->_cmd_buffer[i] == '"') {
//             in_quote = 1;
//             i++; 
//         } 
//         else if (cmd_buff->_cmd_buffer[i] == '>') {
//             in_redirect_out = 1;
//             i++;
//         }
//         else if (cmd_buff->_cmd_buffer[i] == '<') {
//             in_redirect_in = 1;
//             i++;
//         }

//         // set argument start in cmd_buff
//         if (!in_redirect_in && !in_redirect_out) {
//             cmd_buff->argv[cmd_buff->argc] = &(cmd_buff->_cmd_buffer[i]);
//             cmd_buff->argc++;
//         }
        
//         while (!in_quote && cmd_buff->_cmd_buffer[i] != SPACE_CHAR && !in_redirect_out && !in_redirect_in && cmd_buff->_cmd_buffer[i] != '\0') {
//             i++;
//             if (cmd_buff->_cmd_buffer[i] == '"')
//                 in_quote = 1;  
//         }
        
//         if (in_redirect_in || in_redirect_out) {
//             //skip spaces
//             while (cmd_buff->_cmd_buffer[i] == SPACE_CHAR) {
//                 i++;
//             }

//             if (in_redirect_in) {
//                 // set direct_in ponter from buffer
//                 cmd_buff->in_file = &cmd_buff->_cmd_buffer[i];
                
//             } else if (in_redirect_out) { 
//                 cmd_buff->out_file = &cmd_buff->_cmd_buffer[i];
//             }
            
//             // read until end of word. /0 is set below. This should also accomodate quotes though, need to refactor to account for this
//             while (cmd_buff->_cmd_buffer[i] != SPACE_CHAR && cmd_buff->_cmd_buffer[i] != '\0') {
//                 i++;
//             }
//             in_redirect_in = 0;
//             in_redirect_out = 0;
//         }

//         // ignores spaces
//         while (in_quote && cmd_buff->_cmd_buffer[i] != '\0') {
//             i++;
//             if (cmd_buff->_cmd_buffer[i] == '"')
//                 in_quote = 0;
//         }

//         if (cmd_buff->_cmd_buffer[i] == '\0') {
//             at_end = 1;
//         } else {
//             cmd_buff->_cmd_buffer[i] = '\0';
//             i++;
//             while (cmd_buff->_cmd_buffer[i] == SPACE_CHAR) {
//                 i++;
//             }

//             if (cmd_buff->_cmd_buffer[i] == '\0') {
//                 at_end = 1;
//             }
//         }
//     }
//     cmd_buff->argv[cmd_buff->argc] = NULL;
// }

// int execute_built_in_commands(cmd_buff_t *cmd_buff) {

//     // redirect here if necesarry.
//     dup_redirect_in(cmd_buff);
//     dup_redirect_out(cmd_buff);

//     // built in commadns
//     if (strcmp(cmd_buff->argv[0], EXIT_CMD) == 0) {
//         exit(0);
//     }

//     if (strcmp(cmd_buff->argv[0], "cd") == 0) {
//         chdir(cmd_buff->argv[1]);
//         printf("cd recievd\n");
//         return OK;
//     }

//     return 33; // means no built in commands found
// }


// int build_cmd_list(char *cmd_line, command_list_t *clist)
// {
//     clist->num = 0;

//     // Check if amt of commands exceeds CMD_MAX
//     int num_pipes = count_pipes(cmd_line);
//     if (num_pipes > CMD_MAX - 1) {
//         return ERR_TOO_MANY_COMMANDS;
//     }

//     int at_end = 0;
//     while (!at_end) {
        
//         cmd_line = skip_spaces(cmd_line);

//         cmd_buff_t *cmd_buff = malloc(sizeof(cmd_buff_t)); // memory leak here, will fix
//         cmd_buff->argc = 0;

//         // Read command into cmd_buff._cmd_buffer
//         int i = 0;
//         while (*cmd_line != PIPE_CHAR && *cmd_line != '\0') {
//             if (i == (EXE_MAX - 1))
//                 return ERR_CMD_OR_ARGS_TOO_BIG;
//             cmd_buff->_cmd_buffer[i++] = *cmd_line++;
//         }
//         cmd_buff->_cmd_buffer[i] = '\0';

//         // parse command into argv array
//         parse_cmd_into_argv(cmd_buff);
//         memcpy(&clist->commands[clist->num], cmd_buff, sizeof(cmd_buff_t)); // argvs will point to old buffer
//         clist->num += 1;

//         // Check if cmd_line ended
//         if (*cmd_line == '\0') 
//             break;
//         else 
//             cmd_line++;
        
//     }
  
//     return OK;
// }

/*
 * start_server(ifaces, port, is_threaded)
 *      ifaces:  a string in ip address format, indicating the interface
 *              where the server will bind.  In almost all cases it will
 *              be the default "0.0.0.0" which binds to all interfaces.
 *              note the constant RDSH_DEF_SVR_INTFACE in rshlib.h
 * 
 *      port:   The port the server will use.  Note the constant 
 *              RDSH_DEF_PORT which is 1234 in rshlib.h.  If you are using
 *              tux you may need to change this to your own default, or even
 *              better use the command line override -s implemented in dsh_cli.c
 *              For example ./dsh -s 0.0.0.0:5678 where 5678 is the new port  
 * 
 *      is_threded:  Used for extra credit to indicate the server should implement
 *                   per thread connections for clients  
 * 
 *      This function basically runs the server by: 
 *          1. Booting up the server
 *          2. Processing client requests until the client requests the
 *             server to stop by running the `stop-server` command
 *          3. Stopping the server. 
 * 
 *      This function is fully implemented for you and should not require
 *      any changes for basic functionality.  
 * 
 *      IF YOU IMPLEMENT THE MULTI-THREADED SERVER FOR EXTRA CREDIT YOU NEED
 *      TO DO SOMETHING WITH THE is_threaded ARGUMENT HOWEVER.  
 */
int start_server(char *ifaces, int port, int is_threaded){
    int svr_socket;
    int rc;

    //
    //TODO:  If you are implementing the extra credit, please add logic
    //       to keep track of is_threaded to handle this feature
    //

    svr_socket = boot_server(ifaces, port);
    if (svr_socket < 0){
        int err_code = svr_socket;  //server socket will carry error code
        return err_code;
    }

    rc = process_cli_requests(svr_socket);

    stop_server(svr_socket);


    return rc;
}

/*
 * stop_server(svr_socket)
 *      svr_socket: The socket that was created in the boot_server()
 *                  function. 
 * 
 *      This function simply returns the value of close() when closing
 *      the socket.  
 */
int stop_server(int svr_socket){
    return close(svr_socket);
}

/*
 * boot_server(ifaces, port)
 *      ifaces & port:  see start_server for description.  They are passed
 *                      as is to this function.   
 * 
 *      This function "boots" the rsh server.  It is responsible for all
 *      socket operations prior to accepting client connections.  Specifically: 
 * 
 *      1. Create the server socket using the socket() function. 
 *      2. Calling bind to "bind" the server to the interface and port
 *      3. Calling listen to get the server ready to listen for connections.
 * 
 *      after creating the socket and prior to calling bind you might want to 
 *      include the following code:
 * 
 *      int enable=1;
 *      setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
 * 
 *      when doing development you often run into issues where you hold onto
 *      the port and then need to wait for linux to detect this issue and free
 *      the port up.  The code above tells linux to force allowing this process
 *      to use the specified port making your life a lot easier.
 * 
 *  Returns:
 * 
 *      server_socket:  Sockets are just file descriptors, if this function is
 *                      successful, it returns the server socket descriptor, 
 *                      which is just an integer.
 * 
 *      ERR_RDSH_COMMUNICATION:  This error code is returned if the socket(),
 *                               bind(), or listen() call fails. 
 * 
 */
int boot_server(char *ifaces, int port){
    int svr_socket;
    int ret;
    
    struct sockaddr_in addr;
    // memset(&addr, 0, sizeof(struct sockaddr_in)); should do? dont think so
    // TODO set up the socket - this is very similar to the demo code
    svr_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Only for development
    int enable=1;
    setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /* Bind socket to socket name. */
    addr.sin_family = AF_INET;
    // if (ifaces != NULL) {
    //     if (inet_pton(AF_INET, ifaces, &addr.sin_addr) <= 0) {
    //         perror("inet_pton failed");
    //         exit(EXIT_FAILURE);
    //     }
    // } else {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //}
    addr.sin_port = htons(port);
    
    ret = bind(svr_socket, (const struct sockaddr *) &addr,
               sizeof(struct sockaddr_in));
    if (ret == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */
    ret = listen(svr_socket, 20);
    if (ret == -1) {
        perror("listen");
        return ERR_RDSH_COMMUNICATION;
    }

    return svr_socket;
}

/*
 * process_cli_requests(svr_socket)
 *      svr_socket:  The server socket that was obtained from boot_server()
 *   
 *  This function handles managing client connections.  It does this using
 *  the following logic
 * 
 *      1.  Starts a while(1) loop:
 *  
 *          a. Calls accept() to wait for a client connection. Recall that 
 *             the accept() function returns another socket specifically
 *             bound to a client connection. 
 *          b. Calls exec_client_requests() to handle executing commands
 *             sent by the client. It will use the socket returned from
 *             accept().
 *          c. Loops back to the top (step 2) to accept connecting another
 *             client.  
 * 
 *          note that the exec_client_requests() return code should be
 *          negative if the client requested the server to stop by sending
 *          the `stop-server` command.  If this is the case step 2b breaks
 *          out of the while(1) loop. 
 * 
 *      2.  After we exit the loop, we need to cleanup.  Dont forget to 
 *          free the buffer you allocated in step #1.  Then call stop_server()
 *          to close the server socket. 
 * 
 *  Returns:
 * 
 *      OK_EXIT:  When the client sends the `stop-server` command this function
 *                should return OK_EXIT. 
 * 
 *      ERR_RDSH_COMMUNICATION:  This error code terminates the loop and is
 *                returned from this function in the case of the accept() 
 *                function failing. 
 * 
 *      OTHERS:   See exec_client_requests() for return codes.  Note that positive
 *                values will keep the loop running to accept additional client
 *                connections, and negative values terminate the server. 
 * 
 */
int process_cli_requests(int svr_socket){
    int     cli_socket;
    int     rc = OK;    

    while(1){
        // TODO use the accept syscall to create cli_socket 
        cli_socket = accept(svr_socket, NULL, NULL);
        if (cli_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("recieved request!\n");
        rc = exec_client_requests(cli_socket);
        if (rc == STOP_SERVER_SC)
            break;
    }

    stop_server(cli_socket);
    return rc;
}

/*
 * exec_client_requests(cli_socket)
 *      cli_socket:  The server-side socket that is connected to the client
 *   
 *  This function handles accepting remote client commands. The function will
 *  loop and continue to accept and execute client commands.  There are 2 ways
 *  that this ongoing loop accepting client commands ends:
 * 
 *      1.  When the client executes the `exit` command, this function returns
 *          to process_cli_requests() so that we can accept another client
 *          connection. 
 *      2.  When the client executes the `stop-server` command this function
 *          returns to process_cli_requests() with a return code of OK_EXIT
 *          indicating that the server should stop. 
 * 
 *  Note that this function largely follows the implementation of the
 *  exec_local_cmd_loop() function that you implemented in the last 
 *  shell program deliverable. The main difference is that the command will
 *  arrive over the recv() socket call rather than reading a string from the
 *  keyboard. 
 * 
 *  This function also must send the EOF character after a command is
 *  successfully executed to let the client know that the output from the
 *  command it sent is finished.  Use the send_message_eof() to accomplish 
 *  this. 
 * 
 *  Of final note, this function must allocate a buffer for storage to 
 *  store the data received by the client. For example:
 *     io_buff = malloc(RDSH_COMM_BUFF_SZ);
 *  And since it is allocating storage, it must also properly clean it up
 *  prior to exiting.
 * 
 *  Returns:
 * 
 *      OK:       The client sent the `exit` command.  Get ready to connect
 *                another client. 
 *      OK_EXIT:  The client sent `stop-server` command to terminate the server
 * 
 *      ERR_RDSH_COMMUNICATION:  A catch all for any socket() related send
 *                or receive errors. 
 */
int exec_client_requests(int cli_socket) {
    int io_size;
    command_list_t cmd_list;
    int rc;
    int cmd_rc;
    int last_rc;
    char *io_buff;
    char *full_rec_message;
    int recv_bytes;
    int  is_last_chunk;
    char eof_char = '\0';

    io_buff = malloc(RDSH_COMM_BUFF_SZ);
    full_rec_message = malloc(RDSH_COMM_BUFF_SZ);

    if (io_buff == NULL){
        return ERR_RDSH_SERVER;
    }

    //use recv to get input
    while (1) {
       
        int total_bytes = 0;
        while((recv_bytes = recv(cli_socket, io_buff, RDSH_COMM_BUFF_SZ, 0)) > 0) {

            if (recv_bytes == 0) {
                // other side went away
                printf("read 0 bytes\n");
                break;
            }
            //printf("recieve data\n", io_buff);
            //printf("iobuff:%.*s \n", (int)recv_bytes, io_buff);
    
            is_last_chunk = ((char)io_buff[recv_bytes - 1] == eof_char) ? 1 : 0;
            
            

            //add recieved bytes to full message
            for (int i = 0; i < recv_bytes; i++, total_bytes++) {
                full_rec_message[total_bytes] = io_buff[i];
            }

            // guarentees buff is null terminated
            if (is_last_chunk){
                io_buff[recv_bytes - 1] = '\0'; 
                //printf("last chunk\n");
                break;
            }
            
        }
        
        // send_message_string(cli_socket, full_rec_message, recv_bytes);
        // send_message_eof(cli_socket);
        //printf("full message sent:%s\n", full_rec_message);
        
        // check if message satifies requirements to be passed to build_cmd_list
          // Check if buff is empty
        if (full_rec_message[0] == '\0') {
            printf(CMD_WARN_NO_CMD);
            //send this to client
            send_message_string(cli_socket, CMD_WARN_NO_CMD, strlen(CMD_WARN_NO_CMD) + 1);
            send_message_eof(cli_socket);
            continue;
            return WARN_NO_CMDS; //ik this wont be reached
        } 

        // build cmd list
        command_list_t command_list = { 0 };
        if (build_cmd_list(full_rec_message, &command_list) == ERR_TOO_MANY_COMMANDS) {
            char err_msg[strlen(CMD_ERR_PIPE_LIMIT) + (CMD_MAX / 10 + 1) + 1];
            //printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            sprintf(err_msg, CMD_ERR_PIPE_LIMIT, CMD_MAX);
            send_message_string(cli_socket, err_msg, strlen(err_msg) + 1);
            send_message_eof(cli_socket);
            continue;
            return ERR_TOO_MANY_COMMANDS; // ik will never be reached
        }

        // check if built in command, if not pass to pipeline
        rc = rsh_built_in_cmd(&command_list.commands[0]);
        if (rc == BI_NOT_BI) {
            rsh_execute_pipeline(cli_socket, &command_list);
        } else if (rc == BI_CMD_EXIT){
            if (close(cli_socket) == -1) {
                perror("error closing socket");
                exit(1);
            }
            break;
        } else if (rc == BI_CMD_STOP_SVR) {
            free(io_buff);
            free(full_rec_message);
            return STOP_SERVER_SC;
        }
        send_message_eof(cli_socket);
             
        
    }   
    

    // TODO build up a cmd_list

        // TODO rsh_execute_pipeline to run your cmd_list

        // TODO send appropriate respones with send_message_string
        // - error constants for failures
        // - buffer contents from execute commands
        //  - etc.
       
        // TODO send_message_eof when done
    return WARN_RDSH_NOT_IMPL;
}

/*
 * send_message_eof(cli_socket)
 *      cli_socket:  The server-side socket that is connected to the client

 *  Sends the EOF character to the client to indicate that the server is
 *  finished executing the command that it sent. 
 * 
 *  Returns:
 * 
 *      OK:  The EOF character was sent successfully. 
 * 
 *      ERR_RDSH_COMMUNICATION:  The send() socket call returned an error or if
 *           we were unable to send the EOF character. 
 */
int send_message_eof(int cli_socket){
    int send_len = (int)sizeof(RDSH_EOF_CHAR);
    int sent_len;
    sent_len = send(cli_socket, &RDSH_EOF_CHAR, send_len, 0);

    if (sent_len != send_len){
        return ERR_RDSH_COMMUNICATION;
    }
    return OK;
}


/*
 * send_message_string(cli_socket, char *buff)
 *      cli_socket:  The server-side socket that is connected to the client
 *      buff:        A C string (aka null terminated) of a message we want
 *                   to send to the client. 
 *   
 *  Sends a message to the client.  Note this command executes both a send()
 *  to send the message and a send_message_eof() to send the EOF character to
 *  the client to indicate command execution terminated. 
 * 
 *  Returns:
 * 
 *      OK:  The message in buff followed by the EOF character was 
 *           sent successfully. 
 * 
 *      ERR_RDSH_COMMUNICATION:  The send() socket call returned an error or if
 *           we were unable to send the message followed by the EOF character. 
 */
int send_message_string(int cli_socket, char *buff, int message_len){
    //TODO implement writing to cli_socket with send()
    char *send_buff = malloc(RDSH_COMM_BUFF_SZ);
    // int buff_len = snprintf((char *)send_buff, 
    //         sizeof(buff), "THANK YOU -> %s", buff);

    send(cli_socket, buff, message_len, 0);
    return WARN_RDSH_NOT_IMPL;
}


/*
 * rsh_execute_pipeline(int cli_sock, command_list_t *clist)
 *      cli_sock:    The server-side socket that is connected to the client
 *      clist:       The command_list_t structure that we implemented in
 *                   the last shell. 
 *   
 *  This function executes the command pipeline.  It should basically be a
 *  replica of the execute_pipeline() function from the last deliverable. 
 *  The only thing different is that you will be using the cli_sock as the
 *  main file descriptor on the first executable in the pipeline for STDIN,
 *  and the cli_sock for the file descriptor for STDOUT, and STDERR for the
 *  last executable in the pipeline.  See picture below:  
 * 
 *      
 *┌───────────┐                                                    ┌───────────┐
 *│ cli_sock  │                                                    │ cli_sock  │
 *└─────┬─────┘                                                    └────▲──▲───┘
 *      │   ┌──────────────┐     ┌──────────────┐     ┌──────────────┐  │  │    
 *      │   │   Process 1  │     │   Process 2  │     │   Process N  │  │  │    
 *      │   │              │     │              │     │              │  │  │    
 *      └───▶stdin   stdout├─┬──▶│stdin   stdout├─┬──▶│stdin   stdout├──┘  │    
 *          │              │ │   │              │ │   │              │     │    
 *          │        stderr├─┘   │        stderr├─┘   │        stderr├─────┘    
 *          └──────────────┘     └──────────────┘     └──────────────┘   
 *                                                      WEXITSTATUS()
 *                                                      of this last
 *                                                      process to get
 *                                                      the return code
 *                                                      for this function       
 * 
 *  Returns:
 * 
 *      EXIT_CODE:  This function returns the exit code of the last command
 *                  executed in the pipeline.  If only one command is executed
 *                  that value is returned.  Remember, use the WEXITSTATUS()
 *                  macro that we discussed during our fork/exec lecture to
 *                  get this value. 
 */
int rsh_execute_pipeline(int cli_sock, command_list_t *clist) {

    // check if only one command and execute accordingly


    int pipes[clist->num - 1][2];  // Array of pipes
    pid_t pids[clist->num];

    int  pids_st[clist->num];         // Array to store process IDs
    Built_In_Cmds bi_cmd;
    int exit_code;

    // Create all necessary pipes
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < clist->num; i++) {
        // TODO this is basically the same as the piped fork/exec assignment, except for where you connect the begin and end of the pipeline (hint: cli_sock)
        // create a process for each command
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        if (pids[i] == 0) {
            // map stdout to the write side of pipe
            // for all except last pipe
            if (i < clist->num -1)
                dup2(pipes[i][1], STDOUT_FILENO);

            if (i == clist->num - 1) {
                dup2(cli_sock, STDOUT_FILENO);
            }
            // maps stdin of current process to read side of 
            // the pipe for the previous process. 
            // aka mapping previous processes stdout to current 
            // proc stdin
            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);
            
            // redirects stdin for 1st cmd if necesary
            if (i == 0) 
                dup_redirect_in(&clist->commands[i]);

            //redirects stdout for last command if necesary
            if (i == clist->num - 1) 
                dup_redirect_out(&clist->commands[i]);

            for (int j = 0; j < clist->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // here we check for redirects. If its the first command can use < to take in input
            // if is the last command can use > to redirect output. 
            // I think you should just ignore it if its not in the first or last. (maybe you can do both but later problem)
             // Execute command
            dup2(cli_sock, STDERR_FILENO);
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
            
        }
    

    }


    // Parent process: close all pipe ends
    for (int i = 0; i < clist->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], &pids_st[i], 0);
    }

    //by default get exit code of last process
    //use this as the return value
    exit_code = WEXITSTATUS(pids_st[clist->num - 1]);
    for (int i = 0; i < clist->num; i++) {
        //if any commands in the pipeline are EXIT_SC
        //return that to enable the caller to react
        if (WEXITSTATUS(pids_st[i]) == EXIT_SC)
            exit_code = EXIT_SC;
    }

    return exit_code;
}

/**************   OPTIONAL STUFF  ***************/
/****
 **** NOTE THAT THE FUNCTIONS BELOW ALIGN TO HOW WE CRAFTED THE SOLUTION
 **** TO SEE IF A COMMAND WAS BUILT IN OR NOT.  YOU CAN USE A DIFFERENT
 **** STRATEGY IF YOU WANT.  IF YOU CHOOSE TO DO SO PLEASE REMOVE THESE
 **** FUNCTIONS AND THE PROTOTYPES FROM rshlib.h
 **** 
 */

/*
 * rsh_match_command(const char *input)
 *      cli_socket:  The string command for a built-in command, e.g., dragon,
 *                   cd, exit-server
 *   
 *  This optional function accepts a command string as input and returns
 *  one of the enumerated values from the BuiltInCmds enum as output. For
 *  example:
 * 
 *      Input             Output
 *      exit              BI_CMD_EXIT
 *      dragon            BI_CMD_DRAGON
 * 
 *  This function is entirely optional to implement if you want to handle
 *  processing built-in commands differently in your implementation. 
 * 
 *  Returns:
 * 
 *      BI_CMD_*:   If the command is built-in returns one of the enumeration
 *                  options, for example "cd" returns BI_CMD_CD
 * 
 *      BI_NOT_BI:  If the command is not "built-in" the BI_NOT_BI value is
 *                  returned. 
 */
Built_In_Cmds rsh_match_command(const char *input)
{
    if (strcmp(input, "exit") == 0)
        return BI_CMD_EXIT;
    if (strcmp(input, "dragon") == 0)
        return BI_CMD_DRAGON;
    if (strcmp(input, "cd") == 0)
        return BI_CMD_CD;
    if (strcmp(input, "stop-server") == 0)
        return BI_CMD_STOP_SVR;
    if (strcmp(input, "rc") == 0)
        return BI_CMD_RC;
    return BI_NOT_BI;
}

/*
 * rsh_built_in_cmd(cmd_buff_t *cmd)
 *      cmd:  The cmd_buff_t of the command, remember, this is the 
 *            parsed version fo the command
 *   
 *  This optional function accepts a parsed cmd and then checks to see if
 *  the cmd is built in or not.  It calls rsh_match_command to see if the 
 *  cmd is built in or not.  Note that rsh_match_command returns BI_NOT_BI
 *  if the command is not built in. If the command is built in this function
 *  uses a switch statement to handle execution if appropriate.   
 * 
 *  Again, using this function is entirely optional if you are using a different
 *  strategy to handle built-in commands.  
 * 
 *  Returns:
 * 
 *      BI_NOT_BI:   Indicates that the cmd provided as input is not built
 *                   in so it should be sent to your fork/exec logic
 *      BI_EXECUTED: Indicates that this function handled the direct execution
 *                   of the command and there is nothing else to do, consider
 *                   it executed.  For example the cmd of "cd" gets the value of
 *                   BI_CMD_CD from rsh_match_command().  It then makes the libc
 *                   call to chdir(cmd->argv[1]); and finally returns BI_EXECUTED
 *      BI_CMD_*     Indicates that a built-in command was matched and the caller
 *                   is responsible for executing it.  For example if this function
 *                   returns BI_CMD_STOP_SVR the caller of this function is
 *                   responsible for stopping the server.  If BI_CMD_EXIT is returned
 *                   the caller is responsible for closing the client connection.
 * 
 *   AGAIN - THIS IS TOTALLY OPTIONAL IF YOU HAVE OR WANT TO HANDLE BUILT-IN
 *   COMMANDS DIFFERENTLY. 
 */
Built_In_Cmds rsh_built_in_cmd(cmd_buff_t *cmd)
{
    Built_In_Cmds ctype = BI_NOT_BI;
    ctype = rsh_match_command(cmd->argv[0]);

    switch (ctype)
    {
    // case BI_CMD_DRAGON:
    //     print_dragon();
    //     return BI_EXECUTED;
    case BI_CMD_EXIT:
        return BI_CMD_EXIT;
    case BI_CMD_STOP_SVR:
        return BI_CMD_STOP_SVR;
    case BI_CMD_RC:
        return BI_CMD_RC;
    case BI_CMD_CD:
        chdir(cmd->argv[1]);
        printf("changed dirctory\n");
        return BI_EXECUTED;
    default:
        return BI_NOT_BI;
    }
}
