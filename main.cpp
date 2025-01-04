#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;       // From <boost/beast.hpp>
namespace http = beast::http;         // From <boost/beast/http.hpp>
namespace net = boost::asio;          // From <boost/asio.hpp>
namespace json = boost::json;         // From <boost/json.hpp>
using tcp = boost::asio::ip::tcp;     // From <boost/asio/ip/tcp.hpp>

// Function to handle HTTP requests
void handle_request(
    http::request<http::string_body> req,
    http::response<http::string_body> &res)
{
    if ((req.method() == http::verb::get) && (req.target() == "/"))
    {
        res.result(http::status::ok);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Welcome to the Boost.Beast REST API!\n";
    }
    else if ((req.method() == http::verb::get) && req.target().starts_with("/hello/"))
    {
        std::string name = req.target().substr(7);
        res.result(http::status::ok);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Hello, " + name + "!\n";
    }
    else if ((req.method() == http::verb::post) && (req.target() == "/add"))
    {
        try
        {
            auto body = json::parse(req.body());
            int a = body.at("a").as_int64();
            int b = body.at("b").as_int64();
            int sum = a + b;

            json::object result;
            result["sum"] = sum;

            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");
            res.body() = json::serialize(result) + "\n";
        }
        catch (...)
        {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "text/plain");
            res.body() = "Invalid JSON or missing fields";
        }
    }
    else
    {
        res.result(http::status::not_found);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Not Found";
    }
    res.prepare_payload();
}

// Function to handle a single HTTP session
void session(tcp::socket socket)
{
    try
    {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        http::response<http::string_body> res;
        handle_request(std::move(req), res);

        http::write(socket, res);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Main function
int main()
{
    try
    {
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, { tcp::v4(), 8080 });

        std::cout << "Server is running on http://localhost:8080/" << std::endl;

        while (true)
        {
            tcp::socket socket(ioc);
            acceptor.accept(socket);
            std::thread(session, std::move(socket)).detach();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    return 0;
}
