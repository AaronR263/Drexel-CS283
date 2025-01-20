#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int is_white_space(char);

int setup_buff(char *buff, char *user_str, int len){
    int usr_str_len = 0;
    int is_space = 0;
    int filled_buff_size = 0;
 
    // skip leading spaces
    while (is_white_space(*user_str)) {
        user_str++;
    }

    for ( ; *user_str != '\0'; user_str++){
        if (is_white_space(*user_str)) {
            is_space = 1;
        } 
        else {
            // Check if buffer is full  // SHOULD MOVE THIS ABOVE TO BE CLEANER TEST IT
            if (filled_buff_size + 1 > len) {
                return -1;
            }
            // If whitespace was in input, put one space in buffer
            if (is_space) {
                *buff = ' ';
                buff++;
                filled_buff_size++;
                is_space = 0;
            }
            *buff = *user_str;
            filled_buff_size++;
            buff++;
        }
        usr_str_len++;
    }

    // Fill remaining buff space with dots
    for (int i = filled_buff_size; i < BUFFER_SZ; i++) {
        *buff = '.';
        buff++;
    }
  
    return filled_buff_size;  
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    putchar('[');
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int space_cnt = 0;

    if (str_len > len) {
        return -1;
    }

    for (int i = 0; i < str_len - 1; i++) {
        if (*buff == ' ')
            space_cnt++;
        buff++;
    }
    return space_cnt + 1;
}

int reverse_buffer(char* buff, int len, int usr_str_len) {

    if (usr_str_len > len) {
        return -1;
    }

    char temp;
    char* buff_start = buff;
    char* buff_end = buff + usr_str_len - 1;
   
    while ( buff_start < buff_end) {
        temp = *buff_start;
        *buff_start = *buff_end;
        *buff_end = temp;

        buff_start++;
        buff_end--;
    }

    return 0;
}

int word_print(char* buff, int len, int usr_str_len) {

    if (usr_str_len > len) {
        return -1;
    }

    int word_cntr = 0;
    int char_cntr = 0;
    char* end = buff + usr_str_len - 1;

    printf("Word Print\n----------\n");

    while (buff <= end + 1) {
        printf("%d. ", word_cntr + 1);

        while (*buff != ' ' && buff <= end + 1) {
            putchar(*buff);
            buff++;
            char_cntr++;
        }

        printf("(%d)\n",char_cntr);

        char_cntr = 0;
        word_cntr++;

        if (buff <= end + 1)
            buff++;
    }

    return 0;
}

int is_white_space(char input){
    return input == ' ' || input == '\t' || input == '\n';
}
//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      If argv[1] does not exist, then argc would be less than two.
    //      The first consition of the or statement is evaluated first.
    //      Because the first condition is true, and because of the truth table
    //      of OR, the second condition would not need to be evaluated to 
    //      evaluate the OR statement, so the error would never occur. 
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      Checks if all required command line arguments are supplied
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string
    
    // #3 Allocate space for the buffer using malloc and
    //    handle error if malloc fails by exiting with a 
    //    return code of 99
    buff = (char*) malloc(BUFFER_SZ);
    if (buff == NULL) {
        exit(99);
    }
    
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            rc = reverse_buffer(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            break;

        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}


//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          This function may be used in other contexts than just in this file
//          or program. It is wise to build functions with modularity in mind
//          to accomodate for future changes in design.
//          Also, the name of the functions don't specify that they only
//          use the main buffer in this program. Maybe you want to use the functions 
//          with a different buffer that has a different length. 