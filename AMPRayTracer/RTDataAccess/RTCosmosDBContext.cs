using Microsoft.Azure.Documents;
using System;
using System.Collections.Generic;
using System.Configuration;
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
            if (statusCode != System.Net.HttpStatusCode.OK)
            {
                throw new IOException("Network error when connecting to Cosmos DB table service");
            }
        }
        #endregion

        public RTCosmoDBContext(IConnectionContext _context, IDataMapper _mapper) :
            base(_context,_mapper)
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
            client = new DocumentClient(new Uri(ConfigurationManager.ConnectionStrings[str].ConnectionString), ConfigurationManager.AppSettings[KEY].ToString());
            database = ConfigurationManager.AppSettings[DB].ToString();
            client.CreateDatabaseIfNotExistsAsync(new Database { Id = database }).ContinueWith(result => ThrowOnHttpFailure(result.Result.StatusCode));


        }

        public override int Delete<K, T>(K key)
        {

            ValidateKeyType<T, K>();
            client.DeleteDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()))
                .ContinueWith(result => ThrowOnHttpFailure(result.Result.StatusCode));
            return 0;
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {

            IList<T> matched = SelectMatching(matcher).ToList();

            IList<Task<ResourceResponse<Document>>> deletionTasks = new List<Task<ResourceResponse<Document>>>();
            foreach (T match in matched)
            {
                deletionTasks.Add(client.DeleteDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), Mapper.GetKeyValue(match).ToString())));
            }
            Task.WhenAll(deletionTasks).ContinueWith(result =>
            {
                foreach (var outcome in result.Result)
                {
                    ThrowOnHttpFailure(outcome.StatusCode);
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
            client.CreateDocumentAsync(UriFactory.CreateDocumentCollectionUri(database, Mapper.GetAzureDocumentCollection(typeof(T))), data)
                .ContinueWith(result => ThrowOnHttpFailure(result.Result.StatusCode));
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

        public override T SelectOne<T, K>(K key)
        {
            ValidateKeyType<T, K>();
            return client.CreateDocumentQuery<T>(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString())).First();
        }

   

        public override int Update<K, T>(K key, T newData)
        {
            client.ReplaceDocumentAsync(UriFactory.CreateDocumentUri(database,Mapper.GetAzureDocumentCollection(typeof(T)),key.ToString()),newData)
                 .ContinueWith(result => ThrowOnHttpFailure(result.Result.StatusCode));
            return 0;

        }

        public override int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher)
        {
            var matches = SelectMatching(matcher);
            foreach(var match in matches)
            {
                Update(Mapper.GetKeyValue(match), newData);
            }
            return 0;
        }
    }
}
