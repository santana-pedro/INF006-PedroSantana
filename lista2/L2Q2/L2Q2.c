//ALUNOS: PEDRO SANTANA E VICTOR GABRIEL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    int key;
    struct Node* left;
    struct Node* right;
}Node;

Node* createNode(int key){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertNode(Node** rootRef, int key){
    if(*rootRef == NULL){
        *rootRef = createNode(key);
        return;
    }

    Node* current = *rootRef;
    Node* parent = NULL;

    while(current != NULL){
        parent = current;
        if(key < current->key){
            current = current->left;
        }else if(key > current->key){
            current = current->right;
        }else{
            return; // Nó duplicado, não insere
        }
    }

    if(key < parent->key){
        parent->left = createNode(key);
    }else{
        parent->right = createNode(key);
    }
}

long long sumSubtree(Node* node){
    if(node == NULL){
        return 0;
    }
    return (long long)node->key + sumSubtree(node->left) + sumSubtree(node->right);
}

void printPreOrder(Node* node, FILE* outFile, int* isFirst){
    if(node == NULL){
        return;
    }

    if(!(*isFirst)){
        fprintf(outFile, " ");
    }
    *isFirst = 0;

    long long sumRight = sumSubtree(node->right);
    long long sumLeft = sumSubtree(node->left);
    long long diff = sumRight - sumLeft;

    fprintf(outFile, "%d (%lld)", node->key, diff);

    printPreOrder(node->left, outFile, isFirst);
    printPreOrder(node->right, outFile, isFirst);
}

void freeTree(Node* root){
    if(root == NULL){
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void processLine(char* line, FILE* outFile){
    Node* root = NULL;
    
    char* p = line;
    int isEmpty = 1;
    while(*p != '\0' && *p != '\n'){
        if(*p != ' '){
            isEmpty = 0;
            break;
        }
        p++;
    }
    if(isEmpty) return;
    
    char* rest = line;
    int val;
    int n;
    while(sscanf(rest, "%d%n", &val, &n) == 1){
        insertNode(&root, val);
        rest += n;
    }

    int isFirst = 1;
    printPreOrder(root, outFile, &isFirst);
    
    fprintf(outFile, "\n");
    freeTree(root);
}

int main(){
    FILE* inFile = fopen("L2Q2.in", "r");
    FILE* outFile = fopen("L2Q2.out", "w");

    if(inFile == NULL || outFile == NULL){
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    char line[801];
    while(fgets(line, sizeof(line), inFile) != NULL){
        processLine(line, outFile);
    }

    fclose(inFile);
    fclose(outFile);

    return 0;
}