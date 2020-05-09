package com.anko.wechat.frame;

import com.anko.wechat.entity.User;
import com.anko.wechat.service.UserService;
import com.anko.wechat.tool.MassageUtil;
import org.json.JSONObject;

import java.awt.event.*;
import java.awt.*;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.List;
import javax.swing.*;

public class WechatFrame extends JFrame {
	private static final long serialVersionUID = 1L;

	public WechatFrame(String name, Socket socket) {
        userService = new UserService();
        this.socket = socket;
        this.user = userService.getUserInfo(name);
//        ScreenSize.centered(this);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("WeChat");
        initComponents();
    }
   // private WechatFrame() { initComponents(); }

    // 退出按钮
    private void exitBtnActionPerformed(ActionEvent e) {
        this.dispose();
        System.exit(0);
    }

    //发送信息按钮
    private void sendBtnActionPerformed(ActionEvent e) {
        String msg = msgTextArea.getText();
        msgTextArea.setText("");
        // 发送数据给服务器
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        JSONObject json =  userUtil.recordsToJson(user.getName(),df.format(new Date()),"toFriend",dChoiceFriend,msg);
        userUtil.sendmsgToServer(socket, json.toString());
        // 处理消息，显示到页面
        List<String> listRecords = chattingRecords.get(dChoiceFriend);
        if(listRecords == null) listRecords = new ArrayList<>();
        listRecords.add(json.toString());
        String records = userUtil.formatRecords(listRecords);
        dispTextArea.setText(records);
        // 添加回至聊天记录
        chattingRecords.put(dChoiceFriend,listRecords);
    }

    // 增加好友按钮
    private void addFriendBtnActionPerformed(ActionEvent e) {
        String name = JOptionPane.showInputDialog("请输入对方网名：");
        if(name == null || "".equals(name)) return  ;
        if(friendListModel.contains(name)) {
            JOptionPane.showMessageDialog(null,"你们已是好友","添加好友",JOptionPane.INFORMATION_MESSAGE);
            return ;
        }
        if(!userService.isExistUser(name)) {
        	JOptionPane.showMessageDialog(null,"没有该用户","添加好友",JOptionPane.ERROR_MESSAGE);
        	return ;
        }
        int res = userService.addFriend(user, name);
        if(res > 0) {
            friendListModel.addElement(name);
            friendList.setModel(friendListModel);
            JOptionPane.showMessageDialog(null,"添加好友成功","添加好友",JOptionPane.INFORMATION_MESSAGE);
            // 告诉服务器，让对方同意
            SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            JSONObject json =  userUtil.recordsToJson(user.getName(),df.format(new Date()),"addFriend",name,"");
            userUtil.sendmsgToServer(socket, json.toString());
        } else {
            JOptionPane.showMessageDialog(null,"添加好友失败","添加好友",JOptionPane.ERROR_MESSAGE);
        }
    }

    // 删除按钮
    private void deleteBtnActionPerformed(ActionEvent e) {
        if(cChoiceFriend == null) {
            JOptionPane.showMessageDialog(null,"请选择删除对象","删除",JOptionPane.ERROR_MESSAGE);
        } else {
            int n = JOptionPane.showConfirmDialog(null, "确定要删除"+ cChoiceFriend +"吗？", "删除",JOptionPane.YES_NO_OPTION);
            if(n == 0) { // 点击确认
                int res = userService.delFriend(user, cChoiceFriend);
                if(res > 0) {
                    friendListModel.removeElement(cChoiceFriend);
                    cChoiceFriend = null;
                    JOptionPane.showMessageDialog(null,"删除成功","删除",JOptionPane.INFORMATION_MESSAGE);
                    friendList.setModel(friendListModel);
                }
                else {
                    JOptionPane.showMessageDialog(null,"删除失败","删除",JOptionPane.INFORMATION_MESSAGE);
                }
            }
        }
    }

    // 点击好友列表
    private void friendListMouseClicked(MouseEvent e) {
        if(friendList.getSelectedIndex() != -1) {
            if(e.getClickCount() == 1) {
                cChoiceFriend = (String) friendList.getSelectedValue();
            }
            if(e.getClickCount() == 2) {
                // 改变聊天对象
                dChoiceFriend = (String) friendList.getSelectedValue();
                this.setTitle("与 " + dChoiceFriend + " 聊天中...");
                // 设置与其的聊天记录
                List<String> listRecords = chattingRecords.get(dChoiceFriend);
                if(listRecords == null) listRecords = new ArrayList<>();
                String records = userUtil.formatRecords(listRecords);
                dispTextArea.setText(records);
            }
        }
    }

    // 点击新消息列表
    private void newsListMouseClicked(MouseEvent e) {
        if(newsList.getSelectedIndex() != -1) {
            if(e.getClickCount() == 1 || e.getClickCount() == 2) {
                cChoiceFriend = (String) friendList.getSelectedValue();
                String value = (String) newsList.getSelectedValue();
                String str[] = value.split(" ");
                if("请求添加好友".equals(str[1])) {
                    int n = JOptionPane.showConfirmDialog(null, "确定添加"+ str[0] +"吗？", "添加好友",JOptionPane.YES_NO_OPTION);
                    if(n == 0) {
                        int res = userService.addFriend(user, str[0]);
                        if(res > 0) {
                            friendListModel.addElement(str[0]);
                            friendList.setModel(friendListModel);
                        }
                    }
                } else if("的消息".equals(str[1])) {
                    // 设置与其的聊天记录
                    this.setTitle("与 " + str[0] + " 聊天中");
                    dChoiceFriend = str[0];
                    List<String> listRecords = chattingRecords.get(str[0]);
                    if(listRecords == null) listRecords = new ArrayList<>();
                    String records = userUtil.formatRecords(listRecords);
                    dispTextArea.setText(records);
                }
                // 移除
                newsListModel.removeElement(value);
                newsList.setModel(newsListModel);
            }
        }
    }

    private void initComponents() {
        friendListModel = new DefaultListModel<>();
        newsListModel = new DefaultListModel<>();
        chattingRecords = new HashMap<>();
        userUtil = new MassageUtil();
        // JFormDesigner - Component initialization - DO NOT MODIFY  //GEN-BEGIN:initComponents
        wechatPanel = new JPanel();
        contentPanel = new JPanel();
        inputPanel = new JPanel();
        msgPanel = new JPanel();
        btnPanel = new JPanel();
        exitBtn = new JButton();
        sendBtn = new JButton();
        msgScroll = new JScrollPane();
        msgTextArea = new JTextArea();
        dispScroll = new JScrollPane();
        dispTextArea = new JTextArea();
        listPanel = new JPanel();
        tabbedPane = new JTabbedPane();
        newsScroll = new JScrollPane();
        newsList = new JList<>();
        friendScroll = new JScrollPane();
        friendList = new JList<>();
        flockScroll = new JScrollPane();
        flockList = new JList<>();
        handlePanel = new JPanel();
        addFriendBtn = new JButton();
        addFlockBtn = new JButton();
        deleteBtn = new JButton();

        //======== this ========
        Container contentPane = getContentPane();
        //======== wechatPanel ========
        wechatPanel.setLayout(new BorderLayout());
        //======== contentPanel ========
        contentPanel.setLayout(new BorderLayout());
        //======== inputPanel ========
		inputPanel.setPreferredSize(new Dimension(56, 200));
		inputPanel.setLayout(new BorderLayout());
		//======== msgPanel ========
		msgPanel.setLayout(new BorderLayout());
		//======== btnPanel ========
		btnPanel.setPreferredSize(new Dimension(0, 30));
		btnPanel.setLayout(new FlowLayout(FlowLayout.RIGHT, 13, 1));
		
		//---- exitBtn ----
		exitBtn.setText("\u5173\u95ed");
		exitBtn.setPreferredSize(new Dimension(70, 29));
		exitBtn.addActionListener(e -> exitBtnActionPerformed(e));
		btnPanel.add(exitBtn);

		//---- sendBtn ----
		sendBtn.setText("\u53d1\u9001");
		sendBtn.setPreferredSize(new Dimension(70, 29));
		sendBtn.addActionListener(e -> sendBtnActionPerformed(e));
		btnPanel.add(sendBtn);
		msgPanel.add(btnPanel, BorderLayout.SOUTH);

		//======== msgScroll ========
		//---- msgTextArea ----
        msgTextArea.setFont(msgTextArea.getFont().deriveFont(msgTextArea.getFont().getSize() + 5f));
        msgScroll.setViewportView(msgTextArea);
        msgPanel.add(msgScroll, BorderLayout.CENTER);
        inputPanel.add(msgPanel, BorderLayout.CENTER);
        contentPanel.add(inputPanel, BorderLayout.SOUTH);

        //======== dispScroll ========
        //---- dispTextArea ----
        dispTextArea.setEditable(false);
        dispTextArea.setFont(dispTextArea.getFont().deriveFont(dispTextArea.getFont().getSize() + 5f));
        dispScroll.setViewportView(dispTextArea);
        contentPanel.add(dispScroll, BorderLayout.CENTER);
        wechatPanel.add(contentPanel, BorderLayout.CENTER);

    	//======== listPanel ========
        listPanel.setPreferredSize(new Dimension(200, 0));
        listPanel.setLayout(new BorderLayout());

    	//======== tabbedPane ========
        tabbedPane.setFont(tabbedPane.getFont().deriveFont(tabbedPane.getFont().getSize() + 2f));

    	//======== newsScroll ========
        //---- newsList ----
        newsList.setFont(newsList.getFont().deriveFont(newsList.getFont().getSize() + 3f));
        newsList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                newsListMouseClicked(e);
            }
        });
        newsScroll.setViewportView(newsList);
        tabbedPane.addTab("\u6d88\u606f", newsScroll);

        //======== friendScroll ========
        //---- friendList ----
        friendList.setFont(friendList.getFont().deriveFont(friendList.getFont().getSize() + 3f));
        friendList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                friendListMouseClicked(e);
            }
        });
        friendScroll.setViewportView(friendList);
        tabbedPane.addTab("\u597d\u53cb", friendScroll);

        //======== flockScroll ========
        //---- flockList ----
        flockList.setFont(flockList.getFont().deriveFont(flockList.getFont().getSize() + 3f));
        flockScroll.setViewportView(flockList);
        tabbedPane.addTab("\u7fa4\u804a", flockScroll);
        listPanel.add(tabbedPane, BorderLayout.CENTER);

    	//======== handlePanel ========
        handlePanel.setPreferredSize(new Dimension(0, 30));
        handlePanel.setLayout(new FlowLayout(FlowLayout.CENTER, 3, 1));

        //---- addFriendBtn ----
        addFriendBtn.setText("+\u597d\u53cb");
        addFriendBtn.setPreferredSize(new Dimension(70, 29));
        addFriendBtn.addActionListener(e -> addFriendBtnActionPerformed(e));
        handlePanel.add(addFriendBtn);

        //---- addFlockBtn ----
        addFlockBtn.setText("+\u7fa4");
        addFlockBtn.setPreferredSize(new Dimension(55, 29));
        handlePanel.add(addFlockBtn);

        //---- deleteBtn ----
        deleteBtn.setText("\u5220\u9664");
        deleteBtn.setPreferredSize(new Dimension(63, 29));
        deleteBtn.addActionListener(e -> deleteBtnActionPerformed(e));
        handlePanel.add(deleteBtn);
        listPanel.add(handlePanel, BorderLayout.SOUTH);
        
        wechatPanel.add(listPanel, BorderLayout.EAST);

        GroupLayout contentPaneLayout = new GroupLayout(contentPane);
        contentPane.setLayout(contentPaneLayout);
        contentPaneLayout.setHorizontalGroup(
            contentPaneLayout.createParallelGroup()
                .addComponent(wechatPanel, GroupLayout.Alignment.TRAILING, GroupLayout.DEFAULT_SIZE, 900, Short.MAX_VALUE)
        );
        contentPaneLayout.setVerticalGroup(
            contentPaneLayout.createParallelGroup()
                .addComponent(wechatPanel, GroupLayout.DEFAULT_SIZE, 620, Short.MAX_VALUE)
        );
        pack();
        setLocationRelativeTo(getOwner());
        // JFormDesigner - End of component initialization  //GEN-END:initComponents

        // ------------------------------AddContent - begin------------------------------
        friendListModel = userService.getFriendList(user.getName());
        friendList.setModel(friendListModel);
        // ------------------------------AddContent - end------------------------------
    }

    // JFormDesigner - Variables declaration - DO NOT MODIFY  //GEN-BEGIN:variables
    private JPanel wechatPanel;
    private JPanel contentPanel;
    private JPanel inputPanel;
    private JPanel msgPanel;
    private JPanel btnPanel;
    private JButton exitBtn;
    private JButton sendBtn;
    private JScrollPane msgScroll;
    private JTextArea msgTextArea;
    private JScrollPane dispScroll;
    private JTextArea dispTextArea;
    private JPanel listPanel;
    private JTabbedPane tabbedPane;
    private JScrollPane newsScroll;
    private JList<String> newsList;
    private JScrollPane friendScroll;
    private JList<String> friendList;
    private JScrollPane flockScroll;
    private JList<String> flockList;
    private JPanel handlePanel;
    private JButton addFriendBtn;
    private JButton addFlockBtn;
    private JButton deleteBtn;
    // JFormDesigner - End of variables declaration  //GEN-END:variables

    private DefaultListModel<String> friendListModel;
    private DefaultListModel<String> newsListModel;
    private User user;
    private UserService userService;
    private String cChoiceFriend;
    private String dChoiceFriend;
    private Map<String, List<String>> chattingRecords;
    private MassageUtil userUtil;
    private Socket socket;

    public JTextArea getDispTextArea() { return dispTextArea; }
    public Map<String, List<String>> getChattingRecords() { return chattingRecords; }
    public String getDChoiceFriend() { return dChoiceFriend; }
    public JList<String> getNewList() { return newsList; }
    public DefaultListModel<String> getNewsListModel() { return newsListModel; }

}
