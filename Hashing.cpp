#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 10

struct Node {
    char key[100];
    struct Node* next;
};

struct HashTable {
    char key[100];
    struct Node* chain;
} ht[TABLE_SIZE];

// Initialization function for the hash table
void initializationHT() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        strcpy(ht[i].key, ""); // Set key to empty string
        ht[i].chain = NULL;    // Initialize chain to NULL
    }
}

int StoI(char key[]) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += key[i]; // Sum of ASCII values of characters
    }
    return sum;
}

// Hash Function (Division)
int DivHash(char key[]) {
    int intKey = StoI(key);
    return intKey % TABLE_SIZE;
}

// Hash Function (Mid-Square)
int MidSquareHash(char key[]) {
    int intKey = StoI(key);
    intKey = intKey * intKey;
    return (intKey / 100) % TABLE_SIZE;
}

// Hash Function (Folding)
int FoldingHash(char key[]) {
    int intKey = StoI(key);
    int sum = 0;
    while (intKey > 0) {
        sum += intKey % 100; // Sum the pairs of digits
        intKey /= 100;
    }
    return sum % TABLE_SIZE;
}

// Hash Function (Digit Extraction)
int DigitExtractionHash(char key[]) {
    int intKey = StoI(key);
    char str[10];
    sprintf(str, "%d", intKey);
    int extractedDigit = str[1] - '0'; // Extract second digit
    return extractedDigit % TABLE_SIZE;
}

// Hash Function (Rotating)
int RotatingHash(char key[]) {
    int intKey = StoI(key);
    int rotate = (intKey << 4) | (intKey >> (8 - 4)); // Rotate 4 bits left
    return rotate % TABLE_SIZE;
}

// Collision Handling (Linear Probing)
int LinearProbing(char key[], int index) {
    while (strcmp(ht[index].key, "") != 0) {
        index = (index + 1) % TABLE_SIZE;
    }
    return index;
}

// Collision Handling (Rehashing)
int Rehashing(char key[], int index) {
    int intKey = StoI(key);
    int newHash = 7 - (intKey % 7);
    while (strcmp(ht[index].key, "") != 0) {
        index = (index + newHash) % TABLE_SIZE;
    }
    return index;
}

// Collision Handling (Chaining)
void Chaining(char key[], int index) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->key, key);
    newNode->next = ht[index].chain;
    ht[index].chain = newNode;
}

// Insert data into the Hash Table
void insert(char key[], int (*hashFunction)(char[]), void (*collisionHandling)(char[], int)) {
    int index = hashFunction(key);
    if (strcmp(ht[index].key, "") == 0) {
        strcpy(ht[index].key, key);
        printf("Key %s inserted at index %d.\n", key, index);
    } else {
        printf("Collision occurred!\n");
        collisionHandling(key, index);
    }
}

// Display Hash Table
void displayHT() {
    printf("\nHash Table\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (strcmp(ht[i].key, "") != 0) {
            printf("Index %d: %s", i, ht[i].key);
            struct Node* temp = ht[i].chain;
            while (temp != NULL) {
                printf(" -> %s", temp->key);
                temp = temp->next;
            }
            printf("\n");
        } else {
            printf("Index %d: Empty\n", i);
        }
    }
}

int main() {
    initializationHT();
    char key[100];
    char keys[TABLE_SIZE][100];
    int keyCount = 0;

    // Input all keys
    while (keyCount < TABLE_SIZE) {
        printf("Enter key (string) or 'done' to finish input: ");
        scanf("%s", key);
        if (strcmp(key, "done") == 0) break;
        strcpy(keys[keyCount], key);
        keyCount++;
    }

    // Pointers to selected hash function and collision handling method
    int (*hashFunction)(char[]) = NULL;
    void (*collisionHandling)(char[], int) = NULL;

    // Select hash function
    int hashChoice;
    printf("Choose Hash Function:\n1. Division\n2. Mid-Square\n3. Folding\n4. Digit Extraction\n5. Rotating\nEnter choice: ");
    scanf("%d", &hashChoice);

    switch (hashChoice) {
        case 1: hashFunction = DivHash; break;
        case 2: hashFunction = MidSquareHash; break;
        case 3: hashFunction = FoldingHash; break;
        case 4: hashFunction = DigitExtractionHash; break;
        case 5: hashFunction = RotatingHash; break;
        default: printf("Invalid choice!\n"); return 1;
    }

    // Select collision handling method
    int collisionChoice;
    printf("Choose Collision Handling:\n1. Linear Probing\n2. Rehashing\n3. Chaining\nEnter choice: ");
    scanf("%d", &collisionChoice);

    switch (collisionChoice) {
        case 1: collisionHandling = (void (*)(char[], int))LinearProbing; break;
        case 2: collisionHandling = (void (*)(char[], int))Rehashing; break;
        case 3: collisionHandling = (void (*)(char[], int))Chaining; break;
        default: printf("Invalid choice!\n"); return 1;
    }

    // Insert keys into hash table
    for (int i = 0; i < keyCount; i++) {
        insert(keys[i], hashFunction, collisionHandling);
    }

    displayHT();

    return 0;
}
