package com.anko.wechat.tool;

import java.io.IOException;
import java.lang.reflect.Field;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.*;

public class DBUtil {
	private String driver;
	private String url;
	private String username;
	private String password;
	private Connection con;
	private PreparedStatement pstmt;
	private ResultSet rs;

	public DBUtil() {
		Properties properties = new Properties();
		try {
			properties.load(DBUtil.class.getClassLoader().getResourceAsStream("database.properties"));
			driver = properties.getProperty("driver");
			url = properties.getProperty("url");
			username = properties.getProperty("username");
			password = properties.getProperty("password");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public DBUtil(String driver, String url, String username, String password) {
		this.driver = driver;
		this.url = url;
		this.username = username;
		this.password = password;
	}

	/**
	 * 判断数据是否存在于数据库
	 * @return true表示存在
	 */
	public boolean isExist(String sql, String[] params) {
		try {
			this.setParams(sql, params);			// 根据sql语句和params，设置pstmt对象
			ResultSet rs = pstmt.executeQuery();	// 执行查询操作
			if (rs.next()) {
				return true;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			close();
		}
		return false;
	}

	/**
	 * 执行数据库查询操作时，将返回的结果封装到List对象中
	 * @return [{key=value, key=value}]
	 */
	public List<Map<String, Object>> getList(String sql, String[] params) {
		List<Map<String, Object>> list = new ArrayList<>();
		try {
			this.setParams(sql, params);
			ResultSet rs = pstmt.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			while (rs.next()) {
				Map<String, Object> m = new HashMap<>();
				for (int i = 1; i <= rsmd.getColumnCount(); i++) {
					String colName = rsmd.getColumnName(i);	// 获取列名
					m.put(colName, rs.getObject(i));
				}
				list.add(m);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			close();
		}
		return list;
	}

	/**
	 * 将获取的数据封装成bean加入list集合
	 * @param clazz 要封装对象的字节码对象
	 * @return [bean,bean]
	 */
	public List<?> getBeans(String sql, String[] params, Class<?> clazz) {
		List<Object> list = new ArrayList<>();
		try {
			this.setParams(sql, params);
			ResultSet rs = pstmt.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			while (rs.next()) {
				Object obj = clazz.newInstance();
				for (int i = 1; i <= rsmd.getColumnCount(); i++) {
					String colName = rsmd.getColumnName(i);
					Field field = clazz.getDeclaredField(colName); 	// 获取属性
					if (field != null) {
						field.setAccessible(true);					// 取消访问检查
						Object value = rs.getObject(i);
						if (value != null)
							field.set(obj, value);					// 为属性赋值
					}
				}
				list.add(obj);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
		return list;
	}

	/**
	 * 更新数据库时调用的update方法
	 * @return 受影响条数
	 */
	public int update(String sql, String[] params) {
		int recNo = 0;						// 表示受影响的记录行数
		try {
			setParams(sql, params);
			recNo = pstmt.executeUpdate();	// 执行更新操作
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
		return recNo;
	}

	// 给pstmt的SQL语句设置参数（要求参数以数组形式给出）
	private void setParams(String sql, String[] params) {
		pstmt = this.getPrepareStatement(sql);
		if (params != null) {
			for (int i = 0; i < params.length; i++) {
				try {
					pstmt.setString(i + 1, params[i]);
				} catch (SQLException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	private Connection getConnection() { // 获取连接对象
		try {
			Class.forName(driver);
			con = DriverManager.getConnection(url, username, password);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return con;
	}
	
	private PreparedStatement getPrepareStatement(String sql) { // 获取语句对象
		try {
			pstmt = getConnection().prepareStatement(sql);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return pstmt;
	}

	private void close() { // 关闭对象
		try {
			if (rs != null)
				rs.close();
			if (pstmt != null)
				pstmt.close();
			if (con != null)
				con.close();
		} catch (SQLException e) { }
	}

}