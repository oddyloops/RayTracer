using System;
using System.Linq;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using RTMeld.DataAccess;

namespace RTDataAccess
{
    /// <summary>
    /// Contains all reusable methods spread across all other data contexts
    /// </summary>
    public abstract class DataContext : IDataContext
    {
        public abstract void Connect();
        public abstract void Connect(string str);
        public abstract void Commit();
        public abstract void RollBack();
        public abstract int Delete<K>(K key, Type type);
        
        public abstract int DeleteMatching<T>(Expression<Func<T, bool>> matcher);
       
        public abstract int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap);
        public abstract int ExecuteNonQuery<T>(string exec,IDataMapper mapper);
        public abstract int Insert<T>(T data);
       
        public abstract IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap);
        public abstract IEnumerable<T> Query<T>(string exec) where T : IDataMapper;
       
        public abstract IEnumerable<T> SelectAll<T>();
        public abstract IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher);
        
        public abstract T SelectOne<T, K>(K key);
       
        public abstract IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length);
        
        public abstract int Update<K, T>(K key, T newData);
       
        public abstract int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher);
        

        public virtual Task<int> DeleteAsync<K>(K key, Type type)
        {
            int result = Delete(key, type);
            return Task.FromResult(result);
        }

        public virtual Task<int> DeleteMatchingAsync<T>(Expression<Func<T, bool>> matcher)
        {
            int result = DeleteMatching(matcher);
            return Task.FromResult(result);
        }


        public virtual Task<int> ExecuteNonQueryAsync(string exec, IDictionary<string, object> paramMap)
        {
            int result = ExecuteNonQuery(exec, paramMap);
            return Task.FromResult(result);
        }


        public virtual Task<int> ExecuteNonQueryAsync<T>(string exec,IDataMapper mapper) where T : IDataMapper
        {
            int result = ExecuteNonQuery<T>(exec,mapper);
            return Task.FromResult(result);
        }


        public virtual Task<int> InsertAsync<T>(T data)
        {
            int result = Insert(data);
            return Task.FromResult(result);
        }


        public virtual Task<IList<T>> QueryAsync<T>(string exec) where T : IDataMapper
        {
            IList<T> results = Query<T>(exec).ToList();
            return Task.FromResult(results);
        }

        public virtual Task<IList<T>> SelectMatchingAsync<T>(Expression<Func<T, bool>> matcher)
        {
            IList<T> results = SelectMatching(matcher).ToList();
            return Task.FromResult(results);
        }

        public virtual Task<T> SelectOneAsync<T, K>(K key)
        {
           T result = SelectOne<T,K>(key);
           return Task.FromResult(result);
        }


        public virtual Task<IList<T>> SelectRangeAsync<T>(Expression<Func<T, bool>> matcher, int from, int to)
        {
            IList<T> results = SelectRange(matcher, from, to);
            return Task.FromResult(results);
        }


        public virtual Task<int> UpdateAsync<K, T>(K key, T newData)
        {
            int result = Update(key, newData);
            return Task.FromResult(result);
        }

        public virtual Task<int> UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher)
        {
            int result = UpdateMatching(newData, matcher);
            return Task.FromResult(result);
        }
    }
}
