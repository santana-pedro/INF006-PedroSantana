//Alunos: Pedro Santana e Victor Gabriel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char* text;
    struct Node* next;
}Node;

void push(Node** top, const char* text){
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->text = strdup(text);
    new_node->next = *top;
    *top = new_node;
}

char* pop(Node** top){
    if(*top == NULL) return NULL;
    Node* temp = *top;
    *top = temp->next;
    char* popped_text = temp->text;
    free(temp);
    return popped_text;
}

void sorted_push(Node** top, const char* text){
    Node* temp_stack = NULL;
    while(*top != NULL && strcmp((*top)->text, text) > 0){
        char* val = pop(top);
        push(&temp_stack, val);
        free(val);
    }
    push(top, text);
    while(temp_stack != NULL){
        char* val = pop(&temp_stack);
        push(top, val);
        free(val);
    }
}

void print_stack(Node* top, FILE* fp_out){
    if(top == NULL) return;

    fprintf(fp_out, "%s", top->text);
    Node* current = top->next;
    while(current != NULL){
        fprintf(fp_out, " %s", current->text);
        current = current->next;
    }
}

void free_stack(Node** top){
    while (*top != NULL) {
        char* val = pop(top);
        free(val);
    }
}

int main(){
    FILE *fp_in = fopen("L1Q2.in", "r");
    FILE *fp_out = fopen("L1Q2.out", "w");
    char line[3001];

    if(fp_in == NULL || fp_out == NULL){
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while(fgets(line, sizeof(line), fp_in) != NULL){
        line[strcspn(line, "\n")] = 0; // remove \n

        Node* stack = NULL;
        char* command = strtok(line, " ");

        while(command != NULL){
            if(strncmp(command, "push-", 5) == 0){
                sorted_push(&stack, command + 5);
            }else if(strstr(command, "x-pop") != NULL){
                int n;
                if(sscanf(command, "%dx-pop", &n) == 1){
                    for (int i = 0; i < n; i++) {
                        char* val = pop(&stack);
                        if (val) free(val);
                    }
                }
            }
            command = strtok(NULL, " ");
        }
        print_stack(stack, fp_out);
        fprintf(fp_out, "\n");

        free_stack(&stack);
    }

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}