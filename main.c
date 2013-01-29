#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 15

char *lisp_read();
char *lisp_eval(char *);
char *removeWhitesapce(char *);
void  stringInsert(char *,char *,int position);


struct object
{
    int type;
    char *word;
    struct object *next;
};

int main()
{

    //Main loop
    int i=100;
    printf("//phatlisp\n" );
    while(i--)
        lisp_eval(lisp_read());



    return 0;
}

char *lisp_eval(char *stringEval)
{
    int i=0;
    int n=0;
    int counter=0;
    int oldBracket=0;
    int currentBracket=0;
    int closingBracket=0;
    char *bracketAtom;
    char *function;
    char *arg;

    printf("%s\n", stringEval);

    for (i=0; i<strlen(stringEval); ++i)
    {
        if(stringEval[i] == '(')
        {
            oldBracket=currentBracket;
            currentBracket=i;
        }
    }

    for (i=0; i<strlen(stringEval); ++i)
    {
        if(stringEval[i] == ')')
        {
            closingBracket=i;
            break;
        }
    }

    bracketAtom = malloc(sizeof(char)*(closingBracket-currentBracket+1));

    for(i=currentBracket; i<=closingBracket; ++i)
    {
        bracketAtom[i-currentBracket]=stringEval[i];
    }
    bracketAtom[closingBracket-currentBracket+1]='\0';


    //RECURSION STOP
    if(strcmp(bracketAtom,stringEval)==0)
       {
               function=malloc(15*sizeof(char)); //Realloc was giving me grief, also maximum function size is fairly small anyway

             for(i=1; bracketAtom[i]!=' '; ++i)
             {

                 function[i-1]=bracketAtom[i];
             }

             function[i-1]='\0';

             printf("\ni=%d\n", i);
             printf("You chose to use %s\n", function);

             arg=malloc(MAXLEN); //15 is max arg number

             i=i+2;

             for(counter=i; bracketAtom[i]!=' '; ++i)
             {
                 arg[i-counter]=bracketAtom[i];
             }

             arg[i-counter]='\0';

             printf("\narg1 = %s", arg);


             counter=i;

             return 0;
       }



    printf("%s\n", bracketAtom);

    lisp_eval(bracketAtom); // OMG RECURSION AHHHHH


    //lisp_eval(bracketAtom);
    free(bracketAtom);
    free(stringEval);


    printf("\nold=%d new=%d\n and closing bracket is at %d\n", oldBracket, currentBracket, closingBracket);
}


char *lisp_read()
{
    char c, *stringBuffer;
    int bracket=0;
    int i=0;

    stringBuffer = malloc(sizeof(char));

    printf("> ");
    while((c=getchar()) != EOF)
    {

        if((stringBuffer = realloc(stringBuffer, i+2 ))==NULL)
            printf("Realloc Failed");

        stringBuffer[i]=c;


        if(c=='(')
            ++bracket;
        if(c==')')
            --bracket;
        if(bracket<0)
        {
            printf("Error, brackets did not balance\n");
            break;
        }

        if( c == '\n' && bracket == 0 )
        {
            stringBuffer[++i]='\0';
            break;
        }

        ++i;
    }

    if(bracket<0)
       return NULL;


    /* Doing it twice just to make sure some really rare bugs don't happen */
    stringBuffer = removeWhitesapce(stringBuffer);
    stringBuffer = removeWhitesapce(stringBuffer);

    return stringBuffer;
}

char *removeWhitesapce(char *stringBuffer)
{
    int i;
    int j;
    int length=strlen(stringBuffer);



    for(i=0; i<length; ++i)
    {

        if(stringBuffer[0]==' ')
        {
            for(j=0; j<length; ++j)
                stringBuffer[j]=stringBuffer[1+j];

            i=0;
        }


        if(stringBuffer[i]=='\n')
        {
            for(j=i; j<length; ++j)
                stringBuffer[j]=stringBuffer[1+j];

            i=0;
        }



        if(stringBuffer[i]==' ')
            if(stringBuffer[i+1]==')')
            {
                for(j=i; j<length; ++j)
                {
                    stringBuffer[j]=stringBuffer[j+1];
                }

                i=0;
            }

        if(stringBuffer[i]==' ')
            if(stringBuffer[i+1]==' ')
            {
                for(j=i+1; j<length; ++j)
                {
                    stringBuffer[j]=stringBuffer[j+1];
                }

                i=0;
            }

        if(stringBuffer[i]=='(')
            if(stringBuffer[i+1]==' ')
            {
                for(j=i+1; j<length; ++j)
                {
                    stringBuffer[j]=stringBuffer[j+1];
                }

                i=0;
            }





    }

    return stringBuffer;
}

void  stringInsert(char *target,char *bullet,int position)
{
    int targetLength = strlen(target);
    int targetBullet = strlen(bullet);
    int newLength    = targetLength + targetBullet;

    target = realloc(target, sizeof(char)*newLength);


    return;
}
