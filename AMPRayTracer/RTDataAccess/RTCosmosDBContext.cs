using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq.Expressions;
using System.Text;

namespace RTDataAccess
{
    /// <summary>
    /// IDataContext compliant wrapper around the Azure Cosmos DB Table API
    /// </summary>
    public class RTCosmoDBContext : DataContext
    {

        CloudStorageAccount account;
        CloudTable tableHandle;

        #region HelperMethods
        private void ValidateEntityType<T>()
        {
            if(!(typeof(T).IsSubclassOf(  typeof(TableEntity))))
            {
                throw new InvalidOperationException("Only instances of TableEntity can be interfaced with Azure Table");
            }
        }
#endregion
        public override void Commit()
        {
            throw new NotImplementedException();
        }

        public override void Connect()
        {
            Connect("DefaultCosmosDBConnection");
        }

        public override void Connect(string str)
        {
            base.Connect(str);
            account = CloudStorageAccount.Parse(ConfigurationManager.ConnectionStrings[str].ConnectionString);
            CloudTableClient tableClient = account.CreateCloudTableClient();
            tableHandle = tableClient.GetTableReference(ConfigurationManager.AppSettings["AzureCosmosDBTable"].ToString());
            tableHandle.CreateIfNotExistsAsync().ContinueWith(result => { if (!result.Result) {
                    throw new Exception("Error getting table handle from Azure Cosmos DB");
                } });
        }

        public override int Delete<K, T>(K key)
        {
            ValidateKeyType<T, K>();
            ValidateEntityType<T>();
            
            return 0;
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }

        public override int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override int Insert<T>(T data)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> Query<T>(string exec, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override void RollBack()
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> SelectAll<T>()
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }

        public override T SelectOne<T, K>(K key)
        {
            throw new NotImplementedException();
        }

        public override IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length)
        {
            throw new NotImplementedException();
        }

        public override int Update<K, T>(K key, T newData)
        {
            throw new NotImplementedException();
        }

        public override int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }
    }
}
