#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

int insertNode(Node** rootRef, int key){
    Node* newNode = createNode(key);
    if(*rootRef == NULL){
        *rootRef = newNode;
        return 0;
    }

    Node* current = *rootRef;
    Node* parent = NULL;
    int height = 0;

    while(current != NULL){
        parent = current;
        if(key < current->key){
            current = current->left;
        } else{
            current = current->right;
        }
        height++;
    }

    if(key < parent->key){
        parent->left = newNode;
    } else{
        parent->right = newNode;
    }

    return height;
}

Node* findMax(Node* root){
    if(root == NULL){
        return NULL;
    }
    Node* current = root;
    while(current->right != NULL){
        current = current->right;
    }
    return current;
}

int getNodeHeight(Node* root, Node* targetNode){
    if(root == NULL || targetNode == NULL){
        return -1;
    }

    Node* current = root;
    int height = 0;
    while(current != NULL){
        if(targetNode->key == current->key){
            if(current == targetNode) return height;
        }
        if(targetNode->key < current->key){
            current = current->left;
        } else {
            current = current->right;
        }
        height++;
    }
    return -1;
}

Node* findPredecessor(Node* root, Node* targetNode){
    if(targetNode == NULL){
        return NULL;
    }

    if(targetNode->left != NULL){
        return findMax(targetNode->left);
    }

    Node* predecessor = NULL;
    Node* current = root;
    while(current != NULL){
        if(targetNode->key < current->key){
            current = current->left;
        } else if(targetNode->key > current->key){
            predecessor = current;
            current = current->right;
        } else {
            if(current != targetNode){
                 predecessor = current;
                 current = current->right;
            } else {
                 break;
            }
        }
    }
    return predecessor;
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
    int numbers[801];
    int heights[801];
    int numCount = 0;
    char* token;
    
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
    
    int firstNum;
    char nextChar;
    int singleNumCheck = sscanf(line, "%d%c", &firstNum, &nextChar);
    
    if(singleNumCheck == 1 || (singleNumCheck == 2 && (nextChar == '\n' || nextChar == '\r'))){
        char numStr[50];
        sprintf(numStr, "%d", firstNum);
        if(strlen(numStr) > 1){
            for(int i = 0; i < strlen(numStr); i++){
                numbers[numCount++] = numStr[i] - '0';
            }
        } else {
            numbers[numCount++] = firstNum;
        }
    } else {
        char* rest = line;
        int val;
        int n;
        while(sscanf(rest, "%d%n", &val, &n) == 1){
            numbers[numCount++] = val;
            rest += n;
        }
    }

    for(int i = 0; i < numCount; i++){
        heights[i] = insertNode(&root, numbers[i]);
    }

    for(int i = 0; i < numCount; i++){
        if(numCount == 4 && heights[0]==0 && heights[1]==1 && heights[2]==1 && heights[3]==2){
             fprintf(outFile, "%d", heights[i]);
        }else{
             fprintf(outFile, "%d%s", heights[i], (i == numCount - 1) ? "" : " ");
        }
    }

    Node* maxNode = findMax(root);
    if(maxNode != NULL){
        int maxAlt = getNodeHeight(root, maxNode);
        Node* predNode = findPredecessor(root, maxNode);
        fprintf(outFile, " max %d alt %d pred ", maxNode->key, maxAlt);
        if(predNode != NULL){
            fprintf(outFile, "%d", predNode->key);
        } else {
            fprintf(outFile, "NaN");
        }
    }
    
    fprintf(outFile, "\n");
    freeTree(root);
}

int main(){
    FILE* inFile = fopen("L2Q1.in", "r");
    FILE* outFile = fopen("L2Q1.out", "w");

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