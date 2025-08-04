#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void addNode(Node** rootRef, int key){
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
            return;
        }
    }
    if(key < parent->key){
        parent->left = createNode(key);
    }else{
        parent->right = createNode(key);
    }
}

Node* findMin(Node* node){
    while(node != NULL && node->left != NULL){
        node = node->left;
    }
    return node;
}

Node* removeRecursive(Node* root, int key){
    if(root == NULL) return root;

    if(key < root->key){
        root->left = removeRecursive(root->left, key);
    }else if(key > root->key){
        root->right = removeRecursive(root->right, key);
    }else{
        if(root->left == NULL){
            Node* temp = root->right;
            free(root);
            return temp;
        }else if(root->right == NULL){
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = findMin(root->right);
        root->key = temp->key;
        root->right = removeRecursive(root->right, temp->key);
    }
    return root;
}

int searchNode(Node* root, int key){
    Node* current = root;
    while(current != NULL){
        if(key == current->key) return 1;
        if(key < current->key) current = current->left;
        else current = current->right;
    }
    return 0;
}

int getNodeHeight(Node* root, int key){
    Node* current = root;
    int height = 0;
    while(current != NULL){
        if(key == current->key) return height;
        if(key < current->key) current = current->left;
        else current = current->right;
        height++;
    }
    return -1;
}

void printInOrder(Node* node, Node* root, FILE* outFile, int* isFirst){
    if(node == NULL) return;
    
    printInOrder(node->left, root, outFile, isFirst);
    
    if(!(*isFirst)){
        fprintf(outFile, " ");
    }
    *isFirst = 0;
    
    int height = getNodeHeight(root, node->key);
    fprintf(outFile, "%d (%d)", node->key, height);
    
    printInOrder(node->right, root, outFile, isFirst);
}

void freeTree(Node* root){
    if(root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void processLine(char* line, FILE* outFile){
    Node* root = NULL;
    char* p = line;

    while(*p != '\0'){
        char op = 0;
        int value = 0;
        
        if(*p == 'a' || *p == 'r'){
            op = *p;
            p++;
            while(*p != '\0' && !isdigit(*p)){
                p++;
            }
            if(isdigit(*p)){
                value = strtol(p, &p, 10);
            }
            
            if(op == 'a'){
                addNode(&root, value);
            }else if(op == 'r'){
                if(searchNode(root, value)){
                    root = removeRecursive(root, value);
                }else{
                    addNode(&root, value);
                }
            }
        }else{
            p++;
        }
    }
    
    int isFirst = 1;
    printInOrder(root, root, outFile, &isFirst);
    fprintf(outFile, "\n");
    freeTree(root);
}

int main(){
    FILE* inFile = fopen("L2Q3.in", "r");
    FILE* outFile = fopen("L2Q3.out", "w");

    if(inFile == NULL || outFile == NULL){
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    char line[801];
    while(fgets(line, sizeof(line), inFile) != NULL){
        if(strspn(line, " \t\n\r") == strlen(line)) continue;
        processLine(line, outFile);
    }

    fclose(inFile);
    fclose(outFile);

    return 0;
}