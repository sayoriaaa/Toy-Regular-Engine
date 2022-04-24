/*
Author:sayoriaaa
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LEN 256//use for stack

int NFA_STATES=0;
int NFA_START_STATE=0;
int CHAR_SET_LEN=0;
char char_set[MAX_LEN];

int priority(char c){
    if(c=='*') return 3;
    if(c=='&') return 2;
    if(c=='|') return 1;
    if(c=='(') return 0;
}

char *to_postfix(char *s){
    char stack_o[MAX_LEN];
    char *postfix = (char*)malloc(MAX_LEN*sizeof(char));
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
            case '*':
            case '&':
            case '|':
                while(stack_o_len>0&&priority(stack_o[stack_o_len-1])>=priority(s[i])) postfix[postfix_len++]=stack_o[stack_o_len---1];
            case '(':
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
    return -1;
}

int** structNFA(char *s){//用一个邻接矩阵描述
    int state=0, char_set_len=0;
    for(int i=0;i<strlen(s);i++){
        if(s[i]!='|'&&s[i]!='&'&&s[i]!='*') char_set[char_set_len++]=s[i];
        for(int i=0;i<char_set_len-1;i++){
            if(char_set[i]==char_set[char_set_len-1]) char_set_len--;
        }
    }
    char_set[char_set_len]='\0';
    CHAR_SET_LEN=char_set_len;//save in global field 
//***********获得输入字符集合****************//
    int **F=(int**)malloc(sizeof(int*)*MAX_LEN);
    for(int i=0;i<MAX_LEN;i++){
        F[i]=(int*)malloc(sizeof(int)*(char_set_len+3));
        memset(F[i], -1, sizeof(int)*(char_set_len+3));
    }//initialize the transform function

    int state_stack[MAX_LEN][3],state_stack_len=0;
    for(int i=0;i<strlen(s);i++){
        if(s[i]!='|'&&s[i]!='&'&&s[i]!='*'){
            int cor_pos=getListLoc(char_set,s[i]);
            F[state][cor_pos]=state+1;
            state_stack[state_stack_len][0]=state;
            state_stack[state_stack_len][1]=state+1;//push to stack
            state_stack_len++;//manually maintain stack
            state+=2;
        }
        if(s[i]=='&'){
            //copy_int_line(F[buttom_state[0]],F[top_state[1]]);
            F[state_stack[state_stack_len-2][1]][char_set_len+2]=state_stack[state_stack_len-1][0];//link fronter's tail to follower's tail
            state_stack[state_stack_len-2][1]=state_stack[state_stack_len-1][1];
            state_stack_len--;
        }
        if(s[i]=='|'){
            F[state][char_set_len]=state_stack[state_stack_len-2][0];
            F[state][char_set_len+1]=state_stack[state_stack_len-1][0];//give arrow to the two opoed ones
            F[state_stack[state_stack_len-2][1]][char_set_len]=state+1;
            F[state_stack[state_stack_len-1][1]][char_set_len]=state+1;

            state_stack[state_stack_len-2][0]=state;
            state_stack[state_stack_len-2][1]=state+1;
            state+=2;
            state_stack_len--;
        }
        if(s[i]=='*'){
            F[state][char_set_len]=state_stack[state_stack_len-1][0];
            F[state][char_set_len+1]=state+1;
            F[state_stack[state_stack_len-1][1]][char_set_len]=state_stack[state_stack_len-1][0];
            F[state_stack[state_stack_len-1][1]][char_set_len+1]=state+1;
            state_stack[state_stack_len-1][0]=state;
            state_stack[state_stack_len-1][1]=state+1;
            state+=2;
        }
    }
    
    int state_visit[MAX_LEN]={0}, start_state=0;//to identify the start node
    for(int i=0;i<state;i++){
        printf("\nstate%d : ",i);
        for(int j=0;j<char_set_len+3;j++){
            if(F[i][j]==-1) printf("^ ");
            else{printf("%d ",F[i][j]);state_visit[F[i][j]]=1;}          
        }
    }
    for(int i=0;i<state;i++){
        if(state_visit[i]==0){start_state=i;break;}}
    printf("\n\nstart state:%d\n",start_state);
    NFA_STATES=state;//save state to global field
    NFA_START_STATE=start_state;

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
    fprintf(f,"start[label=\"start\"];\nstart->%d;\n",start_state);
    fprintf(f,"}");
    fclose(f);
    system("dot -Tpng nfa.dot -o nfa.png");
    return F;
}

void NFA_move(int **F, int state, int* oldStates, int* oldStates_len, int* alreadyOn){//handle epsilon for NFA_match
    for(int i=0;i<3;i++){
        int euqal_state=F[state][CHAR_SET_LEN+i];
        if(euqal_state!=-1&&alreadyOn[euqal_state]==0){
            alreadyOn[euqal_state]=1;
            oldStates[*oldStates_len]=euqal_state;
            (*oldStates_len)++;
            NFA_move(F, euqal_state, oldStates, oldStates_len, alreadyOn);
        }
    }
}

int NFA_match(int** F, char *s){//test note for showing the legal state for each step of reading the input string
    int oldStates[MAX_LEN]={0}, newStates[MAX_LEN]={0}, alreadyOn[MAX_LEN]={0};
    int oldStates_len=1, newStates_len=0;
    oldStates[0]=NFA_START_STATE;
    alreadyOn[NFA_START_STATE]=1;

    NFA_move(F, NFA_START_STATE, oldStates, &oldStates_len, alreadyOn);
    
    //for(int i=0;i<oldStates_len;i++) printf("%d ",oldStates[i]);
    //printf("\n");
    
    for(int i=0;i<strlen(s);i++){
        int tmp=getListLoc(char_set, s[i]);
        if(tmp==-1){printf("wrong alphabet set\n"); return 0;}
        while(oldStates_len>0){//pop stack operation
            int t=F[oldStates[oldStates_len-1]][tmp];
            if(t!=-1&&alreadyOn[t]==0) NFA_move(F, t, newStates, &newStates_len, alreadyOn);
            oldStates_len--;
        }
        while(newStates_len>0){
            oldStates[oldStates_len++]=newStates[newStates_len-1];
            alreadyOn[newStates[newStates_len-1]]=0;
            newStates_len--;
        }
        //for(int i=0;i<oldStates_len;i++) printf("%d ",oldStates[i]);
        //printf("\n");            
    }
    for(int i=0;i<oldStates_len;i++){
        if(oldStates[i]==NFA_STATES-1) return 1;
    }
    return 0;
}

int IsSetSame(int* a, int* b){
    for(int i=0;i<NFA_STATES;i++){
        if(a[i]!=b[i]) return 0;
    }
    return 1;
}

int* epsilon_closure_T(int** NFA, int *T, int T_len){
    int* alreadyOn=(int*)malloc(sizeof(int)*NFA_STATES);
    memset(alreadyOn, 0, sizeof(int)*(NFA_STATES));
    int epsilon_extend[MAX_LEN]={0}, epsilon_extend_len=T_len;

    for(int i=0; i<T_len; i++){
        epsilon_extend[i]=T[i];
        alreadyOn[T[i]]=1;
    }//防止对原数组修改,拷贝成epsilon-extend

    for(int i=0;i<T_len;i++){
        NFA_move(NFA, T[i], epsilon_extend, &epsilon_extend_len, alreadyOn);
    }
    return alreadyOn;
}


void NFA_to_DFA(int** NFA){
    int DFA_states_len=1, S0[1]={NFA_START_STATE};
    int **F=(int**)malloc(sizeof(int*)*MAX_LEN);
    for(int i=0;i<MAX_LEN;i++){
        F[i]=(int*)malloc(sizeof(int)*(CHAR_SET_LEN));
        memset(F[i], -1, sizeof(int)*(CHAR_SET_LEN));
    }//initialize the transform function

    int **code_set_fun=(int**)malloc(sizeof(int*)*MAX_LEN);//DFA状态编号到具体NFA状态集合的映射
    int taged[MAX_LEN]={0};

    code_set_fun[0]=epsilon_closure_T(NFA, S0, 1);
    for(int i=0; i<DFA_states_len; i++){
        if(taged[i]==0){
            taged[i]=1;
            for(int a=0; a<CHAR_SET_LEN; a++){
                int U[MAX_LEN]={0}, U_len=0;
                for(int j=0; j<NFA_STATES; j++){
                    if(code_set_fun[i][j]==1&&NFA[j][a]!=-1) U[U_len++]=NFA[j][a];
                }
                int* wait_to_code=epsilon_closure_T(NFA, U, U_len);//等待将此集合编号
                int U_code=0;
                for(; U_code<DFA_states_len; U_code++){
                    if(IsSetSame(wait_to_code, code_set_fun[U_code])) break;//查看此集合是否已经有记录
                }
                if(U_code==DFA_states_len){
                    DFA_states_len++;
                    code_set_fun[U_code]=wait_to_code;
                }
                F[i][a]=U_code;
            }
        }
    }

    for(int i=0;i<DFA_states_len;i++){
        printf("\nstate%d : ",i);
        for(int j=0;j<CHAR_SET_LEN;j++){
            if(F[i][j]==-1) printf("^ ");
            else{printf("%d ",F[i][j]);}          
        }
    }

    FILE *f;
    f = fopen("dfa.dot","w+");
    fprintf(f,"digraph G{\n");
    for(int i=0;i<DFA_states_len;i++){
        if(code_set_fun[i][NFA_STATES-1]==1) fprintf(f,"%d[label=\"%d\",shape=\"doublecircle\"];\n",i,i);
        fprintf(f,"%d[label=\"%d\"];\n",i,i);
    }
    for(int i=0;i<DFA_states_len;i++){
        for(int j=0;j<CHAR_SET_LEN;j++){
            if(F[i][j]!=-1){
                fprintf(f,"%d->%d[label=\"%c\"];\n",i,F[i][j],char_set[j]);
            }
        }
    }
    fprintf(f,"start[label=\"start\"];\nstart->0;\n");
    fprintf(f,"}");
    fclose(f);
    system("dot -Tpng dfa.dot -o dfa.png");
}



int main(int argc, char **argv){
    int control=0, **F;
    char s[MAX_LEN];
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-para")==0) control=1;
    }
    scanf("%s",s);
    printf("%s", to_postfix(s));
    F=structNFA(to_postfix(s));
    NFA_to_DFA(F);
    /*
    while(1){
        scanf("%s",s);
        printf("%d\n",NFA_match(F,s));    
    }*/
}


