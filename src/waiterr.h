#ifndef WAITERR_INCL
#define WAITERR_INCL
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <map>
#include <stdexcept>
#include <functional>
#include <pthread.h>
#include <sstream>

namespace waiterr {
#define DEF_HTTP_VER "1.1"
#define DEF_APP_NAME "Waiterr"
#define DEF_VERSION "0.1"
#define DEF_MISC_INFO "Light-weight http server utility for C++"

#define LF "\n"
#define CRLF "\r\n"

typedef std::string ss;
typedef const ss css;

    enum operation_codes {
        OK = 0,
        ERROR = 1
    };

    namespace config_info {
        css HTTP_VER = DEF_HTTP_VER;
        css APP_NAME = DEF_APP_NAME,
                                VERSION = DEF_VERSION,
                                MISC_INFO = DEF_MISC_INFO;
        
    };

    namespace header_strings {
        css CONTENT_TYPE = "Content-Type";
    };

    class Request {

    };

    class Response {
        private:
            ss prepare_header();
            int status_code;
            ss body;
            std::map<css, ss> header_map;
            const int _sock;
        public:
            Response(int sock) : _sock(sock) {
                status_code = 200;
            }

            Response& set_status_code(int status_code) {
                this->status_code = status_code;
                return *this;
            }

            Response& set_content_type(ss content_type) {
                header_map[header_strings::CONTENT_TYPE] = content_type;
                return *this;
            }

            Response& set_header(css header_key, ss header_value) {
                header_map[header_key] = header_value;
                return *this;
            }

            Response& set_body(ss body) {
                this->body = body;
                return *this;
            }

            void send() {
                std::stringstream header_stream;
                header_stream << "HTTP/1.1 " << status_code << LF;
                
                for (auto head_it = header_map.begin(); head_it != header_map.end(); ++head_it) {
                    header_stream << head_it->first << ": " << head_it->second << LF;
                }

                header_stream << CRLF;
                header_stream << body;

                int _len = header_stream.seekg(0, std::ios::end).tellg();

                write(_sock, header_stream.str().c_str(), _len);
            }
    };

    struct _threadH_paramStruct {
        int incoming_socket;
        std::map<css, std::function<void(waiterr::Request&, waiterr::Response&)> >& get_callbacks;
    };

    class Waiter {
        
        private:
            int port;
            int sock;
            struct sockaddr_in address;
            int address_length = sizeof(struct sockaddr_in);
            enum operation_codes setup_socket();
            enum operation_codes accept_connections();
            static void* handle_new_connection(void * incoming_params);
            std::map<css, std::function<void(waiterr::Request&, waiterr::Response&)> > get_callbacks;
        
        public:
            Waiter(int port) : port(port) {
                setup_socket();
            }

            enum operation_codes start_listen();

            void get(css uri_path, std::function<void(waiterr::Request&, waiterr::Response&)> callback);

            int getPort() { return port; }
    };

    class WaiterrException : std::runtime_error {
        private:
            ss message;

        public:
            WaiterrException (css &msg) : std::runtime_error(msg), message(msg) {}
    };
}

waiterr::operation_codes waiterr::Waiter::setup_socket() {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw waiterr::WaiterrException("Some error creating the socket. Exiting");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if ((bind(sock, (sockaddr*) &address, (socklen_t) sizeof(address)) < 0)) {
        throw waiterr::WaiterrException("Some error in bind operation. Exiting");
    }

    return operation_codes(OK);
}

waiterr::operation_codes waiterr::Waiter::start_listen() {
    if ((listen(sock, 1)) < 0) {
        throw waiterr::WaiterrException("Some error in listen operation. Exiting");
    }

    return accept_connections();
}

void* waiterr::Waiter::handle_new_connection(void * incoming_params) {
    struct _threadH_paramStruct param_struct = *((struct _threadH_paramStruct *) incoming_params);
    
    char *buffer = new char[4096];
    int val_read = read(param_struct.incoming_socket, buffer, 4096);

    std::stringstream buffer_ss(buffer);

    if (buffer != NULL) {
        ss buffer_line;
        std::getline(buffer_ss, buffer_line, '\n');

        [&, param_struct](ss& parse_method_and_url) {
            
            if (parse_method_and_url.find(":") == ss::npos) {
            
                std::stringstream method_params(parse_method_and_url);
                ss method, url_format;
                std::getline(method_params, method, ' ');
                std::getline(method_params, url_format, ' ');

                Request req; 
                Response *res = new Response(param_struct.incoming_socket);
                
                if (method.compare("GET") == 0) {
                    param_struct.get_callbacks[url_format](req, *res);
                    close(param_struct.incoming_socket);
                }

            }
        }(buffer_line);
    }

    // write(param_struct.incoming_socket, data_to_send, strlen(data_to_send));

    return (void*) NULL;
}

waiterr::operation_codes waiterr::Waiter::accept_connections() {
    while (1) {
        int in_socket;

        if ((in_socket = accept(sock, (struct sockaddr*) &address, (socklen_t*) &address_length)) < 0) {
            perror("In socket accept");
            exit(4);
        }
        
        pthread_t in_conn_th;
        struct _threadH_paramStruct thread_params = { in_socket, get_callbacks};

        pthread_create(&in_conn_th, NULL, handle_new_connection, (void *)(&thread_params));
    }
    return operation_codes(OK);
}

void waiterr::Waiter::get(css uri_path, std::function<void(waiterr::Request&, waiterr::Response&)> callback) {
    get_callbacks[uri_path] = callback;
}

#endif
