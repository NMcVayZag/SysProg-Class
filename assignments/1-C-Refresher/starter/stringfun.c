#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here
void word_print(char *buff, int str_len){
    char *buff_ptr = buff;  // Pointer to the first character of the string
    int end_point = str_len;
    int in_word = 0;
    int wc = 0;
    int i = 0;
    int lc = 0;

    printf("Word Print\n");
    printf("----------\n");

    while (i<end_point){
        if(in_word == 0){
            if (*(buff_ptr+i) != ' ')
            in_word = 1;
            lc++;               //maintain letter count
            wc++;               //maintain word count
            printf("%i. ", wc);
            printf("%c", *(buff_ptr+i));
        }else{
            if(*(buff_ptr+i) != ' '){
                printf("%c",*(buff_ptr+i));
                lc++;
            }else{
                in_word = 0;
                printf(" (%d)\n", lc);
                lc= 0;
            }
        }
        i++;
    }
    printf(" (%d)\n", lc); // make sure to print the length of the word in the end of string case.
}
void reverse_string(char *buff, int str_len){
    char *buff_ptr = buff;  // Pointer to the first character of the string
    int begin_point = 0;
    int end_point = str_len -1;
    char temp_char;
    
    while (end_point > begin_point){        //create while loop that goes until all string character indexes have been covered and swapped
    temp_char = *(buff_ptr + begin_point);  // save the character at start index
    *(buff_ptr + begin_point) = *(buff_ptr + end_point);
    *(buff_ptr + end_point) = temp_char;    // define char at end index to as the character from start index
    begin_point++;                          // move from outside in, swapping characters through these two itteration commands.
    end_point--;
    }
    printf("Reversed String: ");
    for (int i = 0; i < str_len; i++) {
    printf("%c", *(buff_ptr + i));  // Dereference the pointer to get the current character
    }
    printf("\n");
}

int count_words(char *buff,int len){
    char *buff_ptr = buff;  // Pointer to the first character of the string
    int word_count = 0;
    int in_word = 0;  // Flag to track whether we are inside a word
    while (*buff_ptr != '.' ){
        if((buff_ptr - buff) > len-1){//break to avoid overflow
            break;
        }
        if (in_word != 1){
            if (*buff_ptr != ' '){
                in_word = 1;
                word_count++;
            }
        }
        else{
            if(*buff_ptr == ' '){
                in_word = 0;            // now out of the word
                }
            }
        buff_ptr++;
        }
    return word_count;
}

int setup_buff(char *buff, char *user_str, int len){
    // Pointer to the current position in the buffer
    char *buff_ptr = buff;
    // Pointer to the current position in the input string
    const char *input_ptr = user_str;
    bool whitespace_last = true;
    bool characters_exist = false;
    int user_str_len = 0;

    // Loop through the input string until we reach the null terminator
    while (*input_ptr != '\0') {
        bool is_space = false;

        if ((*input_ptr == '\t') || (*input_ptr == '\n') || (*input_ptr == ' ')) {// Replace any non-space whitespace with a space
            is_space = true;
        }else {
            characters_exist = true; //check if we can reverse through buff removing trailing whitespace using a "non-space" as a "catch"
        }

        if (!whitespace_last){
            if (is_space) {
                whitespace_last = true;
                }else{
                    whitespace_last = false;
                }
            if ((buff_ptr - buff) < len - 1) {  // Ensure there's space for the null terminator
                *buff_ptr = *input_ptr;
                user_str_len++;
                buff_ptr++;  // Move the buffer pointer forward
            }else {
                return -1;  // Buffer is full return code for user input too large
            }
        } else{
            if (is_space) {
                ;
            }else{
                whitespace_last = false;
                if ((buff_ptr - buff) < len - 1) { 
                    *buff_ptr = *input_ptr;
                    user_str_len++;
                    buff_ptr++;  
                }else {
                    return -1; // Buffer is full return code for user input too large
                }
            }
        }
        input_ptr++;  // Move the input string pointer forward
    }
    buff_ptr--;
    if (characters_exist){
        while(*buff_ptr == ' '){
            *buff_ptr = '.';
            user_str_len--;
            buff_ptr--;
        }
    }
    buff_ptr++;
    while((buff_ptr - buff) < len - 1){
        *buff_ptr = '.' ;
        buff_ptr++;
    }
    // Null-terminate the buffer after copying
    *buff_ptr = '\0';


    //TODO: #4:  Implement the setup buff as per the directions
    return user_str_len; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}



//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){ // argc is arg count, argv is "argument vector" - its a pointer to an array of strings
                                  // futhermore, argv[0]= name of the program
                                  // argv[1] = is the first argument in this case a flag telling us which mode to execute
                                  // argv[2] = is the input.txt in this case our string to process
                                  // argv[3] = would be our output file if we had one

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /* 
    This is safe because it ensures that argv[1] even exists before it is accessed. If argc is less than two then it means that there
    are no arguments past the program's name so that argv[1] will not be access and there will be no error regarding invalid memory access.
    This also uses short circuiting which in C is a kind of process flow that only if the first conditon is false the program will try to
    evaluate the second condtion
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag by going to the flag and dereferncing the pointer (passing the first character "-") 
                                // to get to the actual option character in the second position "c"

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    the purpose of this if statement is to check if the user has provided enough arguments to successfully run the program for its intended 
    purpose - we have the name of the program, the mode, & lastly the string so we need at least those 3 arguments. If we dont then we call 
    usage to print out how we should be calling this utility and exit with code 1.
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = (char *)malloc(sizeof(char) * BUFFER_SZ);
    if (buff == NULL) { // if malloc fails it will return NULL
        printf("Memory allocation failed.\n");
        exit(99);  // malloc failure, exit the program because without this memory allocation we cannot conduct the program
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);  //setup_buff() is expected to populate the Buffer buff with the contents
    if (user_str_len < 0){                                     //from the string input_string.
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2); // if the user_str_len is less than zero that means the setup_buff failed. so we exit with code 2 signalling memory 
                 //allocation failure
    }

    print_buff(buff,BUFFER_SZ);// print produced buffer

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            word_print(buff, user_str_len);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
// The main reason we want to have the buffer, bufferlength, and user string length is to provide all our functions with this information 
// so that we can safely and accurately iterate through the user inputed string without segmentation faults or buffer overflows that would 
// cause the program to crash. We can prevent these errors and let the user know if their input is too large. This initiation of buffer
// size and computing user input size makes the code more reusuable and flexible.
//          