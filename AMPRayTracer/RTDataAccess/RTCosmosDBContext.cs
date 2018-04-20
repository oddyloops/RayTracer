using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq.Expressions;
using System.Linq;

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


        private void ThrowOnHttpFailure(int statusCode)
        {
            if(((System.Net.HttpStatusCode)statusCode) != System.Net.HttpStatusCode.OK)
            {
                throw new IOException("Network error when connecting to Cosmos DB table service");
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
                    throw new IOException("Network error when connecting to Cosmos DB table service");
                } });
        }

        public override int Delete<K, T>(K key)
        {
            ValidateKeyType<T, K>();
            ValidateEntityType<T>();
            T entity = Activator.CreateInstance<T>();
            Mapper.SetFieldValue(Mapper.GetKeyName(entity.GetType()), key, entity);
            TableOperation deleteKey = TableOperation.Delete(entity as ITableEntity);
            tableHandle.ExecuteAsync(deleteKey).ContinueWith(result => {
                ThrowOnHttpFailure(result.Result.HttpStatusCode);
            });

            return 0;
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {
            ValidateEntityType<T>();
            IList<T> matched = SelectMatching(matcher).ToList();
            TableBatchOperation batchDelete = new TableBatchOperation();

            foreach(T match in matched)
            {
                batchDelete.Delete(match as ITableEntity);
            }

            tableHandle.ExecuteBatchAsync(batchDelete).ContinueWith(results =>
            {
                foreach(var opResult in results.Result)
                {
                    ThrowOnHttpFailure(opResult.HttpStatusCode);
                }
            });
            return 0;
        }

        public override int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override int Insert<T>(T data)
        {
            ValidateEntityType<T>();
            TableOperation insert = TableOperation.Insert(data as ITableEntity);
            tableHandle.ExecuteAsync(insert).ContinueWith(result => {
                ThrowOnHttpFailure(result.Result.HttpStatusCode);
            });

            return 0;
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

           
            TableQuery<T> allQuery = new TableQuery<T>();
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
