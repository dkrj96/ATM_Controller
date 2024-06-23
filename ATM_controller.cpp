#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>


struct Account_Info {
    std::string account_number, pin;
    int balance;

    Account_Info() : account_number(""), pin(""), balance(0) {}

    Account_Info(const std::string& acNum, const std::string& pinNum, int bal)
        : account_number(acNum), pin(pinNum), balance(bal) {}
};


class BANK_API {
public:
    BANK_API(const std::string& filename) : balancesFile(filename) {
        readBalancesFromFile();
    }

    bool check_Card_Num(const std::string& card_number) {
        if(account_info.find(card_number) != account_info.end()) 
        {
            std::cout << "Card has been recognized : " << card_number << std::endl;
            return true;
        } else {
            std::cerr << "No Valid Card : " << card_number << std::endl;
            return false;
        }
    }

    bool check_PIN(const std::string& card_number, const std::string& pin) {
        if(account_info.find(card_number) != account_info.end()) 
        {
            return account_info[card_number].pin == pin;
        } else {
            std::cerr << "Can't Find Your Account Number : " << card_number << std::endl;
            return false;
        }
    }

    bool check_Account(const std::string& card_number, const std::string& account_number) {
        return account_info[card_number].account_number == account_number;
    }

    int get_balance(const std::string& account_number) {
        if(account_info.find(account_number) != account_info.end()) {
            return account_info[account_number].balance;
        } else {
            std::cerr << "Account Not Found : " << account_number << std::endl;
            return -1;
        }
    }

    void deposit(const std::string& card_number, const std::string& account_number, int money) {
        if(account_info.find(card_number) != account_info.end()) {
            account_info[card_number].balance += money;
        } else {
            std::cerr << "Account Not Found" << std::endl;
        }
    }

    int withdraw(const std::string& card_number, const std::string& account_number, int money) {
        if(account_info.find(card_number) != account_info.end()) {
            if(account_info[card_number].balance >= money)
            {
                account_info[card_number].balance -= money;
                return 1;
            } else {
                std::cout << "Not Enough Amount of Money to Withdraw" << std::endl;
                return -2;
            }
        } else {
            std::cerr << "Account Not Found" << std::endl;
            return -1;
        }
    }


private:
    std::unordered_map<std::string, Account_Info> account_info;
    std::string balancesFile; 

    void readBalancesFromFile() {
        std::ifstream file(balancesFile);
        if (file.is_open()) {
            std::string card_number, account_number, pin;
            int balance;
            while (file >> card_number >> account_number >> pin >> balance) {
                account_info[card_number] = Account_Info(account_number, pin, balance);
            }
            file.close();
        } else {
            std::cerr << "Your PIN is Incorrect" << std::endl;
        }
    }
};


class ATM_CONTROLLER {
public:
    ATM_CONTROLLER(BANK_API& bank_API) : bank_API(bank_API), inserted_card_number(""), account("") {} 

    bool card_insert(const std::string& card_number)
    {
        if(bank_API.check_Card_Num(card_number)) {
            inserted_card_number = card_number;
            return true;
        } else return false;
    }

    bool enter_PIN(const std::string inserted_card_number, const std::string PIN_number)
    {
        return bank_API.check_PIN(inserted_card_number, PIN_number);
    }
    
    bool select_account(const std::string& account_number)
    {
        if(bank_API.check_Account(inserted_card_number, account_number)) {
            account = account_number;
            return true;
        } else return false;
    }

    int check_balance(const std::string& account_number) {
        return bank_API.get_balance(inserted_card_number);
    }

    void deposit(int money) {
        return bank_API.deposit(inserted_card_number, account, money);
    }

    int withdraw(int money) {
        return bank_API.withdraw(inserted_card_number, account, money);
    }


private:
    BANK_API& bank_API;
    std::string inserted_card_number;
    std::string account;
};


int main()
{
    BANK_API bank_API("C:\\Users\\best\\Documents\\vs_code\\account_info_File.txt");  // Put Text File(account_info_File.txt) Location Here
    ATM_CONTROLLER atm_controller(bank_API);
    std::string card_num = "";
    std::string pin = "";
    std::string account_num = "";

    std::cout << "Please Insert Your Card (Type Card Number)" << std::endl;
    std::cin >> card_num;

    if(atm_controller.card_insert(card_num))
    {
        std::cout << "Enter Your PIN" << std::endl;
        std::cin >> pin;
        if(atm_controller.enter_PIN(card_num, pin))
        {    
            std::cout << "PIN is Correct" << std::endl;
            std::cout << "Enter Your Account Number" << std::endl;
            std::cin >> account_num;
            if(atm_controller.select_account(account_num))
            {
                std::cout << "Welcome" << std::endl;
                int option;
                do {
                    std::cout << "\n\nATM Menu:\n";
                    std::cout << "1. Check Balance\n";
                    std::cout << "2. Deposit\n";
                    std::cout << "3. Withdraw\n";
                    std::cout << "4. Exit\n";
                    std::cout << "Enter Option: ";
                    std::cin >> option;

                    std::cout << "option : " << option << std::endl;

                    switch (option) {
                        case 1: {
                            int balance = atm_controller.check_balance(account_num);
                            if (balance != -1) {
                                std::cout << "Current Balance: " << balance << std::endl;
                            }
                            break;
                        }
                        case 2: {
                            int money;
                            std::cout << "Enter your desired deposit amount : ";
                            std::cin >> money;
                            atm_controller.deposit(money);
                            std::cout << "Before Amount : " << atm_controller.check_balance(account_num) - money << std::endl;
                            std::cout << "After Amount : " << atm_controller.check_balance(account_num) << std::endl;
                            break;
                        }
                        case 3: {
                            int money;
                            std::cout << "Enter your desired withdraw amount : ";
                            std::cin >> money;
                            if(atm_controller.withdraw(money) > 0)
                            {
                                std::cout << "Before Amount : " << atm_controller.check_balance(account_num) + money << std::endl;
                                std::cout << "After Amount : " << atm_controller.check_balance(account_num) << std::endl;
                            }
                            break;
                        }
                        case 4:
                            std::cout << "Exiting ATM, Have a Good Day." << std::endl;
                            break;
                        default:
                            std::cout << "Invalid option. Please try again." << std::endl;
                            break;
                    }
                } while (option != 4);
            } else std::cout << "ACCOUNT NUMBER INCORRECT!" << std::endl;
        } else std::cout << "PIN NUMBER IS INCORRECT!" << std::endl;
    } 

    return 0;
}