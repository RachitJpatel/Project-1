#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure name parcel node is Define for  the Binary Search Tree
typedef struct ParcelNode
{
    char* destination;
    int weight;
    float valuation;
    struct ParcelNode* left;
    struct ParcelNode* right;
} ParcelNode;

// Structure to Name HashTable is Define for a Hash Table
typedef struct HashTable
{
    ParcelNode* buckets[127];
} HashTable;
//
// FUNCTION: hash_djb2
//DESCRIPTION:
// The  Function is created To assign hash value using djb2 hash function
//PARAMETERS: char* str
//
unsigned long hash_djb2(char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % 127;
}
//
// FUNCTION:createParcelNode
//DESCRIPTION:
// Function is created to create a new parcel node
//PARAMETERS: const char* destination, int weight, float valuation
//
ParcelNode* createParcelNode(const char* destination, int weight, float valuation)
{
    ParcelNode* newNode = (ParcelNode*)malloc(sizeof(ParcelNode));
    if (newNode == NULL)
    {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    newNode->destination = (char*)malloc(strlen(destination) + 1);
    if (newNode->destination == NULL)
    {
        perror("Failed to allocate memory for destination");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    strcpy_s(newNode->destination, strlen(destination) + 1, destination);

    newNode->weight = weight;
    newNode->valuation = valuation;
    newNode->left = newNode->right = NULL;
    return newNode;
}
//
// FUNCTION:insertParcelNode
//DESCRIPTION:
// Function  created to  insert a parcel node into the Binary Search Tree
//PARAMETERS: ParcelNode* root, char* destination, int weight, float valuation
//
ParcelNode* insertParcelNode(ParcelNode* root, char* destination, int weight, float valuation)
{
    if (root == NULL)
    {
        return createParcelNode(destination, weight, valuation);
    }
    if (weight < root->weight)
    {
        root->left = insertParcelNode(root->left, destination, weight, valuation);
    }
    else
    {
        root->right = insertParcelNode(root->right, destination, weight, valuation);
    }
    return root;
}
//
// FUNCTION:insertParcel
//DESCRIPTION:
// Function is created to insert the parcel into a hash table
//PARAMETERS: HashTable* hashTable, char* destination, int weight, float valuation
//
void insertParcel(HashTable* hashTable, char* destination, int weight, float valuation)
{
    unsigned long index = hash_djb2(destination);
    hashTable->buckets[index] = insertParcelNode(hashTable->buckets[index], destination, weight, valuation);
}
ParcelNode* searchByWeight(ParcelNode* root, int weight)
{
    if (root == NULL || root->weight == weight)
    {
        return root;
    }
    if (weight < root->weight)
    {
        return searchByWeight(root->left, weight);
    }
    else
    {
        return searchByWeight(root->right, weight);
    }
}
//
// FUNCTION:FindMin
//DESCRIPTION:
// Function  is created to find Minimum value of Weight node in BST
//PARAMETERS: ParcelNode* root
//
ParcelNode* findMin(ParcelNode* root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

//
// FUNCTION:FindMax
//DESCRIPTION:
// Function  is created to find Minimum value of Weight node in BST
//PARAMETERS: ParcelNode* root
//
ParcelNode* findMax(ParcelNode* root)
{
    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}
//
// FUNCTION:findMixValuation
//DESCRIPTION:
// Function  is created to find Minimum value of Valuation node in BST
//PARAMETERS: ParcelNode* root
//
ParcelNode* findMinValuation(ParcelNode* root) {
    if (root == NULL) {
        return NULL;
    }

    ParcelNode* minNode = root;

    ParcelNode* leftMin = findMinValuation(root->left);
    if (leftMin != NULL && leftMin->valuation < minNode->valuation) {
        minNode = leftMin;
    }

    ParcelNode* rightMin = findMinValuation(root->right);
    if (rightMin != NULL && rightMin->valuation < minNode->valuation) {
        minNode = rightMin;
    }

    return minNode;
}
//
// FUNCTION:findMaxValuation
//DESCRIPTION:
// Function  is created to find Minimum value of Valuation node in BST
//PARAMETERS: ParcelNode* root
//
ParcelNode* findMaxValuation(ParcelNode* root) {
    if (root == NULL) {
        return NULL;
    }

    ParcelNode* maxNode = root;

    ParcelNode* leftMax = findMaxValuation(root->left);
    if (leftMax != NULL && leftMax->valuation > maxNode->valuation) {
        maxNode = leftMax;
    }

    ParcelNode* rightMax = findMaxValuation(root->right);
    if (rightMax != NULL && rightMax->valuation > maxNode->valuation) {
        maxNode = rightMax;
    }

    return maxNode;
}


//
// FUNCTION:displayParcels
//DESCRIPTION:
// Function  is created to display all the parcles of particular company
//PARAMETERS: ParcelNode* root
//
void displayParcels(ParcelNode* root)
{
    if (root != NULL)
    {
        displayParcels(root->left);
        printf("Destination: %s, Weight: %d g, Valuation: $%.2f\n", root->destination, root->weight, root->valuation);
        displayParcels(root->right);
    }
}
//
// FUNCTION:displayParcelsByWeight
//DESCRIPTION:
// Function is created To display the Parcel by it Weight 
//PARAMETERS: ParcelNode* root, int weight, int higher
void displayParcelsByWeight(ParcelNode* root, int weight, int higher)
{
    if (root == NULL)
    {
        return;
    }
    displayParcelsByWeight(higher ? root->right : root->left, weight, higher);
    if ((higher && root->weight > weight) || (!higher && root->weight < weight))
    {
        printf("Destination: %s, Weight: %d g, Valuation: $%.2f\n", root->destination, root->weight, root->valuation);
    }
    displayParcelsByWeight(higher ? root->left : root->right, weight, higher);
}
//
// FUNCTION:calculateTotals
//DESCRIPTION:
// Function is created to calculate the total amount of the parcels of the Particular Company
//PARAMETERS: ParcelNode* root, int* totalWeight, float* totalValuation
//
void calculateTotals(ParcelNode* root, int* totalWeight, float* totalValuation)
{
    if (root != NULL)
    {
        *totalWeight += root->weight;
        *totalValuation += root->valuation;
        calculateTotals(root->left, totalWeight, totalValuation);
        calculateTotals(root->right, totalWeight, totalValuation);
    }
}
//
// FUNCTION:displayMenu
//DESCRIPTION:
// Function  is created to to display the menu for the the user to Select the Choice ans for various opeartion
//PARAMETERS: HashTable* hashTable
//
void displayMenu(HashTable* hashTable)
{
    int choice;
    int weight;
    int higher;
    char country[20];
    ParcelNode* node;
    do
    {
        printf("\nMenu:\n");
        printf("1. Enter country name and display all parcels\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display total parcel load and valuation for the country\n");
        printf("4. Display cheapest and most expensive parcel details\n");
        printf("5. Display lightest and heaviest parcel for the country\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf_s("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            node = hashTable->buckets[hash_djb2(country)];
            if (node == NULL)
            {
                printf("No parcels found for %s.\n", country);
            }
            else
            {
                displayParcels(node);
            }
            break;
        case 2:
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            printf("Enter weight: ");
            scanf_s("%d", &weight);
            printf("1. Display parcels heavier than %d g\n", weight);
            printf("2. Display parcels lighter than %d g\n", weight);
            printf("Enter your choice: ");
            scanf_s("%d", &higher);
            node = hashTable->buckets[hash_djb2(country)];
            if (node == NULL)
            {
                printf("No parcels found for %s.\n", country);
            }
            else
            {
                displayParcelsByWeight(node, weight, higher == 1);
            }
            break;
        case 3:
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            node = hashTable->buckets[hash_djb2(country)];
            if (node == NULL)
            {
                printf("No parcels found for %s.\n", country);
            }
            else
            {
                int totalWeight = 0;
                float totalValuation = 0.0;
                calculateTotals(node, &totalWeight, &totalValuation);
                printf("Total weight: %d g, Total valuation: $%.2f\n", totalWeight, totalValuation);
            }
            break;
        case 4:
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            node = hashTable->buckets[hash_djb2(country)];
            if (node == NULL)
            {
                printf("No parcels found for %s.\n", country);
            }
            else
            {
                ParcelNode* minNode = findMinValuation(node);
                ParcelNode* maxNode = findMaxValuation(node);
                printf("Cheapest Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", minNode->destination, minNode->weight, minNode->valuation);
                printf("Most Expensive Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", maxNode->destination, maxNode->weight, maxNode->valuation);
            }
            break;
        case 5:
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            node = hashTable->buckets[hash_djb2(country)];
            if (node == NULL)
            {
                printf("No parcels found for %s.\n", country);
            }
            else
            {
                ParcelNode* minNode = findMin(node);
                ParcelNode* maxNode = findMax(node);
                printf("Lightest Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", minNode->destination, minNode->weight, minNode->valuation);
                printf("Heaviest Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", maxNode->destination, maxNode->weight, maxNode->valuation);
            }
            break;
        case 6:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
}
//
// FUNCTION:freeBST
//DESCRIPTION:
// Function  created to Free tge memory of the Binary Seach tree
//PARAMETERS: ParcelNode* root
//
void freeBST(ParcelNode* root)
{
    if (root != NULL)
    {
        freeBST(root->left);
        freeBST(root->right);
        free(root->destination);
        free(root);
    }
}
//
// FUNCTION: freeHashTable
//DESCRIPTION:
// Function  created to Free the memory of the Hash Table
//PARAMETERS: HashTable* hashTable
//
void freeHashTable(HashTable* hashTable)
{
    for (int i = 0; i < 127; i++)
    {
        if (hashTable->buckets[i] != NULL)
        {
            freeBST(hashTable->buckets[i]);
        }
    }
}

int main()
{
    HashTable hashTable = { { NULL } };
    FILE* file;
    errno_t err = fopen_s(&file, "couriers.txt", "r");
    if (err != 0 || !file)
    {
        printf("Could not open couriers.txt file.\n");
        return 1;
    }
    char destination[20];
    int weight;
    float valuation;
    while (fscanf_s(file, "%[^,], %d, %f\n", destination, (unsigned)_countof(destination), &weight, &valuation) != -1)
    {
        insertParcel(&hashTable, destination, weight, valuation);
    }
    fclose(file);
    displayMenu(&hashTable);
    freeHashTable(&hashTable);

    return 0;
}
