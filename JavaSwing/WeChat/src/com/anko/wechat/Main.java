package com.anko.wechat;

import com.anko.wechat.frame.RegisterFrame;
import com.anko.wechat.frame.WechatFrame;

import javax.swing.*;

import java.awt.*;
import java.util.logging.Level;
import java.util.logging.Logger;
/**
 * WeChat 客户端
 */
public class Main {
    public static void main(String[] args) {
        try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException | InstantiationException | IllegalAccessException
				| UnsupportedLookAndFeelException e) {
			Logger.getLogger(RegisterFrame.class.getName()).log(Level.SEVERE, null, e);
            Logger.getLogger(WechatFrame.class.getName()).log(Level.SEVERE, null, e);
		}

        EventQueue.invokeLater(new Runnable() {
            public void run() {
                new RegisterFrame().setVisible(true);
            }
        });
    }
}
