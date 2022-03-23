#include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    unsigned short port = 8989;


    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);

    if(lfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "192.168.142.132", (void *)&saddr.sin_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(port);

    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        return -1;
    }

    // 监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        return -1;
    }

    // 接收客户端连接
    struct sockaddr_in cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
    if(cfd == -1) {
        perror("accpet");
        return -1;
    }

    // 获取客户端信息
    char cliIp[16];
    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIp, sizeof(cliIp));
    unsigned short cliPort = ntohs(cliaddr.sin_port);

    // 输出客户端的信息
    printf("client's ip is %s, and port is %d\n", cliIp, cliPort );

    // 接收客户端发来的数据
    char recvBuf[1024] = {0};
    while(1) {
        int len = read(cfd, &recvBuf, sizeof(recvBuf));
        if(len == -1) {
            perror("read");
            return -1;
        }else if(len > 0) {
            printf("recv client data : %s\n", recvBuf);
        } else {
            printf("client closed...\n");
            break;
        }

        // 写数据
        write(cfd, recvBuf, strlen(recvBuf));
    }

    close(cfd);
    close(lfd);

    return 0;
}
