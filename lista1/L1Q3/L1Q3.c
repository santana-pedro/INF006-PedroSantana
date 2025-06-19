// Nome da equipe: Pedro Santana
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Nó da sublista (LI - simplesmente ligada, circular)
typedef struct SLNode {
    double value;
    struct SLNode* next;
} SLNode;

// Nó da lista principal (LE - duplamente ligada) 
typedef struct DLNode {
    int key;
    SLNode* sublist_head;
    int sublist_count;
    struct DLNode* next;
    struct DLNode* prev;
} DLNode;

// Função de comparação para qsort (inteiros, crescente)
int compare_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Função de comparação para qsort (doubles, decrescente)
int compare_double_desc(const void* a, const void* b) {
    double val1 = *(double*)a;
    double val2 = *(double*)b;
    if (val1 < val2) return 1;
    if (val1 > val2) return -1;
    return 0;
}

// Insere um nó na sublista (de forma não ordenada)
void add_to_sublist(DLNode* dlnode, double value) {
    SLNode* new_node = (SLNode*)malloc(sizeof(SLNode));
    new_node->value = value;
    dlnode->sublist_count++;

    if (dlnode->sublist_head == NULL) {
        dlnode->sublist_head = new_node;
        new_node->next = new_node; // Circular
    } else {
        SLNode* tail = dlnode->sublist_head;
        while (tail->next != dlnode->sublist_head) {
            tail = tail->next;
        }
        tail->next = new_node;
        new_node->next = dlnode->sublist_head;
    }
}

// Ordena a sublista
void sort_sublist(DLNode* dlnode) {
    if (dlnode->sublist_count <= 1) return;

    // Extrai para um array
    double* temp_array = (double*)malloc(dlnode->sublist_count * sizeof(double));
    SLNode* current = dlnode->sublist_head;
    for (int i = 0; i < dlnode->sublist_count; i++) {
        temp_array[i] = current->value;
        current = current->next;
    }

    // Ordena o array em ordem decrescente 
    qsort(temp_array, dlnode->sublist_count, sizeof(double), compare_double_desc);
    
    // Reconstrói a sublista
    current = dlnode->sublist_head;
    for (int i = 0; i < dlnode->sublist_count; i++) {
        current->value = temp_array[i];
        current = current->next;
    }
    free(temp_array);
}

void free_structure(DLNode* head) {
    DLNode* current_dl = head;
    while (current_dl != NULL) {
        if (current_dl->sublist_head != NULL) {
            SLNode* current_sl = current_dl->sublist_head;
            SLNode* sl_head = current_sl;
            current_sl->next = NULL; // Quebra o ciclo para liberar
            
            while(sl_head != NULL){
                SLNode* temp = sl_head;
                sl_head = sl_head->next;
                free(temp);
            }
        }
        DLNode* temp_dl = current_dl;
        current_dl = current_dl->next;
        free(temp_dl);
    }
}


int main() {
    char line[3001];

    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;

        int le_keys[500];
        double li_values[500];
        int le_count = 0, li_count = 0;

        // Parse da linha
        char* token_ptr = line;
        char* token = strtok_r(token_ptr, " ", &token_ptr);
        while (token != NULL) {
            if (strcmp(token, "LE") == 0) {
                token = strtok_r(token_ptr, " ", &token_ptr);
                while (token != NULL && strcmp(token, "LI") != 0) {
                    le_keys[le_count++] = atoi(token);
                    token = strtok_r(token_ptr, " ", &token_ptr);
                }
            }
            if (token != NULL && strcmp(token, "LI") == 0) {
                 token = strtok_r(token_ptr, " ", &token_ptr);
                while (token != NULL) {
                    li_values[li_count++] = atof(token);
                    token = strtok_r(token_ptr, " ", &token_ptr);
                }
            }
        }
        
        // Ordena as chaves LE em ordem crescente 
        qsort(le_keys, le_count, sizeof(int), compare_int);

        // Cria a lista duplamente ligada
        DLNode* head = NULL, *tail = NULL;
        for (int i = 0; i < le_count; i++) {
            DLNode* new_node = (DLNode*)malloc(sizeof(DLNode));
            new_node->key = le_keys[i];
            new_node->sublist_head = NULL;
            new_node->sublist_count = 0;
            new_node->next = NULL;
            new_node->prev = tail;
            if (tail) tail->next = new_node;
            if (!head) head = new_node;
            tail = new_node;
        }
        
        // Associa os valores LI às chaves LE
        for (int i = 0; i < li_count; i++) {
            for (DLNode* current = head; current != NULL; current = current->next) {
                // A regra é que a diferença deve ser menor que 1 
                if (fabs(li_values[i] - current->key) < 1) {
                    add_to_sublist(current, li_values[i]);
                    break; // Associa ao primeiro que encontrar
                }
            }
        }

        // Ordena cada sublista e verifica se há alguma vazia
        int has_empty_sublist = 0;
        for (DLNode* current = head; current != NULL; current = current->next) {
            if (current->sublist_count == 0) {
                has_empty_sublist = 1;
            } else {
                sort_sublist(current);
            }
        }
        
        // Imprime o resultado final
        if (has_empty_sublist) printf("[");

        for (DLNode* current = head; current != NULL; current = current->next) {
            printf("%d(", current->key);
            if (current->sublist_head != NULL) {
                SLNode* sl_current = current->sublist_head;
                for (int i = 0; i < current->sublist_count; i++) {
                    printf("%.2f", sl_current->value);
                    if (i < current->sublist_count - 1) {
                        printf("->");
                    }
                    sl_current = sl_current->next;
                }
            }
            printf(")");
            if (current->next != NULL) {
                printf("->");
            }
        }

        if (has_empty_sublist) printf("]");
        printf("\n");

        free_structure(head);
    }

    return 0;
}