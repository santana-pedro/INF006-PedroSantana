//Alunos: Pedro Santana e Victor Gabriel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct SLNode{
    double value;
    struct SLNode* next;
}SLNode;

typedef struct DLNode{
    int key;
    SLNode* sublist_head;
    int sublist_count;
    struct DLNode* next;
    struct DLNode* prev;
}DLNode;

int compare_int(const void* a, const void* b){
    return (*(int*)a - *(int*)b);
}

int compare_double_desc(const void* a, const void* b){
    double val1 = *(double*)a;
    double val2 = *(double*)b;
    if(val1 < val2) return 1;
    if(val1 > val2) return -1;
    return 0;
}

void add_to_sublist(DLNode* dlnode, double value){
    SLNode* new_node = (SLNode*)malloc(sizeof(SLNode));
    new_node->value = value;
    dlnode->sublist_count++;

    if(dlnode->sublist_head == NULL){
        dlnode->sublist_head = new_node;
        new_node->next = new_node;
    }else{
        SLNode* tail = dlnode->sublist_head;
        while(tail->next != dlnode->sublist_head){
            tail = tail->next;
        }
        tail->next = new_node;
        new_node->next = dlnode->sublist_head;
    }
}

void sort_sublist(DLNode* dlnode){
    if(dlnode->sublist_count <= 1) return;

    double* temp_array = (double*)malloc(dlnode->sublist_count * sizeof(double));
    SLNode* current = dlnode->sublist_head;
    for(int i = 0; i < dlnode->sublist_count; i++){
        temp_array[i] = current->value;
        current = current->next;
    }

    qsort(temp_array, dlnode->sublist_count, sizeof(double), compare_double_desc);

    current = dlnode->sublist_head;
    for(int i = 0; i < dlnode->sublist_count; i++){
        current->value = temp_array[i];
        current = current->next;
    }
    free(temp_array);
}

void free_structure(DLNode* head){
    DLNode* current_dl = head;
    while(current_dl != NULL){
        if(current_dl->sublist_head != NULL){
            SLNode* current_sl = current_dl->sublist_head;
            SLNode* sl_head = current_sl;
            current_sl->next = NULL;

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

int main(){
    FILE *fp_in = fopen("L1Q3.in", "r");
    FILE *fp_out = fopen("L1Q3.out", "w");
    char line[3001];

    if(fp_in == NULL || fp_out == NULL){
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while(fgets(line, sizeof(line), fp_in) != NULL){
        line[strcspn(line, "\n")] = 0;

        int le_keys[500];
        double li_values[500];
        int le_count = 0, li_count = 0;

        char* token = strtok(line, " ");
        while(token != NULL){
            if(strcmp(token, "LE") == 0){
                token = strtok(NULL, " ");
                while(token != NULL && strcmp(token, "LI") != 0){
                    le_keys[le_count++] = atoi(token);
                    token = strtok(NULL, " ");
                }
            }
            if(token != NULL && strcmp(token, "LI") == 0){
                token = strtok(NULL, " ");
                while(token != NULL){
                    li_values[li_count++] = atof(token);
                    token = strtok(NULL, " ");
                }
            }
        }

        qsort(le_keys, le_count, sizeof(int), compare_int);

        DLNode* head = NULL, *tail = NULL;
        for(int i = 0; i < le_count; i++){
            DLNode* new_node = (DLNode*)malloc(sizeof(DLNode));
            new_node->key = le_keys[i];
            new_node->sublist_head = NULL;
            new_node->sublist_count = 0;
            new_node->next = NULL;
            new_node->prev = tail;
            if(tail) tail->next = new_node;
            if(!head) head = new_node;
            tail = new_node;
        }

        for(int i = 0; i < li_count; i++){
            for(DLNode* current = head; current != NULL; current = current->next){
                if(fabs(li_values[i] - current->key) < 1){
                    add_to_sublist(current, li_values[i]);
                    break;
                }
            }
        }

        int has_empty_sublist = 0;
        for(DLNode* current = head; current != NULL; current = current->next){
            if(current->sublist_count == 0){
                has_empty_sublist = 1;
            }else{
                sort_sublist(current);
            }
        }

        if(has_empty_sublist) fprintf(fp_out, "[");

        for(DLNode* current = head; current != NULL; current = current->next){
            fprintf(fp_out, "%d(", current->key);
            if(current->sublist_head != NULL){
                SLNode* sl_current = current->sublist_head;
                for(int i = 0; i < current->sublist_count; i++){
                    fprintf(fp_out, "%.2f", sl_current->value);
                    if(i < current->sublist_count - 1){
                        fprintf(fp_out, "->");
                    }
                    sl_current = sl_current->next;
                }
            }
            fprintf(fp_out, ")");
            if(current->next != NULL){
                fprintf(fp_out, "->");
            }
        }

        if(has_empty_sublist) fprintf(fp_out, "]");
        fprintf(fp_out, "\n");

        free_structure(head);
    }

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}
