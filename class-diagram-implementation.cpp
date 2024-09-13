#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

class Product {
public:
    std::string productId;
    std::string name;
    float price;

    Product(const std::string& id, const std::string& name, float price) : productId(id), name(name), price(price) {}
};

struct ProductCompare {
    bool operator()(const Product& p1, const Product& p2) const {
        return p1.productId < p2.productId;
    }
};

class ShoppingCart {
public:
    std::map<Product, int, ProductCompare> products;

    void addProduct(const Product& product, int quantity);
    void removeProduct(const std::string& productId);
    float getTotalAmount();
};

class Order {
public:
    std::string orderId;
    std::string orderDate;
    float totalAmount = 0;
    std::map<std::string, int> products;

    void addProduct(const Product& product, int quantity);
    void removeProduct(const std::string& productId);
    float getTotalAmount();
};

void ShoppingCart::addProduct(const Product& product, int quantity) {
    if (products.find(product) != products.end()) {
        products[product] += quantity;
    } else {
        products[product] = quantity;
    }
}

void ShoppingCart::removeProduct(const std::string& productId) {
    for (const auto& productPair : products) {
        if (productPair.first.productId == productId) {
            products.erase(productPair.first);
            break;
        }
    }
}

float ShoppingCart::getTotalAmount() {
    float total = 0.0;
    for (const auto& productPair : products) {
        total += productPair.first.price * productPair.second;
    }
    return total;
}

void Order::addProduct(const Product& product, int quantity) {
    products[product.productId] += quantity;
    totalAmount += product.price * quantity;
}

void Order::removeProduct(const std::string& productId) {
    products.erase(productId);
    totalAmount -= products[productId];
}

float Order::getTotalAmount() {
    return totalAmount;
}

Product products[] = {
    {"C001", "Hersheys", 150},
    {"C002", "Toblerone", 250},
    {"C003", "Kisses", 300},
    {"C004", "KitKat", 450},
};

void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. View Products\n";
    std::cout << "2. View Shopping Cart\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    ShoppingCart cart;

    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "\nProducts:\n";
                std::cout << std::setw(10) << "Product ID" << std::setw(15) << "Name" << std::setw(10) << "Price\n";
                for (const Product& product : products) {
                    std::cout << std::setw(10) << product.productId << std::setw(15) << product.name << std::setw(10) << product.price << std::endl;
                }

                char addMore;
                do {
                    std::cout << "\nEnter the ID of the product you want to add: ";
                    std::string productId;
                    std::cin >> productId;

                    bool found = false;
                    for (const Product& product : products) {
                        if (product.productId == productId) {
                            int quantity;
                            std::cout << "Enter the quantity: ";
                            std::cin >> quantity;

                            cart.addProduct(product, quantity);
                            std::cout << "Product added successfully!\n";
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        std::cout << "Product not found.\n";
                    }

                    std::cout << "Do you want to add another product? (Y/N): ";
                    std::cin >> addMore;
                } while (addMore == 'Y' || addMore == 'y');
                break;
            }
            case 2: {
                std::cout << "\nShopping Cart:\n";
                std::cout << std::setw(10) << "Product ID" << std::setw(15) << "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity\n";
                for (const auto& productPair : cart.products) {
                    const Product& product = productPair.first;
                    int quantity = productPair.second;
                    std::cout << std::setw(10) << product.productId << std::setw(15) << product.name << std::setw(10) << product.price << std::setw(10) << quantity << std::endl;
                }

                char checkout;
                std::cout << "\nDo you want to check out all the products? (Y/N): ";
                std::cin >> checkout;
                if (checkout == 'Y' || checkout == 'y') {
                    Order order;
                    for (const auto& productPair : cart.products) {
                        order.addProduct(productPair.first, productPair.second);
                    }
                    std::cout << "\nTotal Amount: $" << order.getTotalAmount() << std::endl;
                    std::cout << "You have successfully checked out the products!\n";
                    cart.products.clear();
                }
                break;
            }
            case 3: {
                std::cout << "Exiting...\n";
                return 0;
            }
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }

    return 0;
}
