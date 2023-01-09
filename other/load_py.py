import socket
import threading

def send_request(server_port, num_requests):
    # 连接服务器的地址和端口
    server_address = ("172.16.167.90" ,8090)

    # 创建一个套接字
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 连接服务器
    sock.connect(server_address)

    # 循环发送 num_requests 个请求
    for i in range(num_requests):
        sock.sendall(b'http://example.com/link/%d' % i)

    # 不断发送字符到服务器
    while True:
        sock.sendall(b'Hello, server!')

    # 关闭套接字
    sock.close()

# 创建 10 个线程，每个线程负责发送 10000 个请求
for i in range(12):
    t = threading.Thread(target=send_request, args=(8000+i, 1000000))
    t.start()

