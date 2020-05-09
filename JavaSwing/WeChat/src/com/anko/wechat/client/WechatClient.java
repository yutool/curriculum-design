package com.anko.wechat.client;

import com.anko.wechat.frame.WechatFrame;
import com.anko.wechat.tool.MassageUtil;
import org.json.JSONObject;

import javax.swing.*;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Properties;

public class WechatClient {
    private MassageUtil userUtil;
    private Socket socket;
    private String serverIP;
    private final int serverPort = 6666;
    private WechatFrame wechatFrame;

    public WechatClient(String useraName) {
        try {
            userUtil = new MassageUtil();
            Properties properties = new Properties();
            properties.load(WechatClient.class.getClassLoader().getResourceAsStream("database.properties"));
            serverIP = properties.getProperty("serverIp");
            socket = new Socket(serverIP, serverPort);
            userUtil.sendmsgToServer(socket, useraName);
            wechatFrame = new WechatFrame(useraName, socket);
            wechatFrame.setVisible(true);
            // 开启一个线程处理消息
            Thread runClientThread = new Thread(new Runnable() {
                @Override
                public void run() { runClient(); }
            });
            runClientThread.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void runClient() {
        while(true) {
            // 接收消息并处理
            String resultMsg = userUtil.ReceiveMassage(socket);
            // 处理信息
            Thread repageThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    JSONObject json = new JSONObject(resultMsg);
                    String fromName = (String) json.get("fromName");
                    String type = (String) json.get("type");
                    String newsMsg = fromName;
                    if(type.equals("toFriend")) { // 好友聊天信息
                        // 获取聊天记录
                        Map<String,List<String>> chattingRecords = wechatFrame.getChattingRecords();
                        List<String> listRecords = chattingRecords.get(fromName);
                        if(listRecords == null) listRecords = new ArrayList<>();
                        // 加到聊天记录
                        listRecords.add(resultMsg);
                        chattingRecords.put(fromName,listRecords);
                        // 刷新页面正在和该好友聊天则刷新其屏幕
                        if(fromName.equals(wechatFrame.getDChoiceFriend())) {
                            String records = userUtil.formatRecords(listRecords);
                            wechatFrame.getDispTextArea().setText(records);
                        } else { // 添加到新消息列表
                            DefaultListModel<String> newListModel = wechatFrame.getNewsListModel();
                            newsMsg += " 的消息";
                            if(!newListModel.contains(newsMsg)) {
                            	newListModel.addElement(newsMsg);
                                wechatFrame.getNewList().setModel(newListModel);
                            }
                        }
                        // 播放声音...
                    } else if(type.equals("addFriend")) { // 添加好友请求
                        DefaultListModel<String> newListModel = wechatFrame.getNewsListModel();
                        newsMsg += " 请求添加好友";
                        newListModel.addElement(fromName+" 请求添加好友");
                        wechatFrame.getNewList().setModel(newListModel);
                    }
                }
            });
            repageThread.start();
        }
    }
}
