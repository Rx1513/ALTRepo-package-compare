#include <iostream>
#include <string>

int main( int argc, char* argv[] ) {
    while (true) {
        std::string firstBranch,
                    secondBranch;

        std::cout << "Enter first branch: ";
        std::cin >> firstBranch;
        std::cout << "Enter second branch: ";
        std::cin >> secondBranch;

        /*
        TODO:

        string? some_local_function_that_takes_user_input_and_returns_JSON_response/s_from_ALTREPO_server(string first_branch, string second_branch)
        
        Some confdition that checks succes of requests
        {
            some_shared_library_function_that_compares_two_branches_and_returns_JSON
        } else {
            Tell user about error
        }
        */

        std::cout << "Do you want to make another comparison? [y/N] ";
        std::string userInput;
        std::cin >> userInput;
        if (userInput[0] != 'y' && userInput[0] != 'Y') {
            std::cout << "Canceled." << std::endl;
            break;
        }
    }
    return 0;
}