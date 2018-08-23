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
using RTMeld;


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
            HttpStatusCode.OK,
            HttpStatusCode.NoContent

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

        public override int Delete<T>(object key)
        {

            throw new NotImplementedException();
        }


        public async override Task<int> DeleteAsync<T>(object key)
        {
            ValidateKeyType(key.GetType(),typeof(T));
            var result = await client.DeleteDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()));
            ThrowOnHttpFailure(result.StatusCode);
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

            throw new NotImplementedException();
        }


        public override T SelectOne<T>(object key)
        {
            throw new NotImplementedException();
        }


        public async override Task<T> SelectOneAsync<T>(object key)
        {
            ValidateKeyType(key.GetType(), typeof(T));
            try
            {
                var result = await client.ReadDocumentAsync<T>(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()));
                return result;
            }
            catch (DocumentClientException ex)
            {
                if (!ex.Message.ToUpper().Contains("RESOURCE NOT FOUND"))
                {
                    throw ex;
                }
            }
            return null;
        }

        public override int Update<T>(object key, T newData, bool excludeNulls = false)
        {
            throw new NotImplementedException();
        }


        public async override Task<int> UpdateAsync<T>(object key, T newData, bool excludeNulls = false)
        {
            if (excludeNulls)
            {
                T oldData = await SelectOneAsync<T>(key);
                Util.DeepCopyNoNulls(newData, oldData);
                var result = await client.ReplaceDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()), oldData);
                ThrowOnHttpFailure(result.StatusCode);
            }
            else
            {
                var result = await client.ReplaceDocumentAsync(UriFactory.CreateDocumentUri(database, Mapper.GetAzureDocumentCollection(typeof(T)), key.ToString()), newData);
                ThrowOnHttpFailure(result.StatusCode);
            }

            return 0;

        }

        public override int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher, bool excludeNulls = false)
        {
            throw new NotImplementedException();
        }



        public async override Task<int> UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher, bool excludeNulls = false)
        {
            throw new NotImplementedException();
        }
    }
}
