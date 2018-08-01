using Microsoft.Azure.Documents;
using System;
using System.Collections.Generic;
using System.Net;
using System.IO;
using System.Linq.Expressions;
using System.Linq;
using Microsoft.Azure.Documents.Client;
using System.Threading.Tasks;
using RTMeld.DataAccess;

namespace RTDataAccess
{
    /// <summary>
    /// IDataContext compliant wrapper around the Azure Cosmos DB SQL API
    /// </summary>
    public class RTCosmoDBContext : DataContext
    {

        DocumentClient client;
        private string database;

        private const string KEY = "AzureCosmosSQLKey";
        private const string DB = "AzureCosmosSQLDB";

        private IList<HttpStatusCode> successCodes = new
            List<HttpStatusCode>()
        {
            HttpStatusCode.Created,
            HttpStatusCode.OK
        };

        #region HelperMethods

        private SqlParameterCollection MapQueryParams(IDictionary<string, object> paramMap)
        {
            if (paramMap == null)
                return null;
            SqlParameterCollection mapped = new SqlParameterCollection();
            foreach (var param in paramMap)
            {
                mapped.Add(new SqlParameter(param.Key, param.Value));
            }
            return mapped;
        }

        private void ThrowOnHttpFailure(System.Net.HttpStatusCode statusCode)
        {
            if (!successCodes.Contains(statusCode))
            {
                throw new IOException("Network error when connecting to Cosmos DB table service");
            }
        }
        #endregion

        public RTCosmoDBContext(IConnectionContext _context, IDataMapper _mapper) :
            base(_context, _mapper)
        {

        }

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
            client = new DocumentClient(new Uri(Context.GetConnectionString(str)), Context.GetAppSetting(KEY));
            database = Context.GetAppSetting(DB);
            client.CreateDatabaseIfNotExistsAsync(new Database { Id = database }).ContinueWith(result => ThrowOnHttpFailure(result.Result.StatusCode));


        }

        public override int Delete<K, T>(K key)
        {

            throw new NotImplementedException();
        }


        public async override Task<int> DeleteAsync<K, T>(K key)
        {
            ValidateKeyType<T, K>();
            var result = await client.DeleteDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()));
            ThrowOnHttpFailure(result.StatusCode);
            return 0;
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {

            throw new NotImplementedException();
        }


        public async override Task<int> DeleteMatchingAsync<T>(Expression<Func<T, bool>> matcher)
        {

            var results = SelectMatching(matcher).ToList();

            if (results != null)
            {
                IList<T> matched = results.ToList();


                IList<Task<ResourceResponse<Document>>> deletionTasks = new List<Task<ResourceResponse<Document>>>();
                foreach (T match in matched)
                {
                    deletionTasks.Add(client.DeleteDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), Mapper.GetKeyValue(match).ToString())));
                }
                var taskResults = await Task.WhenAll(deletionTasks);

                foreach (var taskRes in taskResults)
                {
                    ThrowOnHttpFailure(taskRes.StatusCode);
                }
            }
            return 0;
        }

        public override int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override int Insert<T>(T data)
        {
            throw new NotImplementedException();
        }

        public override async Task<int> InsertAsync<T>(T data)
        {
            var result = await client.CreateDocumentAsync(UriFactory.CreateDocumentCollectionUri(database, Mapper.GetAzureDocumentCollection(typeof(T))), data);

            ThrowOnHttpFailure(result.StatusCode);
            SetKeyField(data, result.Resource.Id);
            return 0;
        }

        public override IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> Query<T>(string exec, IDictionary<string, object> paramMap)
        {
            SqlParameterCollection parameters = MapQueryParams(paramMap);
            IEnumerable<T> result = client.CreateDocumentQuery<T>(UriFactory.CreateDocumentCollectionUri(database, Mapper.GetAzureDocumentCollection(typeof(T))),
                new SqlQuerySpec() { QueryText = exec, Parameters = parameters });
            return result;
        }

        public override void RollBack()
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> SelectAll<T>()
        {
            var result = from data in client.CreateDocumentQuery<T>(UriFactory.CreateDocumentCollectionUri(database, Mapper.GetAzureDocumentCollection(typeof(T))))
                         select data;
            return result;
        }

        public override IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher)
        {
            var result = from data in client.CreateDocumentQuery<T>(UriFactory.CreateDocumentCollectionUri(database, Mapper.GetAzureDocumentCollection(typeof(T))))
                         where matcher.Compile()(data)
                         select data;
            return result;
        }


        public override T SelectOne<T,K>( K key)
        {
            throw new NotImplementedException();
        }

        public async override Task<T> SelectOneAsync<T, K>(K key)
        {
            ValidateKeyType<T, K>();
            /*
             var matches = from data in client.CreateDocumentQuery<T>(UriFactory.CreateDocumentCollectionUri(database, Mapper.GetAzureDocumentCollection(typeof(T))))
                           where Mapper.GetKeyValue(data).Equals(key)
                           select data;
             if (matches != null && matches.Count() > 0)
             {
                 return matches.First();
             }*/
            var result =await  client.ReadDocumentAsync<T>(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()));
            return result;
        }



        public override int Update<K, T>(K key, T newData)
        {
            throw new NotImplementedException();

        }


        public async override Task<int> UpdateAsync<K, T>(K key, T newData)
        {
            var result = await client.ReplaceDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()), newData);
            ThrowOnHttpFailure(result.StatusCode);
            return 0;

        }

        public override int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }


        public async override Task<int> UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher)
        {
            var matches = SelectMatching(matcher);
            IList<Task<int>> updateTasks = new List<Task<int>>();
            foreach (var match in matches)
            {
                updateTasks.Add(UpdateAsync(Mapper.GetKeyValue(match), newData));
            }
            await Task.WhenAll(updateTasks);

            return 0;
        }
    }
}
