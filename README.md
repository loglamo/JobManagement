# JobManagement
# Server and Client for Running Jobs 
These codes are with 3 asynchronous servers and 2 clients. 

Clients can choose one server in (1, 2, 3 - id of server), one job (1, 2 - id of job), one command in (Start, Stop, Pause, Resume) at one time. 

Servers receive commands from clients, and execute commands (Start reading file, Stop reading file, Pause reading file, or Resume reading file).


1. **Run the codes with:**
- Compile servers and clients:

        g++ [server_name.cpp] -o [compiled_file_name] -L /usr/lib/ -lboost_system -lboost_thread -lpthread -ljsoncpp 
        g++ [client_name.cpp] -o [compiled_file_name] -L /usr/lib/ -lboost_system -lboost_thread -lpthread
        
- Execute servers and clients:

        ./[compiled_file_name]

2. **Notice:**

- Install the g++ compiler
- Be sure that the port is available when running code, otherwise the refused connection will occur
- Current version runs with 3 servers and 2 clients. All 3 servers should be available at the time clients want to connect to. 
- With each done command from one client, values of properties "lastcommand", "currentreading" will be update in metadata.json file. The initial states of "lastcommand" and "currentreading" are "Stop" and 0 respectively. 
