#include <iostream>
#include <fstream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/lexical_cast.hpp>

#include <libpkgcmp.hpp>


using namespace boost::beast;
using namespace boost::asio;


template<
    class SyncReadStream,
    class DynamicBuffer,
    bool isRequest, class Body, class Allocator
>
void
readLargeResponseBody(
    SyncReadStream& stream,
    DynamicBuffer& buffer,
    http::message<isRequest, Body, http::basic_fields<Allocator>>& message,
    std::size_t bodyLimit
){
    http::parser<isRequest, Body, Allocator> parser(std::move(message));

    parser.eager(true);
    parser.body_limit(bodyLimit);

    http::read(stream, buffer, parser);

    message = parser.release();
}


std::string
requestPackagesFromBranch(std::string branch)
{
    const std::string host = "rdb.altlinux.org";
    const std::string path = "/api/export/branch_binary_packages/" + branch;
    const std::string port = "443";

    io_service service;

    ssl::context context(ssl::context::sslv23_client);
    ssl::stream<ip::tcp::socket> ssocket = { service, context };
    ip::tcp::resolver resolver(service);

    auto it = resolver.resolve(host, port);

    connect(ssocket.lowest_layer(), it);

    ssocket.handshake(ssl::stream_base::handshake_type::client);

    http::request<http::string_body> request{ http::verb::get, path, 11 };

    request.set(http::field::host, host);
    http::write(ssocket, request);
    http::response<http::string_body> response;

    flat_buffer buffer;
    readLargeResponseBody(ssocket, buffer, response,80*1024*1024);
    std::string response_body = boost::lexical_cast<std::string>(response.body());

    return response_body;
}


int main( int argc, char* argv[] )
{
    while (true) {
        std::string firstBranch;
        std::string secondBranch;

        std::cout << "Enter first branch: ";
        std::cin >> firstBranch;
        std::cout << "Enter second branch: ";
        std::cin >> secondBranch;

        std::string firstBranchPackages = requestPackagesFromBranch(firstBranch);
        std::string secondBranchPackages = requestPackagesFromBranch(secondBranch);
        
        std::cout << "Response received" << std::endl;
        
        std::string result = compareBranches(
            std::move(firstBranchPackages),
            std::move(secondBranchPackages),
            std::move(firstBranch),
            std::move(secondBranch)
        );

        std::cout << result << std::endl;

        std::ofstream resultFile("result.json");
        resultFile << result;
        resultFile.close();

        std::cout << "Do you want to make another comparison? [y/n] ";
        std::string userInput;
        std::cin >> userInput;

        if (userInput[0] != 'y' && userInput[0] != 'Y')
        {
            std::cout << "Canceled." << std::endl;
            break;
        }
    }
    return 0;
}