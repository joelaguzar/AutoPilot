# 🚗 AutoPilot: Personalized Picks for Your Perfect Ride

This C++ program implements a comprehensive console-based car dealership management system, catering to both users and administrators. The system provides a robust set of features for browsing available cars, receiving recommendations, purchasing cars, managing user accounts, and viewing transaction history. Additionally, administrators have specialized tools for managing the car inventory.

## Features

### User Features

#### 📝 Registration and Login
- **Secure User Registration**: New users can create accounts with unique usernames and secure, hashed passwords.
- **User Login**: Existing users can log in using their credentials. Passwords are validated against hashed entries for security.

#### 🚗 Browsing and Purchasing
- **Browse Available Cars**: Users can view all cars in the inventory, sorted by various criteria such as year or price.
- **Car Recommendation System**: Personalized recommendations based on user preferences including year, budget, fuel type, transmission, and number of seats.
- **Purchase Car**: Users can purchase cars, receiving a detailed receipt upon completion. The system automatically deducts the purchase price from the user's balance.

#### 👤 Account Management
- **View and Edit Personal Details**: Users can update their full name, address, email, and contact number.
- **View Current Balance**: Users can check their current account balance.
- **Top-up Balance**: Users can add funds to their account balance.
- **Transaction History**: Users can view a detailed history of their transactions, including receipts.

#### ⭐ Car Ratings and Reviews
- **Rate and Review Cars**: Users can rate purchased cars on a scale of 1-5 stars and leave optional comments.
- **View Ratings**: The system displays all ratings and reviews, sorted by star rating to provide comprehensive feedback for all users.

### Admin Features

#### 🔐 Admin Login
- **Secure Admin Login**: Administrators can log in using a predefined username and password for secure access.

#### 🛠️ Car Inventory Management
- **Add New Cars**: Admins can add new cars to the inventory, including details like brand, model, year, price, fuel type, transmission, seats, and a short description.
- **Delete Cars**: Admins can remove cars from the inventory.

## Program Structure and Files

### Main Program File
- **main.cpp**: Contains the main function, user interface elements, and the primary control flow of the program.

### Data Storage Files
- **users.txt**: Stores user data including username, hashed password, full name, address, email, contact number, and balance.
- **Cars_Data.txt**: Stores car inventory data including brand, model, year, price, fuel type, transmission, seats, and additional info.
- **ratings.txt**: Stores user ratings and reviews for cars.
- **transactions.txt**: Records details of every car purchase including transaction ID, buyer name, car details, price, and timestamp.

## Data Structures

### ⭐ Rating
- **Attributes**: UserID, CarID, Rating, Comment.

### 🚘 Car
- **Attributes**: Brand, Model, Year, Price, FuelType, Transmission, Seats, Info.

### 👤 User
- **Attributes**: Username, HashedPassword, FullName, Address, Email, ContactNumber, Balance.

### 💳 Transaction
- **Attributes**: TransactionID, BuyerName, CarDetails, Price, Timestamp.

### 📊 filtered_array
- **Attributes**: Array of Car structures, Size of the array.

## Implementation Details

### 🔒 Security
- **Hashing**: User passwords are hashed using a custom hash function to ensure security.

### 📂 File Handling
- **File I/O Operations**: The program uses file input/output operations extensively to load, store, and update data in text files.

### 🔍 Filtering and Sorting
- **Sorting Functions**: Helps users find desired cars by sorting data based on various criteria.
- **Filtering Functions**: Users can filter cars based on specific preferences.

### ✅ Input Validation
- **Robust Validation**: The code includes input validation to prevent common errors and ensure data integrity.

## Future Improvements
### 📊 Database Integration
- **Database**: Replace text files with a database (e.g., SQLite, MySQL) for more efficient data management and scalability.
### 🖥️ Graphical User Interface (GUI)
- **GUI Development**: Develop a user-friendly GUI using libraries to enhance user experience.
### 🎯 Advanced Recommendation System
- **Sophisticated Engine**: Implement a more advanced recommendation engine considering user preferences and purchase history.
### 🔍 Search Functionality
- **Search Bar**: Add a search bar for quickly finding cars based on keywords.
### 🖼️ Image Support
- **Car Images**: Include images of cars in the inventory for a better user experience.
### 🔐 Enhanced Security
- **Secure Password Hashing**: Implement a more secure password hashing algorithm to improve password security.
### ⚙️ Efficient Sorting Algorithms
- **Optimized Sorting**: Replace existing sorting methods with more efficient algorithms like Quick Sort or Merge Sort to enhance performance and responsiveness.

##  <a id = "contrib"> 👷‍ Contributors </a> <br>

| Name | Role | E-mail | Other Contacts |
| --- | --- | --- | --- |
| <a href = "https://github.com/joelaguzar">Joel Lazernie A. Aguzar</a> | Developer | 23-00562@g.batstate-u.edu.ph |   |
| <a href = "https://github.com/AintKiel">Kriztel Garcia </a>|  Developer  |  |  |
| <a href = "https://github.com/Brian-Kristofer-Perez">Brian Kristofer Perez </a>| Developer |  | |
| <a href = "https://github.com/Kalel-StaTeresa">David Kalel Sta Teresa </a>| Developer | 23-00401@g.batstate-u.edu.ph | |

<br>

- Course:
  - CS 131 (Data Structures and Algorithms)
- Course Facilitator:
  - Fatima Marie Agdon
