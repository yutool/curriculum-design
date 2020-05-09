package com.anko.wechat.dao;

import com.anko.wechat.entity.User;
import com.anko.wechat.tool.DBUtil;

import java.util.List;
import java.util.Map;

public class UserDao {
    DBUtil db = new DBUtil();

    // 判断是否登录成功
    public boolean isLoginSucceed(User user) {
        String sql = "select * from user where name = ? and password = ?";
        return db.isExist(sql, new String[]{user.getName(),user.getPassword()});
    }

    // 判断用户是否存在
    public boolean isExistUser(String name) {
        String sql = "select * from user where name = ?";
        return db.isExist(sql, new String[]{name});
    }

    // 获取用户信息
    public User getUserInfo(String name) {
        String sql = "select * from user where name = ?";
        @SuppressWarnings("unchecked")
		List<User> list = (List<User>) db.getBeans(sql, new String[]{name}, User.class);
        return list.get(0);
    }

    // 获取好友
    public String getFriends(String name) {
        String sql = "select friends from user where name = ?";
        List<Map<String, Object>> list = db.getList(sql, new String[]{name});
        return (String) list.get(0).get("friends");
    }

    // 增加用户
    public int addUser(User user) {
        String sql = "insert into user(name,password) values(?,?)";
        int res = db.update(sql, new String[]{user.getName(),user.getPassword()});
        return res;
    }

    // 更新好友列表
    public int updateFriend(User user) {
        String sql = "update user set friends = ? where name = ?";
        int res = db.update(sql, new String[]{user.getFriends(),user.getName()});
        return res;
    }

}
