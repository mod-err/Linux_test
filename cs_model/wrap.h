#ifndef __WRAP_H_
#define __WRAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <unistd.h> 
#include <errno.h>

void perr_exit(const char *s);
int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, const void *ptr, size_t nbytes);
int Close(int fd);
ssize_t Readn(int fd, void *vptr, size_t n);
ssize_t Writen(int fd, const void *vptr, size_t n);
static ssize_t my_read(int fd, char *ptr);
ssize_t Readline(int fd, void *vptr, size_t maxlen);

#endif