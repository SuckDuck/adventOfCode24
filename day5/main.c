#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct rule{
    int A,B;
} rule;

typedef struct group{
    int len;
    int* elements;
    int valid;
} group;


int split_str(char* s, int l, char d, char** output){
    int tokens=0;
    int last_token_index=0;
    for(int i=0; i<l; i++){
        char c=s[i];
        if(c==d){
            if(output != NULL){
                s[i]='\0';
                output[tokens] = s+last_token_index;
                last_token_index = i+1;
            }
            tokens++;
        }
    }

    if(tokens>0){
        if(output != NULL)
            output[tokens] = s+last_token_index;
        tokens++;
    }
    return tokens;
}

void print_tokens(char** tokens, int c){
    for(int i=0; i<c; i++){
        printf("[%s] ",tokens[i]);
    }
    printf("\n");
}

int read_input_file(char* path, char** output){
    struct stat st;
    stat(path,&st);
    int fd = open(path,O_RDONLY);
    
    *output = (char*) malloc(st.st_size);
    int bytes_read=0;
    while (bytes_read < st.st_size){
        bytes_read += read(fd,(void*)((*output)+bytes_read),st.st_size-bytes_read);
    }

    close(fd);
    return st.st_size;
}

int get_rules_c(char* input_string, int l){
    int count=0;
    for(int i=0; i<l; i++){
        char c=input_string[i];
        if(c=='|') count++;
    }

    return count;
}

int main(int argsc, char** args){
    if(argsc != 2) return 1;
    char* input_file_path = args[1];

    char* input_string;
    int input_string_l = read_input_file(input_file_path,&input_string);

    int tokens_c = split_str(input_string,input_string_l,'\n',NULL);
    char* tokens[tokens_c];
    split_str(input_string,input_string_l,'\n',(char**) &tokens);

    #pragma region // -------- PARSING RULES -------- //
    int rules_c = get_rules_c(input_string,input_string_l);
    rule rules[rules_c];
    for(int i=0; i<rules_c; i++){
        char* rule_token = tokens[i];
        int rule_len = strlen(rule_token);
        int sub_tokens_c = split_str(rule_token,rule_len,'|',NULL);
        char* sub_tokens[sub_tokens_c];
        split_str(rule_token,rule_len,'|',(char**) &sub_tokens);

        rules[i].A = atoi(sub_tokens[0]);
        rules[i].B = atoi(sub_tokens[1]);
    }
    #pragma endregion // ---------------------------- //

    #pragma region // ------- PARSING GROUPS -------- //
    int group_tokens_c = tokens_c - (rules_c+1);
    char** groups_tokens = tokens+rules_c+1;
    group groups[group_tokens_c];
    for(int i=0; i<group_tokens_c; i++){

        char* group_token = groups_tokens[i];
        int group_len = strlen(group_token);
        int sub_tokens_c = split_str(group_token,group_len,',',NULL);
        char* sub_tokens[sub_tokens_c];
        split_str(group_token,group_len,',',(char**) &sub_tokens);

        groups[i].valid = 1;
        groups[i].len = sub_tokens_c;
        groups[i].elements = (int*) malloc(sub_tokens_c * sizeof(int));
        for(int t=0; t<sub_tokens_c; t++){
            groups[i].elements[t] = atoi(sub_tokens[t]);
        }
    }
    #pragma endregion // ---------------------------- // 
    
    #pragma region // --- REAL PUZZLE STARTS HERE --- //
    // for each group
    for(int i=0; i<group_tokens_c; i++){
        repeat:;
        group* g=groups+i;

        // for each element in group
        for(int o=g->len-1;o>=0;o--){
            int e=g->elements[o];

            // for each rule
            for(int ri=0; ri<rules_c; ri++){
                rule* r=rules+ri;
            
                // if the rule concerns current element
                if(r->A == e){
                    
                    // for each element behind current element
                    for(int y=o-1; y>=0; y--){
                        int pe = g->elements[y];
                        if(pe == r->B){
                            g->valid = 0;
                            g->elements[y] = e;
                            g->elements[o] = pe;
                            goto repeat;
                        }
                        
                    }   
                }
            }
        }
    }
    #pragma endregion // ------------------------------ //

    int answer_0=0;
    int answer_1=0;
    for(int i=0; i<group_tokens_c; i++){
        group* g=groups+i;
        if(g->valid == 1) answer_0 += g->elements[(g->len-1)/2];   
        else              answer_1 += g->elements[(g->len-1)/2];
    }

    printf("FIRST PART ANSWER  : %i\n",answer_0);
    printf("SECOND PART ANSWER : %i\n",answer_1);


    for(int i=0; i<group_tokens_c; i++) free(groups[i].elements);
    free((void*) input_string);
    return 0;
}