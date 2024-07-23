#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <map>
#include <cctype>

using namespace std;

// Constants for admin credentials
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "adminpass";
// ANSI escape codes for colors
#define RESET   "\033[0m"
#define LIGHT_BLUE    "\033[94m"
#define LIGHT_ORANGE  "\033[38;5;214m"
// Struct definitions
struct Rating {
    string username;
    string carBrand;
    string carModel;
    int stars;
    string comment;
};

struct Car {
    string brand;
    string model;
    int year;
    int price;
    string fuel;
    string type;
    int seats;
    string info;
};

struct User {
    string username;
    string full_name;
    string hashed_password;
    string address;
    string email;
    string contact_num;
    long long balance;
    string password; // Store plain text password temporarily during registration
};

struct Transaction {
    string transactionID;
    string name;
    string carBrand;
    string carModel;
    int carPrice;
    string timestamp;
    int carYear;
};

// Structure for holding filtered car results
struct filtered_array {
    int size;
    Car* data;

    filtered_array(int s) : size(s) {
        data = new Car[size];
    }
};

// Function prototypes
void logo();
unsigned int customHash(const std::string& password);
void hashPassword(User& user);
bool usernameExists(const string& username);
bool verifyPassword(const User& user, const string& inputPassword);
void user_register();
void user_login();
Car construct_car(string brand, string model, int year, int price, string fuel, string type, int seats, string info);
void swap(Car& a, Car& b);
void sort_by_year(Car arr[], int arr_size);
void sort_by_price(Car arr[], int arr_size);
void sortCars(Car arr[], int arr_size);
filtered_array filter_by_year(Car arr[], int year, int arr_size);
filtered_array filter_by_seats(Car arr[], int seats, int arr_size);
filtered_array filter_by_price(Car arr[], int budget, int arr_size);
filtered_array filter_by_fuel(Car arr[], string fuel, int arr_size);
filtered_array filter_by_gear(Car arr[], string gear, int arr_size);
filtered_array total_filter(int year, int price, string fuel, string gear, int seats, Car arr[], int arr_size);
vector<string> split(const string& str, char delimiter);
void load_csv(Car arr[]);
void overwrite_to_csv(Car arr[], int arr_size);
int count_csv();
void add_to_csv(string brand, string model, int year, int price, string fuel, string type, int seats, string info);
void delete_in_csv(int target_index, Car arr[], int arr_size);
void add_car(Car arr[], int arr_size);
filtered_array search_cars(Car arr[], int arr_size);
void updateUserFile(const User& user);
void addRating(const Rating& rating);
void rateCar(const User& currentUser, const Car& car);
void displayRatings();
void saveTransaction(const Transaction& transaction);
string generateTransactionID();
string getCurrentTimestamp();
void displayReceipt(const Transaction& transaction);
void buy_car(User& currentUser, Car car);
void show_cars(Car arr[], int arr_size, User& currentUser);
void show_all_cars(User& currentUser);
void show_cars_for_deletion(Car arr[], int arr_size);
void delete_car();
void admin_menu();
void admin_login();
void recommend_car(User& currentUser);
void edit_personal_details(User& currentUser);
void personal_details(User& currentUser);
void balance(const User& currentUser);
void top_up(User& currentUser);
void history_receipts(const User& currentUser);
bool LoggedValidChoice(int choice);
int getLoggedValidChoice();
void logged_in_menu(User& currentUser);
bool isValidChoice(int choice);
int getValidChoice();

// Main Function
int main() {
    while (true) {
        int choice = getValidChoice();

        switch (choice) {
        case 1:
            user_login();
            break;
        case 2:
            user_register();
            break;
        case 3:
            admin_login();
            break;
        case 4:
            displayRatings();
            break;
        case 5:
            cout << "  Exiting program.\n";
            return 0;
        default:
            cout << "  \nInvalid choice. Please select a number from 1 to 5.\n";
            break;
        }
    }
    return 0;
}

// --- Function Definitions ---

//logo.

void logo() {
    std::cout << LIGHT_BLUE << "               _        _____ _ _       _\n";
    cout << "    /\\        | |      |  __ (_) |     | |\n";
    cout << "   /  \\  _   _| |_ ___ | |__) || | ___ | |_\n";
    cout << "  / /\\ \\| | | | __/ _ \\|  ___/ | |/ _ \\| __|\n";
    cout << " / ____ \\ |_| | || (_) | |   | | | (_) | |_\n";
    cout << "/_/    \\_\\__,_|\\__\\___/|_|   |_|_|\\___/ \\__|\n";
    cout << LIGHT_ORANGE << "\nAutoPilot: Personalized Picks for Your Perfect Ride" << endl << RESET <<
        "----------------------------------------------------"
        << std::endl;
}

// Hashing and User Authentication
unsigned int customHash(const std::string& password) {
    unsigned int hash = 0;
    for (char c : password) {
        hash += c;
    }
    return hash;
}

void hashPassword(User& user) {
    user.hashed_password = to_string(customHash(user.password));
}

bool usernameExists(const string& username) {
    ifstream userFile("users.txt");
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            if (line.find("Username: " + username) != string::npos) {
                userFile.close();
                return true;
            }
        }
        userFile.close();
    }
    return false;
}

bool verifyPassword(const User& user, const string& inputPassword) {
    string inputHashed = to_string(customHash(inputPassword));
    return inputHashed == user.hashed_password;
}

// User Registration
void user_register() {

    while (true) {
        User user;

        system("cls");
        logo();
        cout << "\n  REGISTER\n";

        cout << "\n  Enter a Valid Username (Press ENTER to go back): ";
        getline(cin, user.username);

        if (user.username.empty()) {
            system("cls");
            return;
        }
        else if (usernameExists(user.username)) {
            cout << "\n  Username already exists. Please choose another.\n";
        }
        else {
            cout << "  Enter your Full Name: ";
            getline(cin, user.full_name);
            cout << "  Enter a Password (at least 8 characters): ";
            getline(cin, user.password);
            if (user.password.length() < 8) {
                cout << "  Try Again! Password must be at least 8 characters long.\n";
            }
            else {
                hashPassword(user);

                // Initialize other user details
                user.address = "";
                user.email = "";
                user.contact_num = "";
                user.balance = 0.0;

                ofstream userFile("users.txt", ios::app);
                if (userFile.is_open()) {
                    userFile << "Username: " << user.username << endl
                        << "Full Name: " << user.full_name << endl
                        << "Hashed Password: " << user.hashed_password << endl
                        << "Address: " << user.address << endl
                        << "Email: " << user.email << endl
                        << "Contact Number: " << user.contact_num << endl
                        << "Balance: Php " << user.balance << endl << endl;
                    userFile.close();
                    system("cls");
                    cout << "\n  You have successfully created your account!\n";
                    break;
                }
                else {

                    system("cls");
                    cout << "  Unable to open file";
                    return;
                }
            }
        }
    }
}

// User Login
void user_login() {

    system("cls");
    logo();
    cout << "\n  LOGIN\n\n";
    string username, password;
    cout << "  Enter your Username: ";
    getline(cin, username);
    cout << "  Enter your Password: ";
    getline(cin, password);

    ifstream userFile("users.txt");
    if (userFile.is_open()) {
        string line;
        bool found = false;
        User currentUser;

        while (getline(userFile, line)) {
            if (line.find("Username: " + username) != string::npos) {
                currentUser.username = username;

                getline(userFile, line);
                currentUser.full_name = line.substr(line.find(": ") + 2);

                getline(userFile, line);
                currentUser.hashed_password = line.substr(line.find(": ") + 2);

                getline(userFile, line);
                currentUser.address = line.substr(line.find(": ") + 2);

                getline(userFile, line);
                currentUser.email = line.substr(line.find(": ") + 2);

                getline(userFile, line);
                currentUser.contact_num = line.substr(line.find(": ") + 2);

                // Read the balance line
                if (getline(userFile, line)) {
                    // Find "Balance: Php " and extract the value after it
                    size_t pos = line.find("Balance: Php ");
                    if (pos != string::npos) {
                        string balanceStr = line.substr(pos + 13); // Extract AFTER "Balance: Php "

                        // Trim leading/trailing whitespace
                        balanceStr.erase(balanceStr.begin(), std::find_if_not(balanceStr.begin(), balanceStr.end(), ::isspace));
                        balanceStr.erase(std::find_if_not(balanceStr.rbegin(), balanceStr.rend(), ::isspace).base(), balanceStr.end());

                        // Now try converting to a number
                        try {
                            currentUser.balance = stof(balanceStr);
                        }
                        catch (const std::invalid_argument& ia) {
                            cerr << "\n  Error reading balance: Invalid balance format in users.txt\n";
                            userFile.close();
                            return;
                        }
                        catch (const std::out_of_range& oor) {
                            cerr << "\n  Error reading balance: Balance value out of range in users.txt\n";
                            userFile.close();
                            return;
                        }
                    }
                    else {
                        cerr << "\n  Error reading balance from file: 'Balance: Php ' not found.\n";
                        userFile.close();
                        return;
                    }
                }
                else {
                    cerr << "\n  Error reading balance from file: Could not read the next line.\n";
                    userFile.close();
                    return;
                }

                if (verifyPassword(currentUser, password)) {
                    found = true;
                    system("cls");
                    cout << "\n  Login successful!\n";
                    logged_in_menu(currentUser);
                    break;
                }
            }
        }

        userFile.close();

        if (!found) {
            system("cls");
            cout << "\n  Invalid username or password. Please try again.\n";
        }
    }
    else {
        system("cls");
        cerr << "\n  Unable to open users.txt\n";
    }
}

Car construct_car(string brand, string model, int year, int price, string fuel, string type, int seats, string info) {

    Car x;

    x.brand = brand;
    x.model = model;
    x.year = year;
    x.price = price;
    x.fuel = fuel;
    x.type = type;
    x.seats = seats;
    x.info = info;

    return x;
}

// Sorting Functions
void swap(Car& a, Car& b) {
    Car temp = a;
    a = b;
    b = temp;
}

void sort_by_year(Car arr[], int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            if (arr[j].year < arr[j + 1].year) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void sort_by_price(Car arr[], int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            if (arr[j].price < arr[j + 1].price) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void sortCars(Car arr[], int arr_size) {
    cout << "\n  Sort by:\n";
    cout << "  1. Year\n";
    cout << "  2. Price\n";
    cout << "  Enter your choice: ";

    int sortChoice;
    cin >> sortChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (sortChoice) {
    case 1:
        sort_by_year(arr, arr_size);
        cout << "  Sorted by year.\n";
        break;
    case 2:
        sort_by_price(arr, arr_size);
        cout << "  Sorted by price.\n";
        break;
    default:
        cout << "  Invalid choice. Cars not sorted.\n";
    }
}

// Filtering Functions
filtered_array filter_by_year(Car arr[], int year, int arr_size) {

    int new_arr_size = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].year >= year) {
            new_arr_size++;
        }
    }

    filtered_array return_array = filtered_array(new_arr_size);
    int index = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].year >= year) {
            return_array.data[index] = arr[i];
            index++;
        }
    }

    return return_array;
}

filtered_array filter_by_seats(Car arr[], int seats, int arr_size) {

    int new_arr_size = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].seats == seats) {
            new_arr_size++;
        }
    }

    filtered_array return_array = filtered_array(new_arr_size);
    int index = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].seats == seats) {
            return_array.data[index] = arr[i];
            index++;
        }
    }

    return return_array;
}

filtered_array filter_by_price(Car arr[], int budget, int arr_size) {

    int new_arr_size = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].price <= budget) {
            new_arr_size++;
        }
    }

    filtered_array return_array = filtered_array(new_arr_size);
    int index = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].price <= budget) {
            return_array.data[index] = arr[i];
            index++;
        }
    }

    return return_array;
}

filtered_array filter_by_fuel(Car arr[], string fuel, int arr_size) {

    int new_arr_size = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].fuel == fuel) {
            new_arr_size++;
        }
    }

    filtered_array return_array = filtered_array(new_arr_size);
    int index = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].fuel == fuel) {
            return_array.data[index] = arr[i];
            index++;
        }
    }

    return return_array;
}

filtered_array filter_by_gear(Car arr[], string gear, int arr_size) {

    int new_arr_size = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].type == gear) {
            new_arr_size++;
        }
    }

    filtered_array return_array = filtered_array(new_arr_size);
    int index = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i].type == gear) {
            return_array.data[index] = arr[i];
            index++;
        }
    }

    return return_array;
}

filtered_array total_filter(int year, int price, string fuel, string gear, int seats, Car arr[], int arr_size) {


    filtered_array filtered_1 = filter_by_year(arr, year, arr_size);
    filtered_array filtered_2 = filter_by_price(filtered_1.data, price, filtered_1.size);
    filtered_array filtered_3 = filter_by_fuel(filtered_2.data, fuel, filtered_2.size);
    filtered_array filtered_4 = filter_by_gear(filtered_3.data, gear, filtered_3.size);
    filtered_array filtered_5 = filter_by_seats(filtered_4.data, seats, filtered_4.size);

    return filtered_5;
};

// CSV File Handling Functions
vector <string> split(const string& str, char delimiter) {

    vector<string> traits;
    stringstream stream(str);
    string token;

    while (getline(stream, token, delimiter)) {
        traits.push_back(token);
    }

    return traits;
}

void load_csv(Car arr[]) {


    ifstream read_file("Cars_Data.txt");
    string myText;
    const char delimiter = ',';

    int index = 0;

    while (getline(read_file, myText)) {

        vector<string> tokenized_traits = split(myText, delimiter);

        if (tokenized_traits.size() != 8) {
            continue;
        }

        arr[index] = construct_car(tokenized_traits[0], tokenized_traits[1], stoi(tokenized_traits[2]), stoi(tokenized_traits[3]), tokenized_traits[4], tokenized_traits[5], stoi(tokenized_traits[6]), tokenized_traits[7]);
        index++;
    }

    read_file.close();
}

void overwrite_to_csv(Car arr[], int arr_size) {

    ofstream outFile("Cars_Data.txt");

    for (int i = 0; i < arr_size; i++) {

        outFile << arr[i].brand << ","
            << arr[i].model << ","
            << arr[i].year << ","
            << arr[i].price << ","
            << arr[i].fuel << ","
            << arr[i].type << ","
            << arr[i].seats << ","
            << arr[i].info << "\n";
    }

    outFile.close();
}

int count_csv() {

    ifstream read_file("Cars_Data.txt");

    int count = 0;
    string sample;
    while (getline(read_file, sample)) {
        count++;
    }

    return count;
}

void add_to_csv(string brand, string model, int year, int price, string fuel, string type, int seats, string info) {

    ofstream read_file("Cars_Data.txt", ios::app);

    if (!read_file) {
        cout << "No such file found";
        return;
    }
    else {
        read_file << brand << ","
            << model << ","
            << year << ","
            << price << ","
            << fuel << ","
            << type << ","
            << seats << ","
            << info << "\n";

        cout << "Data appended successfully\n";
        read_file.close();
    }
}

void delete_in_csv(int target_index, Car arr[], int arr_size) {

    filtered_array final_arr = filtered_array(arr_size - 1);

    int curr_index = 0;
    for (int i = 0; i < arr_size; i++) {

        if (i == target_index) {
            continue;
        }

        else {
            final_arr.data[curr_index] = arr[i];
            curr_index++;
        }
    }

    overwrite_to_csv(final_arr.data, final_arr.size);
}

// Car Management Functions (Admin)
void add_car(Car arr[], int arr_size) {

    int year, choiceSeats, price, seats;
    string fuelType, transmission;
    string fuel, type, brand, model, info;

    cout << "Add a Car" << endl;
    cout << "-------------------------" << endl;


    //brand
    while (true) {
        cout << "Enter the car's brand (or 0 to exit): ";
        getline(cin, brand);

        if (brand == "0") {
            cout << "Exiting adding car.\n";
            return;
        }
        else if (brand.empty()) {
            cout << "Please enter a valid car brand.\n";
        }
        else {
            break;
        }
    }//brand


    //model
    while (true) {
        cout << "Enter the car's model (or 0 to exit): ";
        getline(cin, model);

        if (model == "0") {
            cout << "Exiting adding car.\n";
            return;
        }
        else if (model.empty()) {
            cout << "Please enter a valid car model.\n";
        }
        else {
            break;
        }
    }


    //year
    while (true) {
        cout << "Enter the year of manufacture: ";
        if (cin >> year) {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid year.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    //price
    while (true) {
        cout << "Enter the car's price: ";
        if (cin >> price) {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid price.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    //fuel
    while (true) {
        cout << "1. Gas" << endl << "2. Electric" << endl << "Enter the car's fuel type: ";
        if (cin >> fuelType) {
            if (fuelType == "1") {
                fuel = "Gas";
                break;
            }
            else if (fuelType == "2") {
                fuel = "Electric";
                break;
            }
            else {
                cout << "Please input a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    //gear
    while (true) {
        cout << "1. Manual" << endl << "2. Automatic" << endl << "Enter preferred gear type: ";
        if (cin >> transmission) {
            if (transmission == "1") {
                type = "Manual";
                break;
            }
            else if (transmission == "2") {
                type = "Automatic";
                break;
            }
            else {
                cout << "Please input a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    //seats

    while (true) {
        cout << "1. 4 seats" << endl << "2. 7 seats" << endl << "3. 9 seats" << endl << "Enter preferred seat count: ";
        if (cin >> choiceSeats) {
            if (choiceSeats == 1) {
                seats = 4;
                break;
            }
            else if (choiceSeats == 2) {
                seats = 7;
                break;
            }
            else if (choiceSeats == 3) {
                seats = 9;
                break;
            }
            else {
                cout << "Please input a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter a value from 1 to 3.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }


    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true) {
        cout << "Describe the car in 1-2 sentences without commas:" << endl;
        getline(cin, info);

        if (info.find(',') != string::npos) {
            cout << "Please retry without commas." << endl;
            continue;
        }

        else if (info == "") {
            cout << "Please provide an input." << endl;
            continue;
        }

        else {
            break;
        }
    }

    add_to_csv(brand, model, year, price, fuel, type, seats, info);
}


void show_cars_for_deletion(Car arr[], int arr_size) {
    system("cls");
    if (arr_size == 0) {
        cout << "No cars in the inventory.\n";
        return;
    }

    system("cls");

    for (int i = 0; i < arr_size; i++) {

        cout << "\n======================================================================\n" << endl;

        cout << i + 1 << ". " << arr[i].brand << " "
            << arr[i].model << " (" << arr[i].year << ") : "
            << "Php " << arr[i].price << "\n";
        cout << "Fuel: " << arr[i].fuel << endl;
        cout << "Min seats: " << arr[i].seats << endl;
        cout << "Gear: " << arr[i].type << endl;
        cout << "Info: " << arr[i].info << endl << endl;
    }

    int choice;
    while (true) {
        cout << "Enter the number of the car to delete (or 0 to go back): ";
        if (cin >> choice && choice >= 0 && choice <= arr_size) {
            break;
        }

        else if (choice == 0) {
            system("cls");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        else {
            cout << "Invalid input. Please enter a valid choice.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if (choice > 0) {
        delete_in_csv(choice - 1, arr, arr_size);
        cout << "Car successfully deleted.\n";
    }
}

void delete_car() {
    filtered_array warehouse = filtered_array(count_csv());
    load_csv(warehouse.data);

    sort_by_year(warehouse.data, warehouse.size);

    show_cars_for_deletion(warehouse.data, warehouse.size);
}

void admin_menu() {
    filtered_array warehouse = filtered_array(count_csv());
    load_csv(warehouse.data);

    while (true) {

        logo();
        cout << "==================================\n";
        cout << "||                              ||\n";
        cout << "||  Admin Settings              ||\n";
        cout << "||                              ||\n";
        cout << "||  1. Add a car                ||\n";
        cout << "||  2. Delete a car             ||\n";
        cout << "||  3. Log out                  ||\n";
        cout << "||                              ||\n";
        cout << "==================================\n\n";


        cout << "Enter your choice: ";

        int choice;
        if (cin >> choice && choice >= 1 && choice <= 3) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

            switch (choice) {
            case 1:
                add_car(warehouse.data, warehouse.size);
                break;
            case 2:
                delete_car();
                break;
            case 3:
                system("cls");
                cout << "Logging out...\n";
                return;
            }
        }
        else {
            system("cls");
            cout << "Invalid input. Please enter a number between 1 and 3.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void admin_login() {
    string name, pass;

    while (true) {
        cout << "Input the admin username (or 0 to exit): ";
        getline(cin, name); // Use getline to read the entire line

        if (name == "0") {
            system("cls");
            cout << "Exiting admin login.\n";
            return; // Go back to the main menu
        }

        cout << "Input the admin access code: ";
        getline(cin, pass); // Use getline to read the entire line

        if (name == ADMIN_USERNAME && pass == ADMIN_PASSWORD) {
            system("cls");
            admin_menu();
            break; // Exit the login loop if successful
        }
        else {

            system("cls");
            cout << "Invalid admin username or access code. \n";
        }
    }
}


// Car Recommendation Function
filtered_array search_cars(Car arr[], int arr_size) {

    int year, choiceSeats, fuelType, transmission, price, seats;
    string fuel, type;
    system("cls");
    logo();;
    cout << "\nCar Recommendation System" << endl;
    cout << "-----------------------------------------" << endl;

    while (true) {
        cout << "Enter the minimum year of manufacture: ";
        if (cin >> year) {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid year.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter your desired budget: ";
        if (cin >> price) {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid budget.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "1. Gas" << endl << "2. Electric" << endl << "Enter preferred fuel type: ";
        if (cin >> fuelType) {
            if (fuelType == 1) {
                fuel = "Gas";
                break;
            }
            else if (fuelType == 2) {
                fuel = "Electric";
                break;
            }
            else {
                cout << "Please input a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "1. Manual" << endl << "2. Automatic" << endl << "Enter preferred gear type: ";
        if (cin >> transmission) {
            if (transmission == 1) {
                type = "Manual";
                break;
            }
            else if (transmission == 2) {
                type = "Automatic";
                break;
            }
            else {
                cout << "Please input a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "1. 4" << endl << "2. 7" << endl << "3. 9" << endl << "Enter preferred seat count: ";
        if (cin >> choiceSeats) {
            if (choiceSeats == 1) {
                seats = 4;
                break;
            }
            else if (choiceSeats == 2) {
                seats = 7;
                break;
            }
            else if (choiceSeats == 3) {
                seats = 9;
                break;
            }
            else {
                cout << "Please input a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter a number from 1 to 3.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }


    filtered_array result = total_filter(year, price, fuel, type, seats, arr, arr_size);

    return result;
}

void recommend_car(User& currentUser) {
    while (true) { // Add a loop here!

        int warehouse_size = count_csv();
        filtered_array warehouse = filtered_array(warehouse_size);
        load_csv(warehouse.data);

        filtered_array result = search_cars(warehouse.data, warehouse.size);

        sort_by_year(result.data, result.size);

        show_cars(result.data, result.size, currentUser);

        cout << endl;

        // Ask the user if they want to make another recommendation
        cout << "Do you want to try another recommendation? (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(choice) != 'y') {
            system("cls");
            break; // Exit the loop if the user enters anything other than 'y'
        }
    }
}

// User Account Functions
void updateUserFile(const User& user) {
    ifstream userFile("users.txt");
    ofstream tempFile("temp.txt");

    if (!userFile.is_open() || !tempFile.is_open()) {
        cout << "\tError opening file(s) for updating user details!\n";
        return;
    }

    string line;
    while (getline(userFile, line)) {
        if (line.find("Username: " + user.username) != string::npos) {
            tempFile << "Username: " << user.username << endl;
            tempFile << "Full Name: " << user.full_name << endl;
            tempFile << "Hashed Password: " << user.hashed_password << endl;
            tempFile << "Address: " << user.address << endl;
            tempFile << "Email: " << user.email << endl;
            tempFile << "Contact Number: " << user.contact_num << endl;
            tempFile << "Balance: Php " << user.balance << endl << endl;

            // Skip the next 6 lines (other user details)
            for (int i = 0; i < 6; ++i) {
                getline(userFile, line);
            }
        }
        else {
            tempFile << line << endl;
        }
    }

    userFile.close();
    tempFile.close();

    // Now copy the content of temp.txt to users.txt
    ifstream tempRead("temp.txt");
    ofstream userWrite("users.txt");

    if (!tempRead.is_open() || !userWrite.is_open()) {
        cout << "  Error opening files for copying!\n";
        return;
    }

    userWrite << tempRead.rdbuf();

    tempRead.close();
    userWrite.close();

    // Remove the temporary file
    remove("temp.txt");
}

void addRating(const Rating& rating) {
    ofstream ratingsFile("ratings.txt", ios::app);
    if (ratingsFile.is_open()) {
        ratingsFile << rating.username << ","
            << rating.carBrand << ","
            << rating.carModel << ","
            << rating.stars << ","
            << rating.comment << endl;
        ratingsFile.close();
        system("cls");
        cout << "  Rating added successfully!\n";
    }
    else {
        cout << "  Unable to open ratings file.\n";
    }
}

void rateCar(const User& currentUser, const Car& car) {
    Rating rating;
    rating.username = currentUser.username;
    rating.carBrand = car.brand;
    rating.carModel = car.model;

    cout << "\n  Rate this car (1-5 stars): ";
    while (!(cin >> rating.stars) || rating.stars < 1 || rating.stars > 5) {
        cout << "  Invalid input. Please enter a rating between 1 and 5: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Add a comment (optional, press Enter to skip): ";
    getline(cin, rating.comment);

    addRating(rating);
}

void displayRatings() {
    ifstream ratingsFile("ratings.txt");
    vector<Rating> ratings;

    if (ratingsFile.is_open()) {
        string line;
        while (getline(ratingsFile, line)) {
            stringstream ss(line);
            string item;
            vector<string> tokens;

            // Extract tokens separated by commas
            while (getline(ss, item, ',')) {
                tokens.push_back(item);
            }

            // Ensure each line has at least the required number of tokens
            if (tokens.size() >= 4) {
                Rating rating;
                rating.username = tokens[0];
                rating.carBrand = tokens[1];
                rating.carModel = tokens[2];
                rating.stars = stoi(tokens[3]);

                // Handle optional comment
                if (tokens.size() == 5) {
                    rating.comment = tokens[4];
                }
                else {
                    rating.comment = ""; // Set an empty comment if not provided
                }

                ratings.push_back(rating);
            }
            else {
                cout << "  Invalid rating format encountered. Skipping...\n";
            }
        }
        ratingsFile.close();

        // Sort ratings by stars (descending order)
        sort(ratings.begin(), ratings.end(), [](const Rating& a, const Rating& b) {
            return a.stars > b.stars;
            });
        system("cls");
        cout << "\n  RATINGS:\n";
        cout << "----------------------------------------------------\n";
        if (ratings.empty()) {
            cout << "  No ratings yet.\n";
        }
        else {
            for (const Rating& r : ratings) {
                cout << "  User: " << r.username << endl;
                cout << "  Car: " << r.carBrand << " " << r.carModel << endl;
                cout << "  Rating: " << r.stars << " stars" << endl;

                // Display comment only if it's not empty
                if (!r.comment.empty()) {
                    cout << "  Comment: " << r.comment << endl;
                }
                cout << "\n";
            }
        }
    }
    else {
        cout << "  Unable to open ratings file.\n";
    }
    cout << "----------------------------------------------------\n";
}

void saveTransaction(const Transaction& transaction) {
    ofstream transactionsFile("transactions.txt", ios::app);
    if (transactionsFile.is_open()) {
        transactionsFile << transaction.transactionID << ","
            << transaction.name << ","
            << transaction.carBrand << ","
            << transaction.carModel << ","
            << transaction.carPrice << ","
            << transaction.timestamp << ","
            << transaction.carYear << endl; // Add car year to output
        transactionsFile.close();
    }
    else {
        cout << "  No Transaction Found.\n";
    }
}

string generateTransactionID() {
    // Generate a simple unique transaction ID (you can customize this)
    auto now = chrono::system_clock::now();
    auto millis = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());
    return to_string(millis.count()) + "-" + to_string(rand() % 1000);
}

// Function to get formatted timestamp string
string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t tt = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &tt); // Use localtime_s for Windows

    stringstream ss;
    ss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void displayReceipt(const Transaction& transaction) {
    // Display the receipt in a formatted way

    cout << "  \n  RECEIPT\n";
    cout << "  ----------------------------------\n";
    cout << "  Transaction ID: " << transaction.transactionID << endl;
    cout << "  Date: " << transaction.timestamp << endl;
    cout << "  ----------------------------------\n";
    cout << "  Buyer: " << transaction.name << endl;
    cout << "  Brand: " << transaction.carBrand << " " << endl;
    cout << "  Model: " << transaction.carModel << " " << endl;
    cout << "  Year:  " << transaction.carYear << " " << endl;
    cout << "  Price: Php " << transaction.carPrice << endl;
    cout << "  ----------------------------------\n";
}

void buy_car(User& currentUser, Car car) {
    if (currentUser.balance >= car.price) {
        // Deduct car price from user balance
        currentUser.balance -= car.price;

        // Update user file with new balance
        updateUserFile(currentUser);

        string transactionID = generateTransactionID();

        Transaction transaction;
        transaction.transactionID = transactionID;
        transaction.name = currentUser.full_name;
        transaction.carBrand = car.brand;
        transaction.carModel = car.model;
        transaction.carPrice = car.price;
        transaction.carYear = car.year;

        transaction.timestamp = getCurrentTimestamp();
        saveTransaction(transaction);

        displayReceipt(transaction);
        cout << "\n  Congratulations! You have purchased a " << car.brand << " " << car.model << ".\n";
        cout << "  Your new balance is: " << "Php " << currentUser.balance << "\n";

        cout << "\n  Would you like to rate this car? (y/n): ";
        char ratingChoice;
        cin >> ratingChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (tolower(ratingChoice) == 'y') {
            rateCar(currentUser, car);
        }
    }
    else {
        system("cls");
        cout << "\n  Insufficient balance. Please top up your account.\n";
    }
}

void show_cars(Car arr[], int arr_size, User& currentUser) {
    system("cls");
    if (arr_size == 0) {
        cout << "\nNo cars match the specified criteria.\n";
        return;
    }

    for (int i = 0; i < arr_size; i++) {


        cout << "\n======================================================================\n" << endl;

        cout << i + 1 << ". " << arr[i].brand << " " << arr[i].model << " (" << arr[i].year << ") : " << "Php " << arr[i].price << "\n";
        cout << "Fuel: " << arr[i].fuel << endl;
        cout << "Min seats: " << arr[i].seats << endl;
        cout << "Gear: " << arr[i].type << endl << endl;
        cout << "Info: \n\n";
        cout << arr[i].info << endl << endl;
    }

    int choice;
    while (true) {
        cout << "Enter the number of the car you want to buy (or 0 to go back): ";
        if (cin >> choice) {
            if (choice == 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                system("cls");
                return; // Go back correctly
            }
            else if (choice > 0 && choice <= arr_size) {
                buy_car(currentUser, arr[choice - 1]);
                return; // Exit after buying a car
            }
            else {
                cout << "Invalid input. Please enter a valid choice.\n";
            }
        }
        else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}



void show_all_cars(User& currentUser) {
    int warehouse_size = count_csv();
    filtered_array warehouse = filtered_array(warehouse_size);
    load_csv(warehouse.data);

    cout << "  Do you want to sort the cars? (y/n): ";
    char sortChoice;
    cin >> sortChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(sortChoice) == 'y') {
        sortCars(warehouse.data, warehouse.size);
    }

    system("cls");
    show_cars(warehouse.data, warehouse.size, currentUser);
};

void edit_personal_details(User& currentUser) {
    system("cls");

    logo();

    cout << "\n  EDIT PERSONAL DETAILS\n";
    cout << "---------------------------------------" << endl;
    cout << "  Leave fields blank if you don't want to change them.\n\n";

    string newName, newAddress, newEmail, newContact;

    cout << "  Current Full Name: " << currentUser.full_name << endl;
    cout << "  Enter new Full Name: ";
    getline(cin, newName);

    cout << "  Current Address: " << currentUser.address << endl;
    cout << "  Enter new Address: ";
    getline(cin, newAddress);

    cout << "  Current Email: " << currentUser.email << endl;
    cout << "  Enter new Email: ";
    getline(cin, newEmail);

    cout << "  Current Contact Number: " << currentUser.contact_num << endl;
    cout << "  Enter new Contact Number: ";
    getline(cin, newContact);

    if (!newName.empty()) {
        currentUser.full_name = newName;
    }
    if (!newAddress.empty()) {
        currentUser.address = newAddress;
    }
    if (!newEmail.empty()) {
        currentUser.email = newEmail;
    }
    if (!newContact.empty()) {
        currentUser.contact_num = newContact;
    }
    updateUserFile(currentUser);
    system("cls");
    cout << "  Personal details updated successfully!\n";
}

void personal_details(User& currentUser) {
    system("cls");
    logo();
    cout << endl;
    cout << "\n  PERSONAL DETAILS\n";
    cout << "---------------------------------------" << endl;
    cout << "  Username: " << currentUser.username << endl;
    cout << "  Full Name: " << currentUser.full_name << endl;
    cout << "  Address: " << currentUser.address << endl;
    cout << "  Email: " << currentUser.email << endl;
    cout << "  Contact Number: " << currentUser.contact_num << endl;
    cout << "  Balance: Php " << currentUser.balance << endl;

    cout << "\n  Do you want to edit your details? (y/n): ";
    char editChoice;
    cin >> editChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    system("cls");

    if (tolower(editChoice) == 'y') {
        edit_personal_details(currentUser);
    }
}

void balance(const User& currentUser) {
    cout << "\n  BALANCE\n";
    cout << "  Current Balance: Php " << currentUser.balance << endl;

}
void top_up(User& currentUser) {
    system("cls");
    logo();
    cout << "\n  TOP UP\n";

    long long amount;
    cout << "  Enter top-up amount (or 0 to cancel): Php ";

    while (true) {
        if (cin >> amount) { // Try to read the input as a number
            if (amount == 0) {
                system("cls");
                cout << "  Top-up canceled.\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
                return; // Exit the function
            }
            else if (amount > 0) {
                currentUser.balance += amount;
                updateUserFile(currentUser);
                system("cls");
                cout << "  Top-up successful.\nNew balance: Php " << currentUser.balance << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
                return; // Exit the function
            }
            else {
                system("cls");
                cout << "  Invalid input. Please enter a non-negative number: Php ";
                cin.clear(); // Clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            }
        }
        else {
            system("cls");
            cout << "  Invalid input. Please enter a non-negative number: Php ";
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
        }
    }
}

void balance(User& currentUser)
{ // Take currentUser by reference
    system("cls");
    cout << "\n  BALANCE\n";
    cout << "  Current Balance: Php " << currentUser.balance << endl;

    top_up(currentUser); // Directly call top_up function
}

void history_receipts(const User& currentUser) {
    system("cls");
    cout << "\n  HISTORY/RECEIPTS\n";
    ifstream transactionsFile("transactions.txt");

    if (transactionsFile.is_open()) {
        string line;
        bool found = false;

        while (getline(transactionsFile, line)) {
            stringstream ss(line);
            string item;
            vector<string> tokens; // Define tokens here, inside the loop

            while (getline(ss, item, ',')) {
                tokens.push_back(item);
            }

            if (tokens.size() == 7 && tokens[1] == currentUser.full_name) {
                found = true;

                Transaction transaction = {
                    tokens[0], tokens[1], tokens[2], tokens[3],
                    stoi(tokens[4]), tokens[5], stoi(tokens[6]) // Include carYear 
                };
                displayReceipt(transaction); // Pass only the transaction object 
            }
        }

        if (!found) {
            system("cls");
            cout << "  No transaction history found.\n";
        }

        transactionsFile.close();
    }
    else {
        system("cls");
        cout << "  Unable to open transactions file.\n";
    }
}

bool LoggedValidChoice(int choices) {
    return choices >= 1 && choices <= 6;
}

// Function to get and validate user choice
int getLoggedValidChoice() {
    string input;
    int choices;

    logo();

    while (true) {

        cout << "==================================";
        cout << "\n||\t\t\t\t\||";
        cout << "\n||\tWelcome to AutoPilot!\t||";
        cout << "\n||\t\t\t\t||\n";
        cout << "||\t1. Available Cars\t||\n";
        cout << "||\t2. Recommended Cars\t||\n";
        cout << "||\t3. Personal Details\t||\n";
        cout << "||\t4. Top up\t\t||\n";
        cout << "||\t5. History/Receipts\t||\n";
        cout << "||\t6. Log out\t\t||\n";
        cout << "||\t\t\t\t||";
        cout << "\n==================================\n\n";
        cout << "\tEnter choice: ";
        getline(cin, input);

        stringstream ss(input);
        if (ss >> choices && ss.eof() && LoggedValidChoice(choices)) {
            return choices;
        }
        else {
            cout << "\t\t\tInvalid input. Please enter a number between 1 and 6." << endl;
        }
    }
}

void logged_in_menu(User& currentUser) {
    while (true) {
        int choice = getLoggedValidChoice();

        switch (choice) {
        case 1:
            show_all_cars(currentUser);
            continue;
        case 2:
            recommend_car(currentUser);
            continue;
        case 3:
            personal_details(currentUser);
            break;
        case 4:
            balance(currentUser);
            break;
        case 5:
            history_receipts(currentUser);
            break;
        case 6:

            system("cls");
            cout << "  Logging out...\n";
            cout << "  Logged out successfully. See you next time!\n";
            return; // Return to the main menu
        default:
            cout << "  Invalid choice. Please select a number from 1 to 6.\n";
            break;
        }
    }
}

bool isValidChoice(int choice) {
    return choice >= 1 && choice <= 5; // Updated validation for main menu (1-5)
}

int getValidChoice() {
    string input;
    int choice;

    logo();

    while (true) {
        cout << "==================================\n";
        cout << "||                              ||\n";
        cout << "||     Welcome to Autopilot!    ||\n";
        cout << "||                              ||\n";
        cout << "||  1. Log in                   ||\n";
        cout << "||  2. Sign up                  ||\n";
        cout << "||  3. Admin settings           ||\n";
        cout << "||  4. Ratings                  ||\n";
        cout << "||  5. Exit                     ||\n";
        cout << "||                              ||\n";
        cout << "==================================\n\n";

        cout << "  Enter choice: ";

        getline(cin, input);
        stringstream ss(input);

        if (ss >> choice && ss.eof() && isValidChoice(choice)) {
            return choice;
        }
        else {
            system("cls");
            cout << "  Invalid input. Please enter a number between 1 and 5." << endl;

        }
    }
}

