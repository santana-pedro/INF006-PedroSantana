// Nome da equipe: Pedro Santana
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_CAPACITY 10

// Estrutura para uma lista de inteiros
typedef struct {
    int* numbers;
    int count;
    int capacity;
    long long sum;
} IntList;

// Estrutura para o mapa de soma -> lista (para filtrar duplicatas)
typedef struct {
    long long sum;
    IntList* list;
} SumMapEntry;

// Funções auxiliares para a IntList
IntList* create_list() {
    IntList* list = (IntList*)malloc(sizeof(IntList));
    list->capacity = INITIAL_CAPACITY;
    list->numbers = (int*)malloc(list->capacity * sizeof(int));
    list->count = 0;
    list->sum = 0;
    return list;
}

void add_to_list(IntList* list, int number) {
    if (list->count == list->capacity) {
        list->capacity *= 2;
        list->numbers = (int*)realloc(list->numbers, list->capacity * sizeof(int));
    }
    list->numbers[list->count++] = number;
}

void free_list(IntList* list) {
    if (list) {
        free(list->numbers);
        free(list);
    }
}

// Função de comparação para qsort (ordem crescente)
int compare_integers(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Função de comparação para qsort (ordem decrescente de soma)
int compare_lists_by_sum(const void* a, const void* b) {
    IntList* listA = *(IntList**)a;
    IntList* listB = *(IntList**)b;
    if (listB->sum > listA->sum) return 1;
    if (listB->sum < listA->sum) return -1;
    return 0;
}


int main() {
    // Armazena todas as listas lidas na ordem original
    IntList** all_lists = (IntList**)malloc(INITIAL_CAPACITY * sizeof(IntList*));
    int list_count = 0;
    int list_capacity = INITIAL_CAPACITY;

    char buffer[3001];
    IntList* current_list = NULL;

    // Lê todo o input
    while (scanf("%s", buffer) != EOF) {
        if (strcmp(buffer, "start") == 0) {
            current_list = create_list();
            if (list_count == list_capacity) {
                list_capacity *= 2;
                all_lists = (IntList**)realloc(all_lists, list_capacity * sizeof(IntList*));
            }
            all_lists[list_count++] = current_list;
        } else {
            if (current_list != NULL) {
                add_to_list(current_list, atoi(buffer));
            }
        }
    }

    // Processa as listas
    // 1. Calcula a soma e ordena internamente cada lista
    for (int i = 0; i < list_count; i++) {
        long long current_sum = 0;
        for (int j = 0; j < all_lists[i]->count; j++) {
            current_sum += all_lists[i]->numbers[j];
        }
        all_lists[i]->sum = current_sum;
        qsort(all_lists[i]->numbers, all_lists[i]->count, sizeof(int), compare_integers);
    }

    // 2. Filtra listas com a mesma soma, mantendo apenas a última
    // Usamos uma abordagem de mapa simples (array de pares)
    SumMapEntry* sum_map = (SumMapEntry*)malloc(list_count * sizeof(SumMapEntry));
    int map_count = 0;

    for (int i = 0; i < list_count; i++) {
        int found_idx = -1;
        for (int j = 0; j < map_count; j++) {
            if (sum_map[j].sum == all_lists[i]->sum) {
                found_idx = j;
                break;
            }
        }
        if (found_idx != -1) {
            // Se a soma já existe, substitui a lista pela mais recente
            sum_map[found_idx].list = all_lists[i];
        } else {
            // Se a soma é nova, adiciona
            sum_map[map_count].sum = all_lists[i]->sum;
            sum_map[map_count].list = all_lists[i];
            map_count++;
        }
    }
    
    // 3. Coloca as listas finais em um novo array para ordenar
    IntList** final_lists = (IntList**)malloc(map_count * sizeof(IntList*));
    for (int i = 0; i < map_count; i++) {
        final_lists[i] = sum_map[i].list;
    }
    
    // 4. Ordena as listas finais pela soma em ordem decrescente 
    qsort(final_lists, map_count, sizeof(IntList*), compare_lists_by_sum);

    // 5. Imprime o resultado
    for (int i = 0; i < map_count; i++) {
        printf("start");
        for (int j = 0; j < final_lists[i]->count; j++) {
            printf(" %d", final_lists[i]->numbers[j]);
        }
        printf("\n");
    }

    // Libera a memória
    for (int i = 0; i < list_count; i++) {
        free_list(all_lists[i]);
    }
    free(all_lists);
    free(sum_map);
    free(final_lists);

    return 0;
}