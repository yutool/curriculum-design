using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;
using System.Data;
using System.Text;

namespace WeChat
{
    public class MySqlHelper
    {
        public static string ConnStr = "server=localhost;port=3306;user id=root;password=root;database=test;charset=utf8";
        public static DataTable GetDataTable(string SQL)
        {
            DataTable ADt = new DataTable();
            MySqlConnection AConn = new MySqlConnection(ConnStr);
            try
            {
                MySqlDataAdapter ADp = new MySqlDataAdapter(SQL, AConn);
                ADp.Fill(ADt);
            }
            catch
            {
                ;
            }
            finally
            {
                AConn.Close();
                AConn.Dispose();
                AConn = null;
            }

            return ADt;
        }
        public static DataRow GetDataRow(string SQL)
        {
            DataRow ARow = null;
            DataTable ADt = GetDataTable(SQL);
            try
            {
                if (ADt.Rows.Count > 0)
                    ARow = ADt.Rows[0];
            }
            finally
            {
                ADt.Dispose(); ADt = null;
            }
            return ARow;
        }
        /// <summary>
        /// 执行 Insert Delete Update
        /// </summary>
        /// <param name="SQL">SQL语句</param>
        /// <param name="AParaNames">参数名数组</param>
        /// <param name="AParaValues">参数值数组</param>
        /// <returns>成功-1;失败-0</returns>
        public static int DoExecute(string SQL, string[] AParaNames, object[] AParaValues)
        {
            if (AParaNames != null)
            {
                if (AParaNames.Length != AParaValues.Length)
                {
                    return 0;
                }
            }
            int Ares = 0;
            MySqlConnection AConn = new MySqlConnection(ConnStr);
            MySqlCommand Acmd = new MySqlCommand(SQL, AConn);
            Acmd.Parameters.Clear();
            Acmd.CommandText = SQL;
            try
            {
                if (AParaNames != null)
                {
                    for (int i = 0; i < AParaNames.Length; i++)
                    {
                        MySqlParameter Ap = null;
                        if (AParaValues[i] != null)
                        {
                            Ap = new MySqlParameter(AParaNames[i], AParaValues[i]);
                        }
                        else
                        {
                            if (AParaNames[i].ToUpper().Contains("F_SYMBOL"))
                            {
                                Ap = new MySqlParameter(AParaNames[i], SqlDbType.Image);
                                Ap.Value = DBNull.Value;
                            }
                            else if (AParaNames[i].ToUpper().Contains("F_PHOTO"))
                            {
                                Ap = new MySqlParameter(AParaNames[i], SqlDbType.Image);
                                Ap.Value = DBNull.Value;
                            }
                            else
                            {
                                Ap = new MySqlParameter(AParaNames[i], DBNull.Value);
                            }
                        }
                        Acmd.Parameters.Add(Ap);
                    }
                }
                AConn.Open();
                Acmd.ExecuteNonQuery();
                Ares = 1;
            }
            catch (Exception err) {; }
            finally
            {
                AConn.Close();
                Acmd.Dispose();
            }
            return Ares;
        }
        /// <summary>
        /// 执行Insert
        /// </summary>
        /// <param name="ATable"></param>
        /// <param name="AFields"></param>
        /// <param name="AValues"></param>
        /// <returns></returns>
        public static int DoInsert(string ATable, string[] AFields, object[] AValues)
        {
            string SQL = "Insert into " + ATable + "(";
            for (int i = 0; i < AFields.Length; i++)
            {
                SQL += AFields[i] + " ,";
            }
            SQL = SQL.Substring(0, SQL.Length - 1) + ") values (";
            string[] APs = new string[AFields.Length];

            for (int i = 0; i < AFields.Length; i++)
            {
                APs[i] = "@AP_" + AFields[i];

                SQL += APs[i] + " ,";

            }
            SQL = SQL.Substring(0, SQL.Length - 1) + ") ";
            return DoExecute(SQL, APs, AValues);
        }
        /// <summary>
        /// 更新数据表
        /// </summary>
        /// <param name="ATable"></param>
        /// <param name="AFields"></param>
        /// <param name="AValues"></param>
        /// <param name="ACondFields"></param>
        /// <param name="ACondValues"></param>
        /// <returns></returns>
        public static int DoUpdate(string ATable, string[] AFields, object[] AValues,
                string[] ACondFields, object[] ACondValues
            )
        {
            string[] APs = new string[AFields.Length + ACondFields.Length];
            object[] AVs = new object[AValues.Length + ACondValues.Length];
            string SQL = "Update " + ATable + " Set ";
            for (int i = 0; i < AFields.Length; i++)
            {
                APs[i] = "@AF_" + AFields[i];
                AVs[i] = AValues[i];
                SQL += AFields[i] + " =" + APs[i] + " ,";
            }
            SQL = SQL.Substring(0, SQL.Length - 1);
            if (ACondValues != null)
            {
                SQL += " where (1>0) ";
                for (int i = 0; i < ACondFields.Length; i++)
                {
                    APs[i + AFields.Length] = "@AP_" + ACondFields[i];
                    AVs[i + AFields.Length] = ACondValues[i];
                    SQL += " and " + ACondFields[i] + " =" + APs[i + AFields.Length];
                }
            }
            return DoExecute(SQL, APs, AVs);
        }
        public static int DoDelete(string ATable, string[] ACondFields, object[] ACondValues)
        {
            string[] APs = new string[ACondFields.Length];
            object[] AVs = new object[ACondValues.Length];
            string SQL = "Delete From  " + ATable + "  ";

            if (ACondValues != null)
            {
                SQL += " where (1>0) ";
                for (int i = 0; i < ACondFields.Length; i++)
                {
                    APs[i] = "@AP_" + ACondFields[i];
                    AVs[i] = ACondValues[i];
                    SQL += " and " + ACondFields[i] + " =" + APs[i];
                }
            }
            return DoExecute(SQL, APs, AVs);
        }
    }
}