#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_string 0
#define T_num 1
#define T_bool 2
#define T_char 3
#define T_procedure 4
#define T_pair 5

struct procedure
{
    char *opName;
    struct lisp_object* (*opPointer)(struct lisp_object *);
};

struct cons *new_cons(struct lisp_object, struct lisp_object *);

struct lisp_object
{
    int objectType;
    char *stringD;
    double numD;
    int boolD;
    char charD;
    struct procedure *proc;
    struct cons *pairD;
};
struct cons{
    struct lisp_object current;
    struct lisp_object * next;
};

struct lisp_object lisp_read();
struct lisp_object lisp_eval(struct lisp_object);

struct lisp_object * opArray = NULL;

struct lisp_object  new_num_object(double);
struct lisp_object  new_string_object(char *);
struct lisp_object  new_bool_object(int);
struct lisp_object  new_char_object(char);
struct lisp_object  new_proc_object(char *,struct lisp_object* (struct lisp_object *));
struct lisp_object  new_pair_object(struct lisp_object ,struct lisp_object*);

struct lisp_object  add      (struct lisp_object a,struct lisp_object b){return new_num_object(a.numD + b.numD);}
struct lisp_object  subtract (struct lisp_object a,struct lisp_object b){return new_num_object(a.numD - b.numD);}
struct lisp_object  multiply (struct lisp_object a,struct lisp_object b){return new_num_object(a.numD * b.numD);}
struct lisp_object  divide   (struct lisp_object a,struct lisp_object b){return new_num_object(a.numD / b.numD);}

struct lisp_object car (struct lisp_object);
struct lisp_object cdr (struct lisp_object);

char *removeWhitesapce(char *);  // I am a twat. I know.

struct lisp_object * splitString(char *,int);
struct lisp_object listeriseString(char *);

int * bracketMatcher(char *);
int * bracketMap(char *);

void lisp_write(struct lisp_object);

int main()
{
    int i = 0;
    /*
    //initialise opArray with opNames and opPointers
    int i = 0;
    //initialise + operator
    ++i;
    opArray = realloc(opArray, i * sizeof(struct lisp_object));
    opArray[i-1].objectType = T_procedure;
    opArray[i-1].proc.opName = malloc(sizeof("+"));
    strcpy(opArray[i-1].opName, "+");
    opArray[i-1].opPointer = &add;
    //initalise - operator
    ++i;
    opArray = realloc(opArray, i * sizeof(struct op));
    opArray[i-1].opName = malloc(sizeof("-"));
    strcpy(opArray[i-1].opName, "-");
    opArray[i-1].opPointer = &subtract;
    //initalise * operator
    ++i;
    opArray = realloc(opArray, i * sizeof(struct op));
    opArray[i-1].opName = malloc(sizeof("*"));
    strcpy(opArray[i-1].opName, "*");
    opArray[i-1].opPointer = &multiply;
        //initalise - operator
    ++i;
    opArray = realloc(opArray, i * sizeof(struct op));
    opArray[i-1].opName = malloc(sizeof("/"));
    strcpy(opArray[i-1].opName, "/");
    opArray[i-1].opPointer = &divide;
    */
    //printf("++Because this is currently all operations have to be done inside a list++\n");
    printf("++++++++++++++++++++Various other things are also a bit shit+++++++++++++++++++\n\n");
    do
    {
        printf(">");
        //struct lisp_object m = lisp_eval(lisp_read());
        lisp_write((lisp_eval(lisp_read())));
    }
    while(1);
}

struct lisp_object lisp_eval(struct lisp_object objEval)
{
    switch(objEval.objectType){
        case T_string :
            //printf("Fuck\n");
            return objEval;
        case T_num :
            return objEval;
        case T_bool :
            return objEval;
        case T_char :
            return objEval;
        case T_procedure:
            return objEval;
        case T_pair:
        printf("Ok....");
           // if(car(objEval).objectType == T_procedure){
            //}else{
            return objEval;
            //}
    }
        //return (*(objEval[0].proc)).opPointer(lisp_eval(&objEval[1])) ;
}

struct lisp_object car(struct lisp_object a){
    if(a.objectType ==T_pair){
        return (*a.pairD).current;
    }else{
        return a;
    }
}

struct lisp_object cdr(struct lisp_object a){
    if(a.objectType ==T_pair){
        if(!(*a.pairD).next){
            printf("Got to NULL\n");
            return new_string_object("NULL");

        }else{
           printf("sup\n");
           return *(*a.pairD).next;
        }
    }else{
        return a;
    }
}
struct lisp_object lisp_read(){
    char c, *stringBuffer;
    int bracket=0;
    int i=0;

    stringBuffer = malloc(1);

    while((c =getchar()) != EOF)
    {
        stringBuffer = realloc(stringBuffer, i+ 1);

        if(c=='(')
        {
            ++bracket;
        }
        if(c==')')
        {
            --bracket;
        }

        if( c == '\n' && bracket == 0 )
        {
            stringBuffer[++i]='\0';
            break;
        }
        stringBuffer[i]=c;
        ++i;
    }

    /* Doing it twice just to make sure some really rare bugs don't happen (also for the shear goddamn hell of it)*/
    stringBuffer = removeWhitesapce(stringBuffer);
    stringBuffer = removeWhitesapce(stringBuffer);

    return listeriseString(stringBuffer);
}
struct lisp_object listeriseString(char * current){

    int * bM = malloc(sizeof(int) * strlen(current));
    bM = bracketMatcher(current);
    int *unProtSpaceLoc = NULL;
    int i,d = 0;
    struct lisp_object stringListRoot;
    struct lisp_object stringListCond;
    struct lisp_object * string = malloc(2*sizeof(struct lisp_object));

    if(current[0] =='(' && bM[0] == (strlen(current) - 1)){  //Hopefully this is equiv to saying whole statement is in a bracket!
       current[strlen(current) - 1] = '\0'; //remove last bracket
       ++current; //move pointer to get rid of first bracket
    }


    bM = bracketMap(current);

    for(i=0;i<strlen(current);++i){ //loop to locate the unprotected spaces (parenthesis protect spaces)
        if(bM[i]==0 && current[i] == ' '){
            ++d;
            unProtSpaceLoc = realloc(unProtSpaceLoc,d*sizeof(int)); //make space for new location
            if(d != 1){
                unProtSpaceLoc[d- 1] = i;
            }else{

                unProtSpaceLoc[d- 1] = i;
            }

        }
    }

    for(i= d-1;i>0;--i){
        --unProtSpaceLoc[i];
    }

    printf("%d\n",d);
    string = splitString(current,unProtSpaceLoc[d-1]);
    stringListRoot = new_pair_object(string[1],NULL); //make a list with one element consed onto null
    (*stringListRoot.pairD).next = malloc(sizeof(struct lisp_object));
    (*(*stringListRoot.pairD).next) = stringListCond;

    for(i= d-2;i>=0;--i){

        string = splitString(string[0].stringD,unProtSpaceLoc[i]);
        (*stringListCond.pairD).next = malloc(sizeof(struct lisp_object));
        (*(*stringListCond.pairD).next) = new_pair_object(string[1],NULL); //wrong wrong wrong weird and wrong!!! (Ok apart from that)
        stringListCond = (*(*stringListCond.pairD).next);

    }
    printf("finished listerise\n");
    (*stringListCond.pairD).next = malloc(sizeof(struct lisp_object));
    (*(*stringListCond.pairD).next) = new_pair_object(string[0],NULL);

   return stringListRoot;
}

struct lisp_object * splitString(char * inp, int n){ //returns input after the n'th char and replaces the n'th char in inp with '\0'
    char * out1;
    char * out2;
    struct lisp_object * out = malloc(2*sizeof(struct lisp_object));
   // n = 5;
    if(n<= strlen(inp)){
        out1 = malloc(strlen(inp) - n);
        strncpy(out1, inp,n +1);
        out1[n+1] = '\0';

    }

    out2 = inp + n +1;
    out[0] = new_string_object(out1);
    out[1] = new_string_object(out2);


    return out;
}

int * bracketMatcher(char * inp){
   int * bm = malloc(sizeof(int) *strlen(inp) );
   bm = bracketMap(inp);
   int * matchMap = malloc(sizeof(int) *strlen(inp) );
   int i = 0,j = 0;
   for(i = 0;i<strlen(inp);++i){
       j = 1;
       if(inp[i] == '('){
          while(bm[i + j] != bm[i]){
          ++j;
          }
          matchMap[i] = i+j;
        }else{
            matchMap[i] = -1;
        }


    }

   return matchMap;
}

int * bracketMap(char * inp){
int i = 0;
int *bracketMap = malloc(sizeof(int) *strlen(inp) );
int depth = 0;
    for(i = 0;i<strlen(inp) ;++i){
        if(inp[i] == '('){
           bracketMap[i] = depth;
           ++depth;
        }else if(inp[i] == ')'){
           --depth;
            bracketMap[i] = depth;
        }else{
            bracketMap[i] = depth;
        }


    }

    return bracketMap;
}


char *removeWhitesapce(char *stringBuffer){
    int i;
    int j;
    int length=strlen(stringBuffer)+1;
if(stringBuffer[0]==' ')
        {
            for(j=0; j<length; ++j)
                stringBuffer[j]=stringBuffer[1+j];

            i=0;
        }
    for(i=0; i<length; ++i)
    {



        if(stringBuffer[i]=='\n')
        {
            stringBuffer[i]=' ';
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

void lisp_write(struct lisp_object objOut){


 switch(objOut.objectType){
        case T_string :
            printf("%s\n", objOut.stringD);
            break;
        case T_num :
            printf("%lf\n", objOut.numD);
            break;
        case T_bool :
            printf("%d\n", objOut.boolD);
            break;
        case T_char :
            printf("%c\n", objOut.charD);
            break;
        case T_procedure :
            printf("%s\n", (*objOut.proc).opName );
            break;
        case T_pair :
            lisp_write(car(objOut));
            lisp_write(cdr(objOut));
            break;
       }
}


struct lisp_object new_num_object(double d){
    struct lisp_object r;
    r.objectType = T_num;
    r.boolD = 0;
    r.charD = ' ';
    r.numD = d;
    r.proc = NULL;
    r.stringD = NULL;
    r.pairD = NULL;
    return r;
}
struct lisp_object new_string_object(char *d){
    struct lisp_object r;
    r.objectType = T_string;
    r.boolD = 0;
    r.charD = ' ';
    r.numD = 0;
    r.proc = NULL;
    r.stringD = malloc(strlen(d) + 1);

    strcpy(r.stringD,d);

    r.pairD = NULL;

    return r;
}

struct lisp_object new_bool_object(int d){
    struct lisp_object r;
    r.objectType = T_bool;
    r.boolD = d;
    r.charD = ' ';
    r.numD = 0;
    r.proc = NULL;
    r.stringD = NULL;
    r.pairD = NULL;

    return r;
}

struct lisp_object new_char_object(char d){
    struct lisp_object r;
    r.objectType = T_char;
    r.boolD = 0;
    r.charD = d;
    r.numD = 0;
    r.proc = NULL;
    r.stringD = NULL;
    r.pairD = NULL;

    return r;
}

struct lisp_object new_pair_object(struct lisp_object a,struct lisp_object *b){
    struct lisp_object r;
    r.objectType = T_pair;
    r.boolD = 0;
    r.charD = ' ';
    r.numD = 0;
    r.proc = NULL;
    r.stringD = NULL;
    //printf("unconsed");
    r.pairD = new_cons(a,b);
    //printf("Consed");
    return r;
}

struct lisp_object new_proc_object(char * n,struct lisp_object* (*d)(struct lisp_object *)){
    struct lisp_object r;
    r.objectType = T_procedure;
    r.boolD = 0;
    r.charD = ' ';
    r.numD = 0;
    r.proc = malloc(sizeof(struct procedure));
    (*(r.proc)).opName = malloc(sizeof(n));
    strcpy((*(r.proc)).opName,n);
    (*(r.proc)).opPointer = d;
    r.stringD = NULL;
    r.pairD = NULL;
    return r;
}

struct cons *new_cons(struct lisp_object a, struct lisp_object *b){
    struct cons r;
    r.current = a;
    r.next = b;
    return &r;
}
