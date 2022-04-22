/*
Author:sayoriaaa
if value of a node is '~', then its tail-edge is epsilon
else is the value of its tail node
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct DFA{
    char value;
    struct DFA **tail;
};

struct DFA *newDFA(char value){
    struct DFA *ret=(struct DFA*)malloc(sizeof(struct DFA));
    ret->value=value;
    ret->tail=(struct DFA**)malloc(2*sizeof(struct DFA*));
    ret->tail[0]=NULL;
    ret->tail[1]=NULL;
    return ret;
}

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
    for(int i=0;i<strlen(s);i++){      
        switch(s[i]){
            case ')':
                while(stack_o[stack_o_len-1]!='(') postfix[postfix_len++]=stack_o[stack_o_len---1];
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
    }
    while(stack_o_len>0) postfix[postfix_len++]=stack_o[stack_o_len---1];
    postfix[postfix_len]='\0';
    return postfix;
}

int getListLoc(char* s, char t){
    for(int i=0;i<strlen(s);i++){
        if(s[i]==t) return i;
    }
}

void copy_int_line(int *a, int *b){
    for(int i=0;i<256;i++){
        b[i]=a[i];
    }
}

void delete_int_line(int *a){
    for(int i=0;i<256;i++) a[i]=-1;
}

void structDFA(char *s){//用一个邻接矩阵描述
    int state=0, char_set_len=0;
    char char_set[256];
    for(int i=0;i<strlen(s);i++){
        if(s[i]!='|'&&s[i]!='&'&&s[i]!='*') char_set[char_set_len++]=s[i];
        for(int i=0;i<char_set_len-1;i++){
            if(char_set[i]==char_set[char_set_len-1]) char_set_len--;
        }
    }
    char_set[char_set_len]='\0';
    printf("\nchar set:%s\n",char_set); 
//***********获得输入字符集合****************//
    int F[256][256];
    int start_state=0;
    for(int i=0;i<256;i++){
        for(int j=0;j<256;j++) F[i][j]=-1;
    }
    int top_state[3]={-1,-1,-1}, buttom_state[3]={-1,-1,-1};
    for(int i=0;i<strlen(s);i++){
        if(s[i]!='|'&&s[i]!='&'&&s[i]!='*'){
            int cor_pos=getListLoc(char_set,s[i]);
            F[state][cor_pos]=state+1;
            if(top_state[0]==-1){
                top_state[0]=state;
                top_state[1]=state+1;
            }
            else{
                buttom_state[0]=state;
                buttom_state[1]=state+1;
            }
            state+=2;
        }
        if(s[i]=='&'){
            //copy_int_line(F[buttom_state[0]],F[top_state[1]]);
            F[top_state[1]][char_set_len+2]=buttom_state[0];
            top_state[1]=buttom_state[1];
            buttom_state[0]=-1;
        }
        if(s[i]=='|'){
            start_state=state;
            F[state][char_set_len]=top_state[0];
            F[state][char_set_len+1]=buttom_state[0];
            top_state[0]=state;
            state++;
            F[buttom_state[1]][char_set_len]=state;
            F[top_state[1]][char_set_len]=state;
            top_state[1]=state;
            state++;
            buttom_state[0]=-1;
        }
        if(s[i]=='*'){
            if(buttom_state[0]==-1){
                start_state=state;
                F[state][char_set_len]=top_state[0];
                F[state][char_set_len+1]=state+1;
                F[top_state[1]][char_set_len]=top_state[0];
                F[top_state[1]][char_set_len+1]=state+1;
                top_state[0]=state;
                top_state[1]=state+1;
                state+=2;
            }
            else{
                F[state][char_set_len]=buttom_state[0];
                F[state][char_set_len+1]=state+1;
                F[buttom_state[1]][char_set_len]=buttom_state[0];
                F[buttom_state[1]][char_set_len+1]=state+1;
                buttom_state[0]=state;
                buttom_state[1]=state+1;
                state+=2;
            }
        }
    }
    printf("start state:%d\n",start_state);
    for(int i=0;i<state;i++){
        printf("\n state%d : ",i);
        for(int j=0;j<char_set_len+3;j++){
            if(F[i][j]==-1) printf("^ ");
            else printf("%d ",F[i][j]);
        }
    }

    FILE *f;
    f = fopen("nfa.dot","w+");
    fprintf(f,"digraph G{\n");
    for(int i=0;i<state-1;i++){
        fprintf(f,"%d[label=\"%d\"];\n",i,i);
    }
    fprintf(f,"%d[label=\"%d\",shape=\"doublecircle\"];\n",state-1,state-1);
    for(int i=0;i<state;i++){
        for(int j=0;j<char_set_len+3;j++){
            if(F[i][j]!=-1){
                if(j>=char_set_len) fprintf(f,"%d->%d[label=\"null\"];\n",i,F[i][j]); 
                else fprintf(f,"%d->%d[label=\"%c\"];\n",i,F[i][j],char_set[j]);
            }
        }
    }
    fprintf(f,"}");
    fclose(f);
    system("dot -Tpng nfa.dot -o nfa.png");

       
}



int main(int argc, char **argv){
    int control=0;
    char s[256];
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-para")==0) control=1;
    }
    scanf("%s",s);
    printf("%s", to_postfix(s));
    structDFA(to_postfix(s));
}


