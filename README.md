This is a foundational Boost.Beast REST API server.

#### How It Works

1. Dependencies:
	
    * The program uses `Boost.Beast` for HTTP handling and `Boost.Json` for JSON
	parsing and generation.

	* `Boost.Asio` manages the network operations.

* Features
        
	* A GET route at `/` that returns a welcome message.

    * A GET route at `/hello/<name>` that greets the user by name.

    * A POST route at `/add` that accepts a JSON body with `a` and `b` fields,
      adds them, and returns the `sum` in JSON.

* Concurrency
        
	* Each client request is handled in a separate thread using std::thread.

#### Compile

    $ mkdir build
    $ cd build
    $ cmake -G Ninja ..
    $ ninja

#### Run

    $ ./rest-server

####$  Test

* GET request

        $ curl http://localhost:8080/
    
* GET re$ quest

        $ curl http://localhost:8080/hello/John

* POST request

        $ curl -X POST http://localhost:8080/add -d '{"a":5,"b":10}'
