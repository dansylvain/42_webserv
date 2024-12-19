# Webserv Project Documentation
A lightweight and efficient HTTP web server, built in C++ as part of the 42 project, inspired by Nginx and Apache.

<p align="center">
  <img src="img/webserv.png" width="70%"/>
</p>

## 🚀 Synopsis  
The `webserv` project challenges students to build a fully functional HTTP/1.1 server in C++ from scratch, inspired by existing web servers like Nginx or Apache.

This endeavor requires a solid understanding of network programming, socket management, and the HTTP protocol. Students must design a server capable of handling multiple simultaneous connections, serving static files, and managing dynamic content through CGI.

By tackling challenges like request parsing, response formatting, and efficient resource management, `webserv` sharpens essential skills in web server development on Unix-like systems.

Successfully completing this project demonstrates mastery of network communication and offers a deep dive into the internal workings of modern web technologies.

## ✨ Features  

The `webserv` project includes the following key features:  

- **HTTP/1.1 Protocol Support**  
  Full compliance with HTTP/1.1, including request parsing and response formatting.  

- **Static File Hosting**  
  Serve static files such as HTML, CSS, JavaScript, and images efficiently.  

- **Dynamic Content Handling (CGI)**  
  Support for executing CGI scripts to generate dynamic content.  

- **Multiple Simultaneous Connections**  
  Manage multiple client connections concurrently using non-blocking I/O.  

- **Custom Configuration**  
  Flexible configuration through a custom file, enabling settings for ports, routes, error pages, etc.  

- **Error Management**  
  Graceful handling of common HTTP errors (404, 500, etc.) with customizable error pages.  

- **Logging**  
  Basic logging of server activity for monitoring and debugging purposes.  

- **Cross-Platform Compatibility**  
  Designed to work on Unix-like systems with standard socket programming interfaces.  

 🖥️ Installation  

To get started with `webserv`, simply follow these steps:

1. Clone the repository:  
```bash
git clone <repo_url>
```
2. Navigate to the project directory:
```bash
cd webserv
```
3. Build the project using the Makefile:
```bash
make
```
The Makefile will automatically install all required dependencies and compile the server.

Once the build process is complete, you can start the server and configure it as needed.

## 🛠️ Configuration  

The server configuration can be customized in detail to suit specific needs. Rather than detailing all the configuration options here, please refer to the configuration file located at:

- **`_configs/template_config.ini`**

This file contains comprehensive explanations of all the available configuration options, including how to adjust parameters for logging, server behavior, and more.

Simply open the `template_config.ini` file and follow the instructions to modify the settings according to your preferences.

For a quick overview, the default configuration will work out of the box, but customization allows for more advanced features.

## ⚙️ Usage  

One of the key aspects we focused on is the logging system, which allows you to control the level of detail displayed during the server's operation. The verbosity of the logs can be configured to show different amounts of information depending on your needs.

### Running the Server  

To run `webserv`, you can execute it in two ways:

1. **Default Configuration**:  
If no configuration file is specified, the server will run with a default configuration file.
```bash
./webserv
```
2. Custom Configuration:
You can specify your own configuration file by providing it as an argument:
```bash
./webserv <path_to_config_file>
```
### Verbosity Levels
You can choose the level of verbosity for the logs by using the -h flag to display the help message and options:
```bash
./webserv -h
```
This will show you the available log levels and other configuration options for adjusting the output according to your preferences.


## 🗂️ Project Structure  

The project is organized into the following main directories and files:

### 📁 `_configs`
Contains various configuration files specifically prepared to demonstrate different features of the project.

### 📁 `_websites`
This directory contains subdirectories, each representing a complete website setup with all necessary files (HTML, CSS, etc.).

### 📁 `doc`
Documentation files, including project specifications, tester guides, scripts, and other related resources.

### 📁 `errorPages`
Contains the default HTML error pages (e.g., 404, 500) served by the server when necessary.

### 📁 `includes`
Contains header files (`.hpp`) used across the project, defining key constants, structures, and function declarations.

### 📁 `src`
The source code directory, where all `.cpp` files are stored along with their corresponding `.hpp` header files. This is the core of the project, where the server’s functionality is implemented.

### 📁 `tests`
Holds files required to run the test suite, including test scripts to ensure server functionality (e.g., `siege` tester).

### 📁 `uploads`
A configurable directory for handling file uploads, used by the server for storing uploaded content.

### 📄 Root Files
- **index.html**: Required files for project correction.
- **Makefile**: Used to build the project and install dependencies.
- **Bash Scripts**: Scripts to help test the project (e.g., running `siege` tests, sending requests with `curl`).

The structure is designed for modularity, with dedicated folders for configuration, websites, documentation, and testing, making it easy to manage and extend the project.

## 🤝 Contributing  

If you would like to contribute to this project, feel free to fork the repository, create a pull request, and follow the code standards used in this project. Contributions are welcome, whether they are bug fixes, new features, or documentation improvements.

## 🚧 Limitations  

While the `webserv` project demonstrates the fundamentals of an HTTP server, it has the following limitations:

- **No HTTPS Support**: The server only supports HTTP, without SSL/TLS encryption.
- **Limited Protocol Support**: The server is based on HTTP/1.1 and does not support newer versions like HTTP/2 or HTTP/3.
- **Limited HTTP Methods**: Server only support the following HTTP Methods : **GET**, **POST** and **DELETE**.
- **No WebSocket Support**: The server cannot handle WebSocket connections for real-time bidirectional communication.
- **Basic Caching**: There is no advanced caching mechanism for HTTP responses.
- **Scalability**: The server is designed for small-scale use and does not support distributed or horizontally scaled systems.
- **No Authentication**: The server does not include authentication or access control features.
- **Limited CGI Support**: While CGI is supported, the implementation is basic and does not cover advanced use cases.
- **Browser Compatibility**: The server is fully compatible with *Google Chrome*. Others browsers should work just fine, but very specific features might not.

## 🧑‍💻 Authors  

This project was developed by the following contributors:

- **Florian VERGE** ([@flverge](https://github.com/maitreverge)) – Specialized in server configuration, testing processes, response generation, and error management.
- **Sébastien VIDOT** ([@svidot](https://github.com/sierra182)) – Focused on implementing core server logic, managing UNIX system calls, and integrating CGI support.
- **Dan SYLVAIN** ([@dsylvain](https://github.com/dansylvain)) – Responsible for configuration file parsing, HTTP request handling, and leading test-driven development.

The project started in September 2024 and was completed over the course of ten weeks.

Special thanks to **Sylvain Duriff** for his invaluable contribution of deep cosmic insight that kept the team going.  
  
## 📜 License  

This project is licensed under the [MIT License](LICENSE), except where noted.

"Everything is permitted except that which is forbidden."  
- The 42 Philosophy of Open Source  

Feel free to use, modify, or share this project, as long as it doesn’t involve sending it to a Vogon poetry reading. 😉
