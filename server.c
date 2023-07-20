#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <getopt.h>
// extra header made here
#include <errno.h>

// The maximum length of an HTTP message line
#define MAX_LINE 256
// The maximum length of an HTTP response message
#define MAX_LENGTH 16*1024
// The size of a chunk of HTTP response to read from the pipe
#define CHUNK_SIZE 1024


void printError(char *);
// temporarily change printServerError(char *str) to printServerError();
void printServerError();
void printResponse(char *str);

int debug = 0;


int main(int argc, char **argv) {
    char msg[MAX_LENGTH];
    int result;

    FILE *fp = stdin; // default is to read from stdin

    // Parse command line options.
    int opt;
    while((opt = getopt(argc, argv, "v")) != -1) {
        switch(opt) {
            case 'v':
                debug = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] [filename]\n", argv[0]);
                exit(1);
        }
    }
    if(optind < argc) {
        if((fp = fopen(argv[optind], "r")) == NULL) {
            perror("fopen");
            exit(1);
        }
    }


    // while loop start
    char line[MAX_LENGTH];
    char word[3];
    char *pointer;
    char *query_str;
    while (fgets(line, MAX_LENGTH + 1, fp) != NULL){
        strncpy(word, line, 3);
        // see if we reach the right line
        if (strncmp(word, "GET", 3) == 0){
            char c2[MAX_LENGTH];
            char c[MAX_LENGTH];
            //extract the word from information to give corresponding value to both parent and child process
            pointer = strchr(line, '/');
            if (strchr(line, '?') != NULL){
                query_str = strtok(pointer, "?");
                strcpy(c, ".");
                // get the string after / and before ?
                strcat(c, query_str);
                strcpy(c2, strchr(c, c[2]));
                query_str = strtok(NULL, " ");
                if (query_str != NULL){
                    // where should we use the result of setenv();
                    if (setenv("QUERY_STRING", query_str, 1) == -1){
                        perror("setenv");
                        exit(1);
                    }
                }
                else{
                    // where should we use the result of setenv();
                    setenv("QUERY_STRING", "", 1);
                }
            }
            else{
                query_str = strtok(pointer, " ");
                strcpy(c, ".");
                // get the string after / and before ?
                strcat(c, query_str);
                strcpy(c2, strchr(c, c[2]));
                setenv("QUERY_STRING", "", 1);
            }
            // set both parent and child process
            // !!!!!!!!!
            int fd[2];
            char data[CHUNK_SIZE];
            int status;
            int pp = pipe(fd);
            // check it later if we should print the message into terminal
            if (pp == -1){
                printf("fail to create pipe\n");
                return -1;
            }
            result = fork();
            if (result == -1){
                printf("failed to generate process");
                return -1;
            }

            // child process here
            if (result == 0){
                    close(fd[0]);
                    dup2(fd[1], 1);
                    if (execl(c, c2, NULL) == -1){
                        exit(errno);
                    }
                    close(fd[1]);
                exit(0);

            }
                // parent process here
            else{
                close(fd[1]);
                // (read(fd[0], data, MAX_LENGTH);
                int size;
                while ((size = read(fd[0], data, CHUNK_SIZE)) > 0){
                    strncat(msg, data, size);
                }
                strncat(msg, "\0", MAX_LENGTH);
                wait(&status);
                if(WIFEXITED(status)){
                    if(WEXITSTATUS(status) == 0){
                        // return file output
                        printResponse(msg);
                    }
                    else if(WEXITSTATUS(status) == 2){
                        printError(c);
                    }
                    else{
                        printServerError(c);
                    }


                }
                else if((WIFEXITED(status) == 1 && WEXITSTATUS(status) == 0 ) || WIFSIGNALED(status) == 1){
                    printServerError(c);
                }
                close(fd[0]);
                strcpy(word, "\000\000");
            }
        }
        else{
            // we reset word to null char;
            strcpy(word, "\000\000");

        }
    }
    

    // the end of file
    if(fp != stdin) {
        if(fclose(fp) == EOF) {
            perror("fclose");
            exit(1);
        }
    }
}


/* Print an http error page  
 * Arguments:
 *    - str is the path to the resource. It does not include the question mark
 * or the query string.
 */
void printError(char *str) {
    printf("HTTP/1.1 404 Not Found\r\n\r\n");

    printf("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
    printf("<html><head>\n");
    printf("<title>404 Not Found</title>\n");
    printf("</head><body>\n");
    printf("<h1>Not Found</h1>\n");
    printf("The requested resource %s was not found on this server.\n", str);
    printf("<hr>\n</body></html>\n");
}


/* Prints an HTTP 500 error page 
 */
void printServerError() {
    printf("HTTP/1.1 500 Internal Server Error\r\n\r\n");

    printf("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
    printf("<html><head>\n");
    printf("<title>500 Internal Server Error</title>\n");
    printf("</head><body>\n");
    printf("<h1>Internal Server Error</h1>\n");
    printf("The server encountered an internal error or\n");
    printf("misconfiguration and was unable to complete your request.<p>\n");
    printf("</body></html>\n");
}


/* Prints a successful response message
 * Arguments:
 *    - str is the output of the CGI program
 */
void printResponse(char *str) {
    printf("HTTP/1.1 200 OK\r\n\r\n");
    printf("%s", str);
}


