using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using Microsoft.EntityFrameworkCore;
using System.Configuration;
using System.Data.SqlClient;
using RTMeld;
using RTMeld.Config;
using RTMeld.DataAccess;
using RTDataAccess.SqlAzure.DataRepos;
using System.Composition;

namespace RTDataAccess.SqlAzure
{
    /// <summary>
    /// Compliant IDataAcess wrapper around the SQL Azure EF interface
    /// </summary>
    [Export("RTSqlAzureContext",typeof(IDataContext))]
    public class RTSqlAzureContext : DataContext
    {
        RTSqlAzureDataRepo repository;

        [Import]
        public override IDataMapper Mapper { get; set; }

       
        public override IConnectionContext Context { get; set; }

        #region HelperMethods
        /// <summary>
        /// Transforms a loosely typed mapping of parameters to their sql equivalent
        /// </summary>
        /// <param name="paramMap">Loosely typed parameters</param>
        /// <returns>List of equivalent SQL parameters</returns>
        private List<SqlParameter> MapQueryParams(IDictionary<string, object> paramMap)
        {
            if (paramMap == null)
                return null;
            List<SqlParameter> paramList = new List<SqlParameter>(paramMap.Count);
            foreach (var key in paramMap.Keys)
            {
                paramList.Add(new SqlParameter(key, paramMap[key]));
            }
            return paramList;
        }


        /// <summary>
        /// Creates a new sql connection object
        /// </summary>
        /// <returns></returns>
        private SqlConnection ConnectADO()
        {
            SqlConnection conn = new SqlConnection(Context.GetConnectionString(connStr));
            return conn;
        }


        /// <summary>
        /// Constructs an SqlCommand object with the specified parameters
        /// </summary>
        /// <param name="cmd">SQL command</param>
        /// <param name="parameters">Command parameters</param>
        /// <returns>Parameterized SQL Command object</returns>
        private SqlCommand BuildADOCommand(string cmd, IList<SqlParameter> parameters)
        {
            SqlCommand command = new SqlCommand(cmd);
            if (parameters != null)
            {
                foreach (SqlParameter param in parameters)
                {
                    command.Parameters.Add(param);
                }
            }
            return command;
        }


        /// <summary>
        /// Clears the entries of entity framework change tracker
        /// </summary>
        /// <typeparam name="T"></typeparam>
        private void ClearTracks<T>() where T : class
        {
            var entries = repository.ChangeTracker.Entries<T>();
            if (entries.Count() > 0)
            {
                foreach (var entity in entries.ToList())
                {
                    entity.State = EntityState.Detached;
                }
            }
        }

        /// <summary>
        /// Returns DBSet for generic interface type
        /// </summary>
        /// <returns></returns>
        private object GetDBSetForType<T>()
        {
            Type efRepoType = repository.GetType();
            var properties = efRepoType.GetProperties();
            foreach (var property in properties)
            {
                if(property.PropertyType.IsGenericType &&
                    property.PropertyType.Equals(typeof(DbSet<>)))
                {
                    Type entityType = property.PropertyType.GetGenericArguments()[0];
                    if(entityType.GetInterfaces().Contains(typeof(T)))
                    {
                        return property;
                    }
                }
            }
            throw new Exception("Interface type not present in dbsets");
        }

        #endregion

       
        [ImportingConstructor]
        public RTSqlAzureContext([Import("JsonConfig")]IConnectionContext context)
        {
            Context = context;
            Connect();
        }

        public override void Connect()
        {
            connStr = Config.DEFAULT_SQL_AZURE_CONNECTION;
            Connect(connStr);
        }

        public override void Connect(string str)
        {
            base.Connect(str);
            var optionsBuilder = new DbContextOptionsBuilder<RTSqlAzureDataRepo>();
            optionsBuilder.UseSqlServer(Context.GetConnectionString(str));
            repository = new RTSqlAzureDataRepo(optionsBuilder.Options);
        }


       
        public override void Commit()
        {
            repository.SaveChanges();
        }

        public override void RollBack()
        {
            repository.Dispose();
            Connect(connStr);

        }


        public override int Delete<T>(object key)
        {
            T entity = Activator.CreateInstance<T>();
            SetKeyField(entity, key);
            ClearTracks<T>();
            repository.Attach(entity);
            repository.Remove(entity);
            Commit();
            return 0;
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {
            List<T> matches = SelectMatching(matcher).ToList();

            if (matches != null)
            {
                repository.RemoveRange(matches);
                Commit();
            }
            return 0;
        }


        public override int DeleteAll<T>(IList<T> records)
        {
            IList<object> keys = (from record in records
                                  select
            Mapper.GetKeyValue(record)).ToList();
            return DeleteMatching<T>(x => keys.Contains(Mapper.GetKeyValue(x)));
        }

        public override int UpdateAll<T>(IList<T> oldData, T newData, bool excludeNulls = false)
        {
            
            IList<object> keys = (from record in oldData
                                  select
            Mapper.GetKeyValue(record)).ToList();
            return UpdateMatching<T>(newData, x => keys.Contains(Mapper.GetKeyValue(x)), excludeNulls);
        }


        public override int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap = null)
        {
            if (paramMap == null)
                return repository.Database.ExecuteSqlCommand(new RawSqlString(exec));
            else
                return repository.Database.ExecuteSqlCommand(new RawSqlString(exec), MapQueryParams(paramMap));

        }



        public override int Insert<T>(T data)
        {
            repository.Add(data);
            Commit();
            return 0;
        }


        public override int InsertAll<T>(IList<T> data)
        {
            repository.AddRange(data);
            Commit();
            return 0;
        }


        public override IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap)
        {

            using (SqlConnection conn = ConnectADO())
            {
                var parameters = MapQueryParams(paramMap);
                SqlCommand command = BuildADOCommand(query, parameters);
                command.Connection = conn;

                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {

                    IDictionary<string, object> row = new Dictionary<string, object>();
                    for (int i = 0; i < reader.FieldCount; i++)
                    {
                        row.Add(reader.GetName(i), reader.GetValue(i));
                    }
                    yield return row;
                }
                reader.Close();
                conn.Close();
            }

        }

        public override IEnumerable<T> Query<T>(string exec, IDictionary<string, object> paramMap)
        {

            using (SqlConnection conn = ConnectADO())
            {
                var parameters = MapQueryParams(paramMap);
                SqlCommand command = BuildADOCommand(exec, parameters);
                command.Connection = conn;
                conn.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    T record = Activator.CreateInstance<T>();
                    for (int i = 0; i < reader.FieldCount; i++)
                    {
                        Mapper.SetFieldValue(reader.GetName(i), reader.GetValue(i), record);
                    }
                    yield return record;
                }
                reader.Close();
                conn.Close();
            }

        }

        public override IEnumerable<T> SelectAll<T>()
        {

            var dbset = (IEnumerable)GetDBSetForType<T>();
            
            return from x in dbset select (T)x;
        }

        public override IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher)
        {
            
            return from x in this.repository.Set<T>() where matcher.Compile()(x) select x;
        }


        public override T SelectOne<T>(object key)
        {
            ValidateKeyType(key.GetType(), typeof(T));
            string keyName = Mapper.GetKeyName(typeof(T));
            var matches = SelectMatching<T>((x => (Mapper.GetField(keyName, x)).Equals(key)));
            if (matches != null && matches.Count() > 0)
            {
                return matches.First();
            }
            return null;
        }





        public override int Update<T>(object key, T newData, bool excludeNulls = false)
        {
            ValidateKeyType(key.GetType(), typeof(T));
            T oldData = SelectOne<T>(key);
            if (excludeNulls)
            {
                Util.DeepCopyNoNulls(newData, oldData);
            }
            else
            {
                Util.DeepCopy(newData, oldData);
            }
            this.repository.Update<T>(oldData);
            Commit();
            return 0;
        }


        public override int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher, bool excludeNulls = false)
        {
            IEnumerable<T> matches = SelectMatching<T>(matcher);
            if (matches != null)
            {
                string keyName = Mapper.GetKeyName(typeof(T));
                foreach (T match in matches)
                {

                    if (excludeNulls)
                    {
                        Util.DeepCopyNoNulls(newData, match, new List<string>() { keyName });
                    }
                    else
                    {
                        Util.DeepCopy(newData, match, new List<string>() { keyName });
                    }

                }
                this.repository.UpdateRange(matches);
                Commit();
            }
            return 0;
        }


        public override void Dispose()
        {
            repository.Database.CloseConnection();
        }
    }
}
