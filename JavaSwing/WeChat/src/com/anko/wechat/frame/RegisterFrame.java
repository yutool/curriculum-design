package com.anko.wechat.frame;

import com.anko.wechat.client.WechatClient;
import com.anko.wechat.entity.User;
import com.anko.wechat.service.UserService;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class RegisterFrame extends JFrame {
	private static final long serialVersionUID = 1L;
	public RegisterFrame() {
        this.setTitle("WeChat");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        initComponents();
    }

    // 判断用户输入是否为空值
    private boolean judegNull(String name, String pwd) {
        if(name == null || "".equals(name)) {
            JOptionPane.showMessageDialog(null,"用户名不能为空","空值",JOptionPane.ERROR_MESSAGE);
            return true;
        }
        if(pwd == null || "".equals(pwd)) {
            JOptionPane.showMessageDialog(null,"密码不能为空","空值",JOptionPane.ERROR_MESSAGE);
            return true;
        }
        return false;
    }

    // 注册用户
    private void registerActionPerformed(ActionEvent e) {
        String name = this.userNameText.getText();
        String pwd = new String(this.userPwdText.getPassword());
        if(!judegNull(name, pwd)) {
            if(userService.isExistUser(name)) {
                JOptionPane.showMessageDialog(null,"用户已存在","注册账号",JOptionPane.ERROR_MESSAGE);
                return ;
            }
            int res = userService.addUser(new User(name,pwd));
            if(res > 0) {
                JOptionPane.showMessageDialog(null,"注册成功","注册账号",JOptionPane.INFORMATION_MESSAGE);
            }
        }
    }

    // 用户登录
    private void loginActionPerformed(ActionEvent e) {
        String name = this.userNameText.getText();
        String pwd = new String(this.userPwdText.getPassword());
        if(!judegNull(name, pwd)) {
            if(userService.login(new User(name,pwd))) {
                dispose();
                new WechatClient(name);
            } else {
                JOptionPane.showMessageDialog(null,"账号或密码错误","登录失败",JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void initComponents() {
        userService = new UserService();
        // JFormDesigner - Component initialization - DO NOT MODIFY  //GEN-BEGIN:initComponents
        jPanel = new JPanel();
        userNameText = new JTextField();
        userPwdText = new JPasswordField();
        nameLabel = new JLabel();
        pwdLabel = new JLabel();
        titleLable = new JLabel();
        register = new JButton();
        login = new JButton();

        //======== this ========
        Container contentPane = getContentPane();
        //======== jPanel ========
        jPanel.setLayout(null);
        //---- userNameText ----
        userNameText.setPreferredSize(new Dimension(3, 20));
        userNameText.setFont(userNameText.getFont().deriveFont(userNameText.getFont().getSize() + 5f));
        jPanel.add(userNameText);
        userNameText.setBounds(150, 105, 240, 35);

        //---- userPwdText ----
        userPwdText.setPreferredSize(new Dimension(3, 20));
        jPanel.add(userPwdText);
        userPwdText.setBounds(150, 160, 240, 35);

        //---- nameLabel ----
        nameLabel.setText("\u7528\u6237\u540d:");
        nameLabel.setFont(nameLabel.getFont().deriveFont(nameLabel.getFont().getSize() + 2f));
        jPanel.add(nameLabel);
        nameLabel.setBounds(70, 100, 80, 40);

        //---- pwdLabel ----
        pwdLabel.setText("\u5bc6  \u7801:");
        pwdLabel.setFont(pwdLabel.getFont().deriveFont(pwdLabel.getFont().getSize() + 2f));
        jPanel.add(pwdLabel);
        pwdLabel.setBounds(70, 160, 75, 30);

        //---- titleLable ----
        titleLable.setText("wechat");
        titleLable.setFont(titleLable.getFont().deriveFont(titleLable.getFont().getSize() + 10f));
        jPanel.add(titleLable);
        titleLable.setBounds(215, 35, 90, 50);

        //---- register ----
        register.setText("\u6ce8\u518c");
        register.setPreferredSize(new Dimension(30, 20));
        register.setMaximumSize(new Dimension(30, 28));
        register.setMinimumSize(new Dimension(30, 28));
        register.addActionListener(e -> registerActionPerformed(e));
        jPanel.add(register);
        register.setBounds(155, 240, 90, 39);

        //---- login ----
        login.setText("\u767b\u5f55");
        login.addActionListener(e -> loginActionPerformed(e));
        jPanel.add(login);
        login.setBounds(290, 240, 90, 39);

        GroupLayout contentPaneLayout = new GroupLayout(contentPane);
        contentPane.setLayout(contentPaneLayout);
        contentPaneLayout.setHorizontalGroup(
            contentPaneLayout.createParallelGroup()
                .addComponent(jPanel, GroupLayout.Alignment.TRAILING, GroupLayout.DEFAULT_SIZE, 520, Short.MAX_VALUE)
        );
        contentPaneLayout.setVerticalGroup(
            contentPaneLayout.createParallelGroup()
                .addComponent(jPanel, GroupLayout.DEFAULT_SIZE, 333, Short.MAX_VALUE)
        );
        pack();
        setLocationRelativeTo(getOwner());
        // JFormDesigner - End of component initialization  //GEN-END:initComponents
    }

    // JFormDesigner - Variables declaration - DO NOT MODIFY  //GEN-BEGIN:variables
    private JPanel jPanel;
    private JTextField userNameText;
    private JPasswordField userPwdText;
    private JLabel nameLabel;
    private JLabel pwdLabel;
    private JLabel titleLable;
    private JButton register;
    private JButton login;
    // JFormDesigner - End of variables declaration  //GEN-END:variables
    private UserService userService;

}
