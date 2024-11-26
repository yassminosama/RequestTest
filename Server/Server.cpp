#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void handleClient(asio::ip::tcp::socket& socket) {
    try {
        char buffer[1024];
        asio::error_code error;

        
        size_t length = socket.read_some(asio::buffer(buffer), error);
        if (error && error != asio::error::eof) {
            throw asio::system_error(error);
        }

        string request(buffer, length);
        cout << "Received request from client: " << request << endl;

        // Read the file
        ifstream file("C:\\Users\\HiTech\\Downloads\\sample.csv");
        if (!file.is_open()) {
            cerr << "Error: Unable to open CSV file." << endl;
            string errorMessage = "Error: Unable to open CSV file.";
            asio::write(socket, asio::buffer(errorMessage));
            return;
        }

        
        ostringstream fileContent;
        fileContent << file.rdbuf();
        file.close();

        // Send the file 
        string response = fileContent.str();
        asio::write(socket, asio::buffer(response));
        cout << "CSV file sent to client." << endl;
    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));

        cout << "Server is running and waiting for a connection..." << endl;

        while (true) {
            asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);
            cout << "Client connected!" << endl;
            handleClient(socket);
        }
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}
