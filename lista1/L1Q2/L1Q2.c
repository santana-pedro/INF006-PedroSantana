// Nome da equipe: Pedro Santana
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Nó da lista ligada (pilha)
typedef struct Node {
    char* text;
    struct Node* next;
} Node;

// Funções básicas da pilha
void push(Node** top, const char* text) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->text = strdup(text);
    new_node->next = *top;
    *top = new_node;
}

char* pop(Node** top) {
    if (*top == NULL) return NULL;
    Node* temp = *top;
    *top = temp->next;
    char* popped_text = temp->text;
    free(temp);
    return popped_text;
}

// Função para inserir na pilha mantendo a ordem alfabética 
void sorted_push(Node** top, const char* text) {
    Node* temp_stack = NULL;
    // Move elementos maiores para uma pilha temporária
    while (*top != NULL && strcmp((*top)->text, text) > 0) {
        char* val = pop(top);
        push(&temp_stack, val);
        free(val);
    }
    push(top, text);
    // Move os elementos de volta para a pilha principal
    while (temp_stack != NULL) {
        char* val = pop(&temp_stack);
        push(top, val);
        free(val);
    }
}

void print_stack(Node* top) {
    if (top == NULL) return;
    
    // Imprime do topo para a base
    printf("%s", top->text);
    Node* current = top->next;
    while(current != NULL) {
        printf(" %s", current->text);
        current = current->next;
    }
}

void free_stack(Node** top) {
    while (*top != NULL) {
        char* val = pop(top);
        free(val);
    }
}

int main() {
    char line[3001];

    while (fgets(line, sizeof(line), stdin)) {
        // Remove a quebra de linha do final, se houver
        line[strcspn(line, "\n")] = 0;

        Node* stack = NULL;
        char* command = strtok(line, " ");

        while (command != NULL) {
            if (strncmp(command, "push-", 5) == 0) {
                sorted_push(&stack, command + 5);
            } else if (strstr(command, "x-pop") != NULL) {
                int n;
                if (sscanf(command, "%dx-pop", &n) == 1) {
                    for (int i = 0; i < n; i++) {
                        char* val = pop(&stack);
                        if (val) free(val);
                    }
                }
            }
            command = strtok(NULL, " ");
        }
        
        print_stack(stack);
        printf("\n");
        free_stack(&stack);
    }

    return 0;
}