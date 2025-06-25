//Alunos: Pedro Santana e Victor Gabriel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_CAPACITY 10
#define MAX_LINE_LENGTH 3001

typedef struct{
    int* numbers;
    int count;
    int capacity;
    long long sum;
}IntList;

typedef struct{
    long long sum;
    IntList* list;
}SumMapEntry;

IntList* create_list(){
    IntList* list = (IntList*)malloc(sizeof(IntList));
    if(!list){
        perror("Erro ao alocar IntList");
        exit(EXIT_FAILURE);
    }
    list->capacity = INITIAL_CAPACITY;
    list->numbers = (int*)malloc(list->capacity * sizeof(int));
    if(!list->numbers){
        perror("Erro ao alocar numbers para IntList");
        free(list);
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->sum = 0;
    return list;
}

void add_to_list(IntList* list, int number){
    if(list->count == list->capacity){
        list->capacity *= 2;
        list->numbers = (int*)realloc(list->numbers, list->capacity * sizeof(int));
        if(!list->numbers){
            perror("Erro ao realocar numbers para IntList");
            exit(EXIT_FAILURE);
        }
    }
    list->numbers[list->count++] = number;
    list->sum += number;
}

void free_list(IntList* list){
    if(list){
        free(list->numbers);
        free(list);
    }
}

int compare_integers(const void* a, const void* b){
    return (*(int*)a - *(int*)b);
}

int compare_lists_by_sum(const void* a, const void* b){
    IntList* listA = *(IntList**)a;
    IntList* listB = *(IntList**)b;
    if (listB->sum > listA->sum) return 1;
    if (listB->sum < listA->sum) return -1;
    return 0;
}

int main(){
    FILE *fp_in = fopen("L1Q1.in", "r");
    FILE *fp_out = fopen("L1Q1.out", "w");

    if(fp_in == NULL || fp_out == NULL){
        perror("Erro ao abrir os arquivos");
        return EXIT_FAILURE;
    }

    char line_buffer[MAX_LINE_LENGTH];

    while(fgets(line_buffer, sizeof(line_buffer), fp_in) != NULL){
        line_buffer[strcspn(line_buffer, "\n")] = 0;

        char *trim_start = line_buffer;
        while(*trim_start == ' ') trim_start++;
        char *trim_end = trim_start + strlen(trim_start) - 1;
        while(trim_end >= trim_start && *trim_end == ' ') trim_end--;
        *(trim_end + 1) = '\0';

        IntList** current_line_lists = (IntList**)malloc(INITIAL_CAPACITY * sizeof(IntList*));
        if(!current_line_lists){
            perror("Erro ao alocar current_line_lists");
            exit(EXIT_FAILURE);
        }
        int current_line_list_count = 0;
        int current_line_list_capacity = INITIAL_CAPACITY;

        SumMapEntry* temp_sum_map = (SumMapEntry*)malloc(INITIAL_CAPACITY * sizeof(SumMapEntry));
        if(!temp_sum_map){
            perror("Erro ao alocar temp_sum_map");
            free(current_line_lists);
            exit(EXIT_FAILURE);
        }
        int temp_map_count = 0;
        int temp_map_capacity = INITIAL_CAPACITY;

        char *token;
        char *line_copy = strdup(trim_start);
        if(!line_copy){
            perror("Erro ao duplicar linha");
            free(current_line_lists);
            free(temp_sum_map);
            exit(EXIT_FAILURE);
        }

        token = strtok(line_copy, " ");
        while(token != NULL){
            if(strcmp(token, "start") == 0){
                if(current_line_list_count == current_line_list_capacity){
                    current_line_list_capacity *= 2;
                    current_line_lists = (IntList**)realloc(current_line_lists, current_line_list_capacity * sizeof(IntList*));
                    if(!current_line_lists){
                        perror("Erro ao realocar current_line_lists");
                        free(line_copy);
                        exit(EXIT_FAILURE);
                    }
                }
                current_line_lists[current_line_list_count] = create_list();
                current_line_list_count++;
            }else{
                if(current_line_list_count > 0){
                    add_to_list(current_line_lists[current_line_list_count - 1], atoi(token));
                }
            }
            token = strtok(NULL, " ");
        }
        free(line_copy);

        for(int i = 0; i < current_line_list_count; i++){
            qsort(current_line_lists[i]->numbers, current_line_lists[i]->count, sizeof(int), compare_integers);

            int found_idx = -1;
            for(int j = 0; j < temp_map_count; j++){
                if(temp_sum_map[j].sum == current_line_lists[i]->sum) {
                    found_idx = j;
                    break;
                }
            }

            if(found_idx != -1){
                free_list(temp_sum_map[found_idx].list);
                temp_sum_map[found_idx].list = current_line_lists[i];
            } else{
                if(temp_map_count == temp_map_capacity){
                    temp_map_capacity *= 2;
                    temp_sum_map = (SumMapEntry*)realloc(temp_sum_map, temp_map_capacity * sizeof(SumMapEntry));
                    if (!temp_sum_map) {
                        perror("Erro ao realocar temp_sum_map");
                        exit(EXIT_FAILURE);
                    }
                }
                temp_sum_map[temp_map_count].sum = current_line_lists[i]->sum;
                temp_sum_map[temp_map_count].list = current_line_lists[i];
                temp_map_count++;
            }
        }
        
        IntList** final_lists_for_line = (IntList**)malloc(temp_map_count * sizeof(IntList*));
        if(!final_lists_for_line){
            perror("Erro ao alocar final_lists_for_line");
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < temp_map_count; i++){
            final_lists_for_line[i] = temp_sum_map[i].list;
        }

        qsort(final_lists_for_line, temp_map_count, sizeof(IntList*), compare_lists_by_sum);

        for(int i = 0; i < temp_map_count; i++){
            fprintf(fp_out, "start");
            for(int j = 0; j < final_lists_for_line[i]->count; j++){
                fprintf(fp_out, " %d", final_lists_for_line[i]->numbers[j]);
            }
            if(i < temp_map_count - 1){
                fprintf(fp_out, " ");
            }
        }
        fprintf(fp_out, "\n");

        for(int i = 0; i < temp_map_count; i++){
            free_list(temp_sum_map[i].list);
        }

        free(current_line_lists);
        free(temp_sum_map);
        free(final_lists_for_line);
    }

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}