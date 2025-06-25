//Alunos: Pedro Santana e Victor Gabriel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_CAPACITY 10

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
    list->capacity = INITIAL_CAPACITY;
    list->numbers = (int*)malloc(list->capacity * sizeof(int));
    list->count = 0;
    list->sum = 0;
    return list;
}

void add_to_list(IntList* list, int number){
    if (list->count == list->capacity) {
        list->capacity *= 2;
        list->numbers = (int*)realloc(list->numbers, list->capacity * sizeof(int));
    }
    list->numbers[list->count++] = number;
}

void free_list(IntList* list){
    if (list) {
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

    IntList** all_lists = (IntList**)malloc(INITIAL_CAPACITY * sizeof(IntList*));
    int list_count = 0;
    int list_capacity = INITIAL_CAPACITY;

    char buffer[3001];
    IntList* current_list = NULL;

    while(fscanf(fp_in, "%s", buffer) != EOF){
        if(strcmp(buffer, "start") == 0){
            current_list = create_list();
            if(list_count == list_capacity){
                list_capacity *= 2;
                all_lists = (IntList**)realloc(all_lists, list_capacity * sizeof(IntList*));
            }
            all_lists[list_count++] = current_list;
        }else{
            if(current_list != NULL){
                add_to_list(current_list, atoi(buffer));
            }
        }
    }

    // Processamento das listas
    for(int i = 0; i < list_count; i++){
        long long current_sum = 0;
        for(int j = 0; j < all_lists[i]->count; j++){
            current_sum += all_lists[i]->numbers[j];
        }
        all_lists[i]->sum = current_sum;
        qsort(all_lists[i]->numbers, all_lists[i]->count, sizeof(int), compare_integers);
    }

    SumMapEntry* sum_map = (SumMapEntry*)malloc(list_count * sizeof(SumMapEntry));
    int map_count = 0;

    for(int i = 0; i < list_count; i++){
        int found_idx = -1;
        for(int j = 0; j < map_count; j++){
            if (sum_map[j].sum == all_lists[i]->sum) {
                found_idx = j;
                break;
            }
        }
        if(found_idx != -1){
            sum_map[found_idx].list = all_lists[i]; // mantém a última ocorrência
        }else{
            sum_map[map_count].sum = all_lists[i]->sum;
            sum_map[map_count].list = all_lists[i];
            map_count++;
        }
    }

    IntList** final_lists = (IntList**)malloc(map_count * sizeof(IntList*));
    for(int i = 0; i < map_count; i++){
        final_lists[i] = sum_map[i].list;
    }

    qsort(final_lists, map_count, sizeof(IntList*), compare_lists_by_sum);

    for(int i = 0; i < map_count; i++){
        fprintf(fp_out, "start");
        for(int j = 0; j < final_lists[i]->count; j++){
            fprintf(fp_out, " %d", final_lists[i]->numbers[j]);
        }
        fprintf(fp_out, "\n");
    }

    for(int i = 0; i < list_count; i++){
        free_list(all_lists[i]);
    }

    free(all_lists);
    free(sum_map);
    free(final_lists);

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}