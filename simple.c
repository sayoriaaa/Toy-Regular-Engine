/*
Author:sayoriaaa
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int priority(char c){
    if(c=='*') return 3;
    if(c=='&') return 2;
    if(c=='|') return 1;
    if(c=='(') return 0;
}

char *to_postfix(char *s){
    char stack_o[256];
    char *postfix = (char*)malloc(256*sizeof(char));
    int stack_o_len=0, postfix_len=0;

    for(int i=0;i<strlen(s)-1;i++){
        if(s[i]!='('&&s[i]!='&'&&s[i]!='|'&&s[i+1]!=')'&&s[i+1]!='&'&&s[i+1]!='|'&&s[i+1]!='*'){
            s[strlen(s)+1]='\0';
            for(int j=strlen(s);j>i+1;j--) s[j]=s[j-1];
            s[i+1]='&';
        }
    }

    printf("%s\n",s);
    for(int i=0;i<strlen(s);i++){
        
        switch(s[i]){
            case ')':
                while(stack_o[stack_o_len-1]!='('){postfix[postfix_len++]=stack_o[stack_o_len---1];printf("called once\n");}
                stack_o_len--;
                break;
            case '(':
                stack_o[stack_o_len++]=s[i];
                break;
            case '*':
            case '&':
            case '|':
                while(stack_o_len>0&&priority(stack_o[stack_o_len-1])>=priority(s[i])) postfix[postfix_len++]=stack_o[stack_o_len---1];
                stack_o[stack_o_len++]=s[i]; 
                break;
            default:
                postfix[postfix_len++]=s[i];                      
        }
        postfix[postfix_len]='\0';
        stack_o[stack_o_len]='\0';
        printf("%d %c %s %s \n",i,s[i],postfix,stack_o);
    }
    while(stack_o_len>0) postfix[postfix_len++]=stack_o[stack_o_len---1];
    postfix[postfix_len]='\0';

    printf("%s\n",postfix);
    return postfix;
}

int main(int argc, char **argv){
    int control=0;
    char s[256];
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-para")==0) control=1;
    }
    scanf("%s",s);
    printf("%s", to_postfix(s));
}


