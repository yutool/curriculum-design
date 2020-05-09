package com.anko.wechat.server;

import com.anko.wechat.tool.MassageUtil;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

/**
 * WeChat 服务端
 */
public class WechatServer {
    private Map<String, Socket> sockets; // 连接对象的socket
    private MassageUtil massageUtil;
    private ServerSocket server = null;
    private Socket socket = null;
    private final int port = 6666;

    public static void main(String[] args) {
        new WechatServer();
    }

    public WechatServer() {
        try {
            massageUtil = new MassageUtil();
            server = new ServerSocket(port);
            sockets = new HashMap<>();
            System.out.println("WeChat服务器启动成功~~~");
            runServer();  // 启动服务器等待请求
        } catch (IOException e) { e.printStackTrace(); }
    }

    public void runServer() {
        while (true) {
            try {
                // 接受服务端请求
                socket = server.accept();
                // 读取用户名
                String userName = new DataInputStream(socket.getInputStream()).readUTF();
                // 为每个用户开启一个线程
                if(socket != null) {
                    System.out.print("用户" + userName + "已连接,他的ip地址为：");
                    System.out.println(socket.getInetAddress().getHostAddress());
                    new ServerThread(userName, socket).start();
                }
            } catch (IOException e) { e.printStackTrace(); }
        }
    }

    /**
     * 为一个客户端开启一个服务器线程
     */
    class ServerThread extends Thread {
        private String userName;    // 用户名
        private Socket socket;      // 对应的socket

        public ServerThread(String name, Socket socket) {
            this.socket = socket;
            this.userName = name;
            sockets.put(name, socket);
            System.out.println("目前在线人数：" + sockets.size());
        }

        public void run() {
            while (true) {
                // 接受信息聊天信息
                String jsonMsg = massageUtil.ReceiveMassage(socket);
                if(jsonMsg == null) { // 连接断开
                    try {
                        sockets.get(userName).close();
                        sockets.remove(userName);
                        System.out.println("用户"+userName+"已经下线");
                        System.out.println("目前在线人数："+sockets.size());
                    } catch (IOException e) { e.printStackTrace();}
                    return ;
                }
                // 接收消息后转发数据给客户端
                Thread transmitThread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        massageUtil.serverTransmitMsg(sockets, jsonMsg);
                    }
                });
                transmitThread.start();
            }
        }
    }
}