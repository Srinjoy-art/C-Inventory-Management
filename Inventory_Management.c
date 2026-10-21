
#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 100
#define FILE_NAME "inventory.txt"

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};

struct Product products[MAX_PRODUCTS];
int count = 0;

int findProduct(int id) {
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

void saveProducts() {
    FILE *file = fopen(FILE_NAME, "w");

    if (file == NULL) {
        printf("Error saving inventory!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%49[^|]|%.2f|%d\n",
                products[i].id,
                products[i].name,
                products[i].price,
                products[i].quantity);
    }

    fclose(file);
}

void loadProducts() {
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        return;
    }

    while (count < MAX_PRODUCTS &&
           fscanf(file, "%d|%49[^|]|%f|%d\n",
                  &products[count].id,
                  products[count].name,
                  &products[count].price,
                  &products[count].quantity) == 4) {
        count++;
    }

    fclose(file);
}

void addProduct() {
    if (count >= MAX_PRODUCTS) {
        printf("Inventory is full!\n");
        return;
    }

    struct Product p;

    printf("Enter product ID: ");
    if (scanf("%d", &p.id) != 1) {
        printf("Invalid ID!\n");
        while (getchar() != '\n');
        return;
    }

    if (findProduct(p.id) != -1) {
        printf("Product ID already exists!\n");
        return;
    }

    printf("Enter product name: ");
    scanf(" %49[^\n]", p.name);

    // The delimiter | is reserved for the data file.
    if (strchr(p.name, '|') != NULL) {
        printf("Product name cannot contain |.\n");
        return;
    }

    printf("Enter price: ");
    if (scanf("%f", &p.price) != 1 || p.price <= 0) {
        printf("Invalid price!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter quantity: ");
    if (scanf("%d", &p.quantity) != 1 || p.quantity < 0) {
        printf("Invalid quantity!\n");
        while (getchar() != '\n');
        return;
    }

    products[count++] = p;
    saveProducts();

    printf("Product added successfully!\n");
}

void displayProducts() {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\n%-8s %-25s %-12s %-10s\n",
           "ID", "Name", "Price", "Quantity");

    for (int i = 0; i < count; i++) {
        printf("%-8d %-25s %-12.2f %-10d\n",
               products[i].id,
               products[i].name,
               products[i].price,
               products[i].quantity);
    }
}

void searchProduct() {
    int id;

    printf("Enter product ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(id);

    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("\nProduct found!\n");
    printf("ID       : %d\n", products[index].id);
    printf("Name     : %s\n", products[index].name);
    printf("Price    : %.2f\n", products[index].price);
    printf("Quantity : %d\n", products[index].quantity);
}

void updateStock() {
    int id, quantity;

    printf("Enter product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(id);

    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("Enter new stock quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity < 0) {
        printf("Invalid quantity!\n");
        while (getchar() != '\n');
        return;
    }

    products[index].quantity = quantity;
    saveProducts();

    printf("Stock updated successfully!\n");
}

void sellProduct() {
    int id, quantity;

    printf("Enter product ID to sell: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(id);

    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("Enter quantity to sell: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Invalid quantity!\n");
        while (getchar() != '\n');
        return;
    }

    if (quantity > products[index].quantity) {
        printf("Insufficient stock!\n");
        return;
    }

    products[index].quantity -= quantity;

    printf("\nSale successful!\n");
    printf("Product   : %s\n", products[index].name);
    printf("Quantity  : %d\n", quantity);
    printf("Unit price: %.2f\n", products[index].price);
    printf("Total bill: %.2f\n",
           products[index].price * quantity);

    saveProducts();
}

void lowStockAlert() {
    int found = 0;

    printf("\n--- Low Stock Products (5 or fewer) ---\n");

    for (int i = 0; i < count; i++) {
        if (products[i].quantity <= 5) {
            printf("%s | Remaining: %d\n",
                   products[i].name,
                   products[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No low-stock products.\n");
    }
}

int main() {
    int choice;

    loadProducts();

    do {
        printf("\n===== INVENTORY MANAGEMENT =====\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Search Product\n");
        printf("4. Update Stock\n");
        printf("5. Sell Product\n");
        printf("6. Low Stock Alert\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayProducts();
                break;
            case 3:
                searchProduct();
                break;
            case 4:
                updateStock();
                break;
            case 5:
                sellProduct();
                break;
            case 6:
                lowStockAlert();
                break;
            case 0:
                saveProducts();
                printf("Inventory saved. Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
