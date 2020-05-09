package com.anko.wechat.tool;

import org.json.JSONObject;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.*;
import java.util.*;

public class MassageUtil {
    // 向服务器发送信息
    public void sendmsgToServer(Socket socket, String massage) {
        try {
            new DataOutputStream(socket.getOutputStream()).writeUTF(massage);
        } catch (IOException e) { e.printStackTrace(); }
    }

    // 转发消息
    public void serverTransmitMsg(Map<String,Socket> sockets, String jsonMsg){
        try {
            JSONObject json = new JSONObject(jsonMsg);
            String name = (String) json.get("toName");
            if(name == null || "".equals(name)) return;
            if(sockets.containsKey(name)) {
            	Socket socket = sockets.get(name);
                new DataOutputStream(socket.getOutputStream()).writeUTF(jsonMsg);
            } else {
            	System.out.println("好友 " + name + " 不在线");
            }
        } catch (IOException e) { e.printStackTrace(); }
    }

    // 接收信息
    public String ReceiveMassage(Socket socket) {
        String jsonMsg = null;
        try {
            jsonMsg = new DataInputStream(socket.getInputStream()).readUTF();
        } catch (IOException e) {/*断开连接会报异常，返回null在服务端处理*/}
        return jsonMsg;
    }

    // 聊天信息格式json{fromName,date,type,toName,msg}
    public JSONObject recordsToJson(String fromName, String date, String type, String toName, String msg) {
        Map<String,Object> map = new HashMap<>();
        map.put("fromName", fromName);
        map.put("date", date);
        map.put("type", type);
        map.put("toName", toName);
        map.put("msg", msg);
        JSONObject json = new JSONObject(map);
        return json;
    }

    // 格式化聊天记录
    public String formatRecords(List<String> listRecords) {
        String records = "";
        String premName = null;
        String formName = null;
        String date = null;
        String msg = null;
        JSONObject json = null;
        for(String str : listRecords) {
            json = new JSONObject(str);
            formName = (String) json.get("fromName");
            date = (String) json.get("date");
            msg = (String) json.get("msg");
            if(!formName.equals(premName))
                records = records + " " + formName + "  " + date + "\n"; // 发信息的用户和时间
            records = records + " · " + msg + "\n";
            premName = formName;
        }
        return records;
    }

}