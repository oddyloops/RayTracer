using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using RTDataAccess.DataRepos;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using System.Configuration;
using System.Data.SqlClient;

namespace RTDataAccess
{
    /// <summary>
    /// Compliant IDataAcess wrapper around the SQL Azure EF interface
    /// </summary>
    public class RTSqlAzureContext : DataContext
    {
        RTSqlAzureDataRepo repository;

        private string connStr;

        private List<SqlParameter> MapQueryParams(IDictionary<string,object> paramMap)
        {
            List<SqlParameter> paramList = new List<SqlParameter>(paramMap.Count);
            foreach(var key in paramMap.Keys)
            {
                paramList.Add(new SqlParameter(key, paramMap[key]));
            }
            return paramList;
        }
        public override void Connect()
        {
            connStr = "DefaultAzureConnection";
            Connect(connStr);
        }

        public override void Connect(string str)
        {
            connStr = str;
            var optionsBuilder = new DbContextOptionsBuilder<RTSqlAzureDataRepo>();
            optionsBuilder.UseSqlServer(ConfigurationManager.ConnectionStrings[connStr].ToString());
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

        public override int Delete<K,T>(K key)
        {
            T entity = new T();
            string keyName = Mapper.GetKeyName(entity);
            if(keyName == null)
            {
                throw new InvalidOperationException("Type " + entity.GetType() + " does not contain a key field");
            }
            foreach(var field in entity.GetType().GetProperties())
            {
                if(field.Name == keyName)
                {
                    if(field.GetType() != key.GetType())
                    {
                        throw new InvalidOperationException("Type " + entity.GetType() + " does not contain a key of type " + key.GetType());
                    }
                    field.SetValue(entity, key);
                    break;
                }
            }
            repository.Attach(entity);
            repository.Remove(entity);
            Commit();
            return 0;
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {
            List<T> matches = SelectMatching(matcher).ToList();
            repository.RemoveRange(matches);
            Commit();
            return 0;
        }

        public override int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap = null)
        {
            if(paramMap == null)
                return repository.Database.ExecuteSqlCommand(new RawSqlString(exec));
            else
                return repository.Database.ExecuteSqlCommand(new RawSqlString(exec),MapQueryParams(paramMap));

        }

       

        public override int Insert<T>(T data)
        {
            repository.Add(data);
            Commit();
            return 0;
        }

        public override IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap)
        {
          
        }

        public override IEnumerable<T> Query<T>(string exec)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> SelectAll<T>()
        {
            
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
