#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>

#define MAX_LINE 4096
#define MAX_WORDS MAX_LINE/2

void tokenize(char *line, char **words, int *nwords);
/* break line into words separated by whitespace, placing them in the 
   array words, and setting the count to nwords */

int main()
{
        char line[MAX_LINE], *words[MAX_WORDS], message[MAX_LINE];
        int stop=0,nwords=0;

        /* Check if user has entered exit */
        int quit = strcmp(line,"exit");

        /* This will hold the process status */
        int status; 

        while(1)
        {
                printf("MHD-Simple-CLI $ ");

                /* read a line of text here */
                fgets(line,MAX_WORDS,stdin);
		
                tokenize(line,words,&nwords);

                pid_t child;
                
                switch(child=fork())
                {
                        case -1:
                        printf("-1\n");
                        perror("fork");
                        exit(1);
                        break;
                        
                        case 0:
                        /* Check if user has entered exit */
                        if (nwords == 1 && strcmp(line, "exit") == 0)
                        {
                                exit(0);
                        }

                        if (execvp(line, words) == -1)
                        {
                                perror("=>");
                        }
                        else
                        {
                                execvp(line, words);
                        }

                        exit(0);

                        break;
                        
                        default:

                        if (nwords == 1 && strcmp(line,"exit") == 0)
                        {
                                exit(0);
                        }
                        else
                        {
                                /* printf("default I am the parent, my pid is %d," ,getpid());
                                printf("and my childs is %d\n",child);
 
                                printf("Parent:\n"); */

                                /* Check if user entered change directori command */
                                if (strcmp(line, "cd") == 0)
                                {
                                        chdir(words[1]);
                                        printf("%s\n", getcwd(line, sizeof(line)) );
                                }

                                /* wait for the child process to finish */
                                wait(&child);


                                /* Checking the child process status on the exit and showint its status*/
                                do
                                {
                                        /* check if child terminated normaly */
                                       if (WIFEXITED(status))
                                {
                                                printf("exited, status=%d\n", WEXITSTATUS(status));
                                }
                                        /* check if child process terminated by signal */
                                        else if (WIFSIGNALED(status))
                                {
                                                printf("killed by signal %d\n", WTERMSIG(status));
                                }
                                        /* check if child process terminated by delivering a signal */
                                        else if (WIFSTOPPED(status)) 
                                {
                                                printf("stopped by signal %d\n", WSTOPSIG(status));
                                }
                                        /* if the child process was resumed by delivery of SIGCONT */
                                        else if (WIFCONTINUED(status))
                                {
                                                printf("continued\n");
                                } 
                                }while (!WIFEXITED(status) && !WIFSIGNALED(status));
                                
                                
                                

                        }                        
                        break;
                }
        }
        return 0;
}

void tokenize(char *line, char **words, int *nwords)
{

        *nwords=1;

        for(words[0]=strtok(line," \t\n");
            (*nwords<MAX_WORDS)&&(words[*nwords]=strtok(NULL, " \t\n"));
            *nwords=*nwords+1
           ); 
        return;
}

