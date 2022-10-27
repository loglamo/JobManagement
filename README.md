# JobManagement
# Server and Client for Running Jobs 
These codes conclude one server file and one client file. They communicate via Socket. The server can start, pause, restart, stop the job according to commands from the client. The client sends a command (start: to start the job; pause: to pause the running job; restart: to continue the paused job; stop: to kill the job) at one time to the server.


1. **Run the codes with:**
- Compile servers and clients:

        g++ [server_name.cpp] -o [compiled_file_name] -L /usr/lib/ -lboost_system -lboost_thread -lpthread -ljsoncpp 
        g++ [client_name.cpp] -o [compiled_file_name] -L /usr/lib/ -lboost_system -lboost_thread -lpthread
        
- Execute servers and clients:

        ./[compiled_file_name]

2. **Notice:**

- Install the g++ compiler
- Be sure that the port is available when running code, otherwise the refused connection will occur
- The code is not completed. The current version is for sending and receiving message between the server and client 
