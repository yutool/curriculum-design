package com.anko.wechat.entity;

public class User {
    private String name;        // 用户名
    private String password;    // 密码
    private String friends;     // 好友
    private String flocks;      // 群
    
    public User() { }
    public User(String name, String password) {
        this.name = name;
        this.password = password;
    }

    public String getName() { return name; }

    public String getPassword() { return password; }

    public String getFriends() { return friends; }

    public String getFlocks() { return flocks; }

    public void setName(String name) { this.name = name; }

    public void setPassword(String password) { this.password = password; }

    public void setFriends(String friends) { this.friends = friends; }

    public void setFlocks(String flocks) { this.flocks = flocks; }

}
