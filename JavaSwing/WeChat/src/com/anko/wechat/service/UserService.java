package com.anko.wechat.service;

import com.anko.wechat.dao.UserDao;
import com.anko.wechat.entity.User;

import javax.swing.*;
import java.util.Arrays;

public class UserService {
    UserDao userDao = new UserDao();

    // 判断用户是否存在
    public boolean isExistUser(String name) { return userDao.isExistUser(name); }
    
    // 登录
    public boolean login(User user) { return userDao.isLoginSucceed(user); }
    
    // 获取用户信息
    public User getUserInfo(String name) {
        if(userDao.isExistUser(name)) {
            return userDao.getUserInfo(name);
        }
        return null;
    }
    
    // 增加用户
    public int addUser(User user) {
        if(!userDao.isExistUser(user.getName())) {
            int res = userDao.addUser(user);
            return res;
        }
        return 0;
    }
    
    // 获取好友列表
    public DefaultListModel<String> getFriendList(String name) {
        DefaultListModel<String> defaultListModel = new DefaultListModel<>();
        if(userDao.isExistUser(name)) {
        	String friends = userDao.getFriends(name);
        	if(friends != null && !"".equals(friends)) {
        		String[] friendArr = userDao.getFriends(name).split("&");
                Arrays.sort(friendArr);
                for(String str : friendArr) {
                    defaultListModel.addElement(str);
                }
        	}
        }
        return defaultListModel;
    }
    
    // 增加好友
    public int addFriend(User user, String name) {
    	String friends = user.getFriends();
    	if(friends == null || "".equals(friends)) { // 判断是否存在好友
    		user.setFriends(name);
        } else {
        	user.setFriends(friends+"&"+name);
        }
        return userDao.updateFriend(user);
    }
    
    // 删除好友
    public int delFriend(User user, String name) {
        if(user.getFriends().contains(name)) { // 判断是否存在这个好友
            String[] temp = user.getFriends().split("&");
            String friends = "";
            for(int i=0; i < temp.length-1; i++) {
                if(!name.equals(temp[i])) {
                    friends = friends + temp[i] + "&";
                }
            }
            if(name.equals(temp[temp.length-1])) { // 被删除的在最后一位
            	if(!"".equals(friends))
            		friends = friends.substring(0,friends.length()-1); // 去除最后一个&
            } else {
                friends = friends + temp[temp.length-1];
            }
            user.setFriends(friends);
            return userDao.updateFriend(user);
        }
        return 0; // 不存在用户
    }

}
