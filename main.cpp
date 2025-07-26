#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const int MAX_USERS = 100;
class Transaction_manager;
class Portfolio;
class Stock {
private:
    string stock_name;
    string stock_symbol;
    double current_price;
    double previous_price;
    double volatility;
    double price_history[5]; // Store last 5 prices
    int history_count;

    static int total_stocks; // Static member to track total stocks

public:
    Stock() : stock_name(""), stock_symbol(""), current_price(0.0), previous_price(0.0), volatility(0.0), history_count(0) {
        for (int i = 0; i < 5; i++) {
        price_history[i] = 0.0;
        
        }
        total_stocks++;
    }

    Stock(string name, string symbol, double price, double prev_price, double vol) : stock_name(name), stock_symbol(symbol), current_price(price), previous_price(prev_price), volatility(vol), history_count(0) {
        price_history[0] = price;
        history_count = 1;
        total_stocks++;
    }

    // Destructor
    ~Stock() 
    { 
        total_stocks--; 
    }

    // Getters
    string get_stock_name() 
    { 
        return stock_name; 
    }
    string get_stock_symbol() 
    { 
        return stock_symbol; 
    }
    double get_current_price(){ 
        return current_price; 
    }
    double get_previous_price() 
    { 
        return previous_price; 
    }

    //random update in price
    void update_price() {
        previous_price = current_price; 
        double random_no = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        /* here if random between 1 and -1 then it will just change sign but what will volatility do here if volatillity is 0.1 then 
        it will change price by 10% of current price and if volatility is 0.2 then it will change price by 20% of current price for 
        example current price is 100$ then if random number is 1 and volatility is 0.1 then it will change price by 10% of current price
        so new price will be 110 and if random number is -1 and volatility is 0.1 then it will change price by -10% of current price so new price will be 90 
        why we are doing this because we want to change price by some random number and we want to change price by some percentage of current price so we are using this formula */

        double change = current_price * random_no * volatility; 
        current_price += change;
        // to make sure price does not go below 0.01
        if (current_price < 0.01) 
        {
        current_price = 0.01;
        }
        else
        {
            volatility = ((double)rand() / RAND_MAX) * 0.1 + 0.01; // volatility is between 0.01 and 0.1
        }

        // Update price history
        if (history_count < 5) {
            price_history[history_count++] = current_price; //post increment so first price will be at 0 at whih current assigned and then 1 and so on
        } else {
            for (int i = 0; i < 4; i++) {
                // here we are shifting prices to left so first price will be at 0 and then 1 and so on
                price_history[i] = price_history[i + 1];
            }
            // here we are assigning last price to 4th index
            price_history[4] = current_price;
        }
    }

    // Displaying price history that is last 5 prices in stock class
    void display_history() {
        cout << "|==================================================|" << endl;
        cout << "| " << left << setw(46) << ("Price History for " + stock_name + " (" + stock_symbol + "):") << " |" << endl;
        cout << "|--------------------------------------------------|" << endl;
        cout << "| " << left << setw(8) << "Refresh" << "| " << left << setw(15) << "Price" << "           |" << endl;
        cout << "|--------------------------------------------------|" << endl;
        for (int i = 0; i < history_count; i++) {
            cout << "| " << left << setw(8) << (i + 1) << "| $" << fixed << setprecision(2) << setw(14) << price_history[i] << "         |" << endl;
        }
        cout << "|==================================================|" << endl;
    }
    
    // to get the total stocks in market
    static int get_total_stocks() 
    {
         return total_stocks; 
    }

    // to Compare stock symbols
    bool operator == (Stock& other) {
        return stock_symbol == other.stock_symbol;
    }

    // Friend function for output streaming 
    friend ostream& operator<<(ostream& os, Stock& stock);
};

// static seted as 0
int Stock::total_stocks = 0;


ostream& operator<<(ostream& os, Stock& stock) {
    double price_change = stock.current_price - stock.previous_price;
    double percentage_change;
    if (stock.previous_price != 0){
        percentage_change = (price_change / stock.previous_price) * 100;
    } else if (stock.current_price != 0) {
        percentage_change = 100; // If previous price is 0 and current price is none then we consider it a 100% increase   
    } else {
        percentage_change = 0;
    }
// using this operator of stock class to display stock details

/*
  This function outputs the stock's name, symbol, current price, previous price, and the percentage change
  between the current and previous prices. The output is formatted using manipulators for alignment and precision.
*/

    os << left << setw(20) << stock.stock_name << setw(15) << stock.stock_symbol
       << setw(15) << fixed << setprecision(2) << stock.current_price
       << setw(15) << stock.previous_price << setw(13) << fixed << setprecision(2)
       << percentage_change << " %" ;
    return os;
}

// Market Class
class Market {
private:
    Stock* stocks;
    int stock_count;
    int capacity;

    void resize_array() {
        capacity *= 2;
        Stock* new_stocks = new Stock[capacity];
        for (int i = 0; i < stock_count; i++) {
            new_stocks[i] = stocks[i];
        }
        delete[] stocks;
        stocks = new_stocks;
    }

public:
    Market() : stock_count(0), capacity(10){ // default capacity 10
        stocks = new Stock[capacity];
        add_default_stocks();
    }
    ~Market() { delete[] stocks; }

    
    void add_stock(Stock s){
        if (stock_count >= capacity) resize_array();
        stocks[stock_count++] = s;
    }

    void delete_stock(string symbol) {
        int index =-1;
        for (int i = 0; i < stock_count; i++){
            if (stocks[i].get_stock_symbol() == symbol) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            for (int i = index; i < stock_count - 1; i++){ 
                stocks[i] = stocks[i + 1];
            }
            stock_count--;
            cout << "Stock with symbol '" << symbol << "' deleted.\n";
        } else {
            cout << "Stock with symbol '" << symbol << "' not found.\n";
        }
    }
    void add_default_stocks() {
        add_stock(Stock("Apple", "AAPL", 170.0, 170.0, 0.02));
        add_stock(Stock("Microsoft", "MSFT", 310.0, 310.0, 0.015));
        add_stock(Stock("Amazon", "AMZN", 3300.0, 3300.0, 0.025));
        add_stock(Stock("Tesla", "TSLA", 720.0, 720.0, 0.04));
        add_stock(Stock("Google", "GOOGL", 2800.0, 2800.0, 0.018));
        add_stock(Stock("Meta", "META", 340.0, 340.0, 0.022));
        add_stock(Stock("Nvidia", "NVDA", 900.0, 900.0, 0.03));
        add_stock(Stock("Netflix", "NFLX", 450.0, 450.0, 0.027));
        add_stock(Stock("Intel", "INTC", 40.0, 40.0, 0.012));
        add_stock(Stock("AMD", "AMD", 120.0, 120.0, 0.021));
    }

    Stock* find_stock_by_symbol(string symbol){
        for (int i = 0; i < stock_count; i++) {
            if (stocks[i].get_stock_symbol() == symbol) {
                return &stocks[i]; // to avoid copy
            }
        }
        return nullptr;
    }

    void update_all_prices() {
        for (int i = 0; i < stock_count; i++) stocks[i].update_price();
    }

    void display_all_stocks() {
        // Print table header with borders
        cout << "+--------------------+---------------+---------------+---------------+---------------+" << endl;
        cout << "| " << left << setw(19) << "Name"
             << "| " << left << setw(14) << "Symbol"
             << "| " << left << setw(14) << "Current Price"
             << "| " << left << setw(14) << "Previous Price"
             << "| " << left << setw(14) << "% Change"
             << "|" << endl;
        cout << "+--------------------+---------------+---------------+---------------+---------------+" << endl;
        // Print each stock row with borders
        for (int i = 0; i < stock_count; i++) {
            cout << "| ";
            cout << left << setw(19) << stocks[i].get_stock_name()
             << "| " << left << setw(14) << stocks[i].get_stock_symbol()
             << "| $" << left << setw(13) << fixed << setprecision(2) << stocks[i].get_current_price()
             << "| $" << left << setw(13) << fixed << setprecision(2) << stocks[i].get_previous_price();
            double price_change = stocks[i].get_current_price() - stocks[i].get_previous_price();
            double percentage_change = (stocks[i].get_previous_price() != 0)
            ? (price_change / stocks[i].get_previous_price()) * 100
            : (stocks[i].get_current_price() != 0 ? 100 : 0);
            cout << "| " << left << setw(13) << fixed << setprecision(2) << percentage_change << " %"
             << "|" << endl;
        }
        cout << "+--------------------+---------------+---------------+---------------+---------------+" << endl;
        cout << "| Total Stocks in Market: " << setw(54) << left << Stock::get_total_stocks() << "|" << endl;
        cout << "+-----------------------------------------------------------------------------------+" << endl;
    }
};

// Abstract User Class
class User {
protected:
    string name;
    int user_id;
    string password;
    double balance;
    int transaction_count;
public:
    User():name(""), user_id(0), password(""), balance(0.0), transaction_count(0) {}
    User(string n, int id, string pass) : name(n), user_id(id), password(pass), balance(5000.0), transaction_count(0){}
    virtual ~User() {}

    string get_name() 
    { 
        return name; 
    }
    int get_id() 
    { 
        return user_id; 
    }
    double get_balance() 
    {
         return balance; 
    }
    int get_transaction_count() 
    {
         return transaction_count; 
    }

    bool checkPassword(string pass) 
    {
         return password == pass; 
    }
    //if this si base class of any other class which inherited from this class user then as its is pure virtual function we can not use this function directly through its object
    virtual void deposit(double amount) = 0;
    virtual void display_user_info() = 0;

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void increment_transactions() { transaction_count++; }

    string getLevel()  {
        if (transaction_count <= 10) return "Level 1";
        else if (transaction_count <= 15) return "Level 2";
        else if (transaction_count <= 25) return "Level 3";
        else return "Level 4 Highest Level";
    }

    // Friend function for output streaming of User class
    friend ostream& operator<<(ostream& os,  User& user);
};

// inherited class of user class
class Regular_user : public User {
public:
    Regular_user(string n, int id, string pass) : User(n, id, pass){}
    void deposit(double amount) override {
        if (amount > 0) 
        balance += amount;
    }

    // this is pure virtual function of user class which is inherited from user class and will be implemented in this class only when created object of this class
    void display_user_info() override {
        cout << "|==================================================================|\n";
        cout << "|                        === User Info ===                         |\n";
        cout << "|==================================================================|\n";
        cout << "| User ID : " << setw(10) << left << user_id << "                                             |\n";
        cout << "| Name    : " << setw(25) << left << name << "                              |\n";
        cout << "| Demo Balance : $" << setw(12) << left << fixed << setprecision(2) << balance << "                                     |\n";
        cout << "| Level   : " << setw(25) << left << getLevel() << "                              |\n";
        cout << "|==================================================================|\n";
    }
};

// inherited class of user class
class Premium_user : public User {
private:
    double bonus_rate;

public:
    Premium_user(string n, int id, string pass, double rate = 0.05) : User(n, id, pass), bonus_rate(rate) {}

    void deposit(double amount) override {
        double bonus = amount * bonus_rate;
        balance += (amount + bonus);
    }

    void display_user_info()  override {
        cout << "|=====================================|\n";
        cout << "|       === Premium User Info ===     |\n";
        cout << "|=====================================|\n";
        cout << "| User ID : " << setw(10) << left << user_id << "                |\n";
        cout << "| Name    : " << setw(25) << left << name << " |\n";
        cout << "| Demo Balance : $" << setw(12) << left << fixed << setprecision(2) << balance << "        |\n";
        cout << "| Level   : " << setw(25) << left << getLevel() << " |\n";
        double premium_balance = balance * 1.05;
        cout << "| Premium Balance : $" << setw(10) << left << fixed << setprecision(2) << premium_balance << "       |\n";
        cout << "| Type   : Premium (Bonus Rate: " << setw(4) << left << (bonus_rate * 100) << "%)|\n";
        cout << "|=====================================|\n";
    }
    
};

// here using this operator as friend function  declared in user class to display user details ..
ostream& operator<<(ostream& os,  User& user) {
    user.display_user_info(); // Polymorphic call to display user info
    return os;
}

// this holding class is used to store the stock symbol, quantity and purchase price of the stock ...
class Holding {
public:
    string stock_symbol;
    int quantity;
    double purchase_price;

    Holding(string symbol = "", int qty = 0, double price = 0.0) : stock_symbol(symbol), quantity(qty), purchase_price(price){}
};

class Portfolio {
public:
    Holding* holdings;
    int holding_count;
    int capacity;

    void resize_array() {
        capacity *= 2;
        Holding* new_holdings = new Holding[capacity];
        for (int i = 0; i < holding_count; i++) new_holdings[i] = holdings[i];
        delete[] holdings;
        holdings = new_holdings;
    }

    int find_index(string symbol) {
        for (int i = 0; i < holding_count; i++) {
            if (holdings[i].stock_symbol == symbol) return i;
        }
        return -1;
    }

    Portfolio() : holding_count(0), capacity(10) {
        holdings = new Holding[capacity];
    }

    ~Portfolio() { delete[] holdings; }

    void add_holding(string symbol, int qty, double price) {
        int index = find_index(symbol);
        if (index != -1) {
            double total_cost = holdings[index].purchase_price * holdings[index].quantity + price * qty;
            holdings[index].quantity += qty;
            holdings[index].purchase_price = total_cost / holdings[index].quantity;
        } else {
            if (holding_count >= capacity) resize_array();
            holdings[holding_count++] = Holding(symbol, qty, price);
        }
    }

    void remove_holding(string symbol, int qty) {
        int index = find_index(symbol);
        if (index != -1) {
            if (holdings[index].quantity > qty) {
                holdings[index].quantity -= qty;
            } else {
                for (int i = index; i < holding_count - 1; i++) {
                    holdings[i] = holdings[i + 1];
                }
                holding_count--;
            }
        }
    }

    void display_holdings(Market& market) {
        if (holding_count == 0) {
            cout << "+------------------------------------------------------------+" << endl;
            cout << "|                  No stocks in portfolio.                   |" << endl;
            cout << "+------------------------------------------------------------+" << endl;
            return;
        }
        double total_value = 0.0;
        cout << "+------------------------------------------------------------+" << endl;
        cout << "|                  Portfolio Holdings                        |" << endl;
        cout << "+------------+------------+-----------------+----------------+" << endl;
        cout << "| Symbol     | Quantity   | Current Price   | Bought At      |" << endl;
        cout << "+------------+------------+-----------------+----------------+" << endl;
        for (int i = 0; i < holding_count; i++) {
            Stock* s = market.find_stock_by_symbol(holdings[i].stock_symbol);
            if (s) {
            double value = s->get_current_price() * holdings[i].quantity;
            total_value += value;
            cout << "| " << left << setw(10) << holdings[i].stock_symbol
                 << "| " << left << setw(10) << holdings[i].quantity
                 << "| $" << left << setw(14) << fixed << setprecision(2) << s->get_current_price()
                 << "| $" << left << setw(13) << fixed << setprecision(2) << holdings[i].purchase_price
                 << "|" << endl;
            }
        }
        cout << "+------------+------------+-----------------+----------------+" << endl;
        cout << "| " << setw(47) << right << "Total Value: $" << fixed << setprecision(2) << total_value << " |" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        }
    

    // Friend function to access private members of Portfolio and Market to calculate total value
    friend double calculate_total_value( Portfolio& portfolio, Market& market);
};

// Friend function to compute total portfolio value
double calculate_total_value( Portfolio& portfolio,  Market& market) {
    double total_value = 0.0;
    for (int i = 0; i < portfolio.holding_count; i++) {
        Stock* s = market.find_stock_by_symbol(portfolio.holdings[i].stock_symbol);
        if (s) {
            total_value += s->get_current_price() * portfolio.holdings[i].quantity;
        }
    }
    return total_value;
}

// Transaction Class purpose to store the transaction details 
class Transaction {
private:
    int transaction_id;
    int user_id;
    string stock_symbol;
    int quantity;
    double price_per_share;
    string type;
    time_t timestamp;

    // Friend class for access to private members
    friend class Transaction_manager;

public:
    Transaction() : transaction_id(0), user_id(0), stock_symbol(""), quantity(0), price_per_share(0.0), type(""), timestamp(0) {}
    Transaction(int t_id, int u_id, string symbol, int qty, double price, string t_type) : transaction_id(t_id), user_id(u_id), stock_symbol(symbol), quantity(qty), price_per_share(price), type(t_type), timestamp(time(0)){}
    int get_user_id() { return user_id; }

    void display() {
        cout << "ID: " << transaction_id << ", User: " << user_id << ", Stock: " << stock_symbol<< ", Qty: " << quantity << ", Price: $" << price_per_share << ", Type: " << type<< ", Time: " << ctime(&timestamp);
    }
};

// Transaction_manager Class
class Transaction_manager {
private:
    Transaction* transactions;
    int transaction_count;
    int capacity;
    int next_id; // to keep track of next transaction id

    void resize_array() {
        capacity *= 2;
        Transaction* new_transactions = new Transaction[capacity];
        for (int i = 0; i < transaction_count; i++) new_transactions[i] = transactions[i];
        delete[] transactions;
        transactions = new_transactions;
    }

public:
    Transaction_manager() : transaction_count(0), capacity(10), next_id(1) {
        transactions = new Transaction[capacity];
    }
    ~Transaction_manager() { delete[] transactions; }
    
    void add_transaction(int u_id, string symbol, int qty, double price, string type) {
        if (transaction_count >= capacity) resize_array();
        transactions[transaction_count++] = Transaction(next_id++, u_id, symbol, qty, price, type);
    }

    void display_user_transactions(int u_id) {
        cout << string(100, '=') << endl;
        cout << setw(60) << right << "=== User Transactions ===" << endl;
        cout << string(100, '=') << endl;
        bool found = false;
        for (int i = 0; i < transaction_count; i++) {
            if (transactions[i].user_id == u_id) { // Direct access via friend relationship
                transactions[i].display();
                found = true;
            }
        }
        if (!found) cout << "No transactions record found.!\n";
        cout << string(100, '=') << endl;
    }
};

// Menu Class
class Menu {
private:
    User* users[MAX_USERS];
    int user_count;

    int find_user_id(int id) {
        for (int i = 0; i < user_count; i++) {
            if (users[i]->get_id() == id) 
            return i;
        }
        return -1;
    }

public:
    int get_user_count(){ 
        return user_count; 
    }

    Menu():user_count(0){
        for (int i = 0; i < MAX_USERS; i++) {
            users[i] = nullptr;
        }
    }

    ~Menu() {
        for (int i = 0; i < user_count; i++) 
        {
            delete users[i];
        }
    }

    void signup() {
        if (user_count >= MAX_USERS) {
            cout << "User limit reached. Can't create more accounts!\n";
            return;
        }

        string name, pass, type;
        int id;
        cout << "|================================================|\n";
        cout << "|              === Signup ===                    |\n";
        cout << "|================================================|\n";
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "\nEnter unique numeric ID: ";
        // this while loop used to check if the input is valid or not if the input is not valid then it will clear the input buffer and ignore the next 1000 characters
        while (!(cin >> id)){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "not valid input Try Again! \nEnter unique numeric ID: ";
        }
       
        if (find_user_id(id) != -1) {
            cout << "ID already exists...\n";
            return;
        }

        cout << "\nSet password: ";
         cin.ignore(1000,'\n');
        getline(cin, pass);

        cout << "\nAccount type (Regular/Premium) (p) for premium and (r) regular: ";
        getline(cin, type);
        if (type == "Premium" || type == "premium" || type == "p") {
            users[user_count++] = new Premium_user(name, id, pass);
            cout << "Signup successful! You have created your Premium account. Please login next to proceed.\n";
        } else if (type == "Regular" || type == "regular" || type == "r") {
            users[user_count++] = new Regular_user(name, id, pass);
            cout << "Signup successful! You have created your Regular account. Please login next to proceed.\n";
        } else {
            cout << "Account type not exist! Please try again.\n";
        }
    }

    User* login() {
        int id;
        string pass;
        cout << "|================================================|\n";
        cout << "|                  === Login ===                 |\n";
        cout << "|================================================|\n";
        cout << "Enter your Numeric ID: ";
        while (!(cin >> id)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "not valid input Try again!\nEnter your Numeric ID: ";
        }
        cin.ignore(1000, '\n');
        cout << "\nEnter password: ";
        getline(cin, pass);
        int index = find_user_id(id);
        if (index != -1 && users[index]->checkPassword(pass)) {
            cout << "Login successful! Welcome " << users[index]->get_name() << " Congratulations! a Demo Account provided to you with 5000$ amount\n";
            return users[index]; //here this will return address of the user object which is created in signup function
        }
        cout << "credentials Not valid ....\n";
        return nullptr;
    }

    void trade_stocks(User* user, Market& market, Transaction_manager& tm, Portfolio& portfolio) {
        cout << "|======================================================================|\n";
        cout << "|                       === Trading Stocks ===                         |\n";
        cout << "|======================================================================|\n";
        market.display_all_stocks();
        int choice;
        do {
            cout << "|======================================================================|\n";
            cout << "|                       === Trading Menu ===                           |\n";
            cout << "|======================================================================|\n";
            cout << "\n1. Refresh Prices\n2. Buy\n3. Sell\n4. Back\nChoice: ";
            while (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "not valid input Try again!\nChoice: ";
            }
            cin.ignore(1000, '\n');
            switch (choice) {
                case 1:
                    cout << "Refreshing prices...\n";
                    market.update_all_prices();
                    market.display_all_stocks();
                    cout << "\nPrices refreshed.\n";
                    break;
                case 2: {
                    string symbol;
                    int qty;
                    cout << "|======================================================================|\n";
                    cout << "|                       === Buy Stocks ===                             |\n";
                    cout << "|======================================================================|\n";
                    cout << "Enter stock symbol(Case sensitive so be carefull): ";
                    getline(cin, symbol);
                    Stock* s = market.find_stock_by_symbol(symbol);
                    if (s){
                        cout << "\nEnter quantity: ";
                        while (!(cin >> qty) || qty <= 0) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "not valid input\nEnter quantity: ";
                        }
                        cin.ignore(1000, '\n');
                        double cost = s->get_current_price() * qty;
                        if (user->withdraw(cost)) {
                            int index = portfolio.find_index(symbol);// to check if the stock is in portfolio then we will get the index of that stock to update the price
                            double profit_loss = 0.0;
                            if (index != -1) {
                                // here we are calculating profit/loss if the stock is already in portfolio
                                // if the stock is already in portfolio then we will get the previous price of that stock from portfolio
                                double prev_price = portfolio.holdings[index].purchase_price;
                                profit_loss = (prev_price - s->get_current_price()) * qty;
                            }
                            // here we are adding the stock to portfolio and updating the price of that stock in portfolio
                            portfolio.add_holding(symbol, qty, s->get_current_price());
                            // here we are adding the transaction to transaction manager
                            tm.add_transaction(user->get_id(), symbol, qty, s->get_current_price(), "BUY");
                            // here we are incrementing the transaction count of user
                            user->increment_transactions();
                            cout << "You bought " << qty << " shares of " << symbol << " at $" << s->get_current_price() << ".\n";
                            if (index != -1) {
                                if (profit_loss < 0) {
                                    cout << "You paid $" << -profit_loss << " more compared to your previous average price per share.\n";
                                } else if (profit_loss > 0) {
                                    cout << " COngrats! You saved $" << profit_loss << " compared to your previous average price per share.\n";
                                } else {
                                    cout << "No profit/loss compared to your previous average price per share.\n";
                                }
                            }
                        } else {
                            cout << "Insufficient Shares To BUY!\n";
                        }
                    } else {
                        cout << "Stock not found.\n";
                    }
                    break;
                }
                case 3: {
                    cout << "|======================================================================|\n";
                    cout << "|                       === Sell Stocks ===                            |\n";
                    cout << "|======================================================================|\n";
                    string symbol;
                    int qty;
                    cout << "Enter stock symbol(Case sensitive): ";
                    getline(cin, symbol);

                    int index = portfolio.find_index(symbol);
                    if (index == -1 || portfolio.holdings[index].quantity <= 0) {
                        cout << "You do not own any shares of this stock.\n";
                        break;
                    }

                    Stock* s = market.find_stock_by_symbol(symbol);
                    if (s) {
                        cout << "You own " << portfolio.holdings[index].quantity << " shares of " << symbol << ".\n";
                        cout << "Enter quantity to sell: ";
                        while (!(cin >> qty) || qty <= 0) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "not valid input Try again!\nEnter quantity to sell: ";
                        }
                        cin.ignore(1000, '\n');
                        // here we are checking if the quantity of stock in portfolio is greater than or equal to the quantity we want to sell
                        if (portfolio.holdings[index].quantity >= qty) {
                            // here we are calculating the revenue and profit/loss
                            double revenue = s->get_current_price() * qty;
                            double profit = (s->get_current_price() - portfolio.holdings[index].purchase_price) * qty;
                            // here we are updating the portfolio and transaction manager and removing the stock from portfolio
                            user->deposit(revenue);
                            portfolio.remove_holding(symbol, qty);
                            tm.add_transaction(user->get_id(), symbol, qty, s->get_current_price(), "SELL");
                            user->increment_transactions();
                            cout << " You Sold " << qty << " shares of " << symbol << " at $" << s->get_current_price() << ".\n";
                            if (profit > 0){
                                cout << "Congrats! You made a profit of $" << profit << ".\n";
                            }
                            else
                            {
                                cout << "Unfortunately You are in a loss of $" << -profit << ".\n";
                            }
                        } else {
                            cout << "Insufficient shares to SELL.\n";
                        }
                    } else {
                        cout << "Stock not found.\n";
                    }
                    break;
                }
                case 4:
                    cout << "Exiting trading menu...\n";
                    break;
                default:
                    cout <<"Choice Not Exist. Try again!.\n";
            }
        } while (choice != 4);
    }

    void view_market(Market& market) {
        int choice;
        do {
            cout << "|======================================================================|\n";
            cout << "|                       === Market Menu ===                            |\n";
            cout << "|======================================================================|\n";
            cout << "\n1. Refresh Prices\n2. View Stocks\n3. View Stock History\n4. Add Stock\n5. Delete Stock\n6. Back\nChoice: ";
            while (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "not valid input\nChoice: ";
            }
            cin.ignore(1000, '\n');
            switch (choice) {
                case 1:
                    market.update_all_prices();
                    cout << "Prices refreshed.\n";
                    break;
                case 2:
                    market.display_all_stocks();
                    break;
                case 3: {
                    string symbol;
                    cout << "Enter stock symbol: ";
                    getline(cin, symbol);
                    Stock* s = market.find_stock_by_symbol(symbol);
                    if (s) s->display_history();
                    else cout << "Stock not found.\n";
                    break;
                }
                case 4: {
                    if (Stock::get_total_stocks() >= 100) {
                        cout << "Stock limit reached.\n";
                        break;
                    }
                    string name, symbol;
                    double price, prev_price, vol;
                    cout << "Enter stock name: ";
                    getline(cin, name);
                    cout << "Enter stock symbol : ";
                    getline(cin, symbol);
                    cout << "Enter current price: ";
                    while (!(cin >> price)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "not valid input\nEnter current price: ";
                    }
                    cin.ignore(1000, '\n');
                    cout << "Enter previous price: ";
                    while (!(cin >> prev_price)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "not valid input\nEnter previous price: ";
                    }
                    cin.ignore(1000, '\n');
                    cout << "Enter volatility (e.g., 0.02): ";
                    while (!(cin >> vol)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "not valid input\nEnter volatility (e.g., 0.02): ";
                    }
                    cin.ignore(1000, '\n');
                    market.add_stock(Stock(name, symbol, price, prev_price, vol));
                    cout << "Stock added successfully.\n";
                    break;
                }
                case 5: {
                    string symbol;
                    cout << "Enter stock symbol to delete: ";
                    getline(cin, symbol);
                    market.delete_stock(symbol);
                    break;
                }
                case 6:
                    break;
                default:
                    cout << "Not Valid choice Try Again!\n";
            }
        } while (choice != 6);
    }
    
};

void dashboard(Menu& menu, User* user, Market& market, Transaction_manager& tm, Portfolio& portfolio) {
    int choice;
    do {
        cout << "|--------------------------------------------------------------------------------------------------------------|" << endl;
        cout << "|                                           === Dashboard ===                                                  |\n";
        cout << "|--------------------------------------------------------------------------------------------------------------|" << endl;
        cout << "\n1. View Portfolio\n2. Trade Stocks\n3. View Market\n4. View Transactions\n5. Exit\nChoice: ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "not valid input\nChoice: ";
        }
        cin.ignore(1000, '\n');
        switch (choice) {
            case 1:
                cout << *user; // through << overload operator which we created in user class which contain display calling this is how we are displaying the user details
                // Display portfolio information
                portfolio.display_holdings(market);
                break;
            case 2:
                menu.trade_stocks(user, market, tm, portfolio);
                break;
            case 3:
                menu.view_market(market);
                break;
            case 4:
                tm.display_user_transactions(user->get_id());
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "choice does not exist.\n";
        }
    } while (choice != 5);
}


int main() {
    srand(time(0));
    Menu menu;
    Market market;
    Transaction_manager tm;
    cout << "|==================================================================================================================================|\n";
    cout << "|                                              Welcome to the Stock Market Simulation!                                             |\n";
    cout << "|==================================================================================================================================|\n";
    cout << "|                  This simulation allows you to trade stocks, view market data, and manage your portfolio.                        |\n";
    cout << "|                  You can create a demo account to practice trading.                                                              |\n";
    cout << "|                           -> Developed by: Aazan Noor Khuwaja                                                                    |\n";
    cout << "|                           -> Course: Object Oriented Programming                                                                 |\n";
    cout << "|                           -> National University of Computer and Emerging Sciences                                               |\n";
    cout << "|==================================================================================================================================|\n";
    cout << "      Features:\n- User Signup/Login\n- Demo Account with $5000\n- Trade Stocks (Buy/Sell)\n- View Portfolio & Market Data\n";
    cout << "      Press any key to continue.... ";
    cin.get();// wait for user input to continue
    

    int choice;
    do {
        cout << "\n|=====================================|\n";
        cout << "|             Main Menu               |\n";
        cout << "|=====================================|\n";
        cout << "\n1. Signup\n2. Login\n3. Exit\nChoice: ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "not valid input Try again!\nChoice: ";
        }
        cin.ignore(1000, '\n');
        switch (choice) {
            case 1:
                menu.signup();
                break;
            case 2:
                if (menu.get_user_count() == 0) {
                    cout << "No users registered. Please signup first.\n";
                } else {
                    User* user = menu.login(); // this will return the address of the user object which is created in signup function
                    if (user) {
                        Portfolio port;
                        dashboard(menu, user, market, tm, port);
                    }
                }
                break;
            case 3:
                cout << "|======================================|\n";
                cout << "|               Goodbye!               |\n";
                cout << "|======================================|\n";
                cout << "Exiting...\n";
                cout << "Thank you for using the Stock Market Simulation!\n";
                break;
            default:
                cout << "choice not exist..\n";
        }
    } while (choice != 3);

    return 0;
}