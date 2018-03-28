using System;
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
        public abstract int Delete<K>(K key, Type type);
        
        public abstract int DeleteMatching<T>(Expression<Func<T, bool>> matcher);
        public abstract Task<int> DeleteMatchingAsync<T>(Expression<Func<T, bool>> matcher);
        public abstract int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap);
        public abstract int ExecuteNonQuery<T>(string exec) where T : IDataMapper;
        public abstract Task<int> ExecuteNonQueryAsync(string exec, IDictionary<string, object> paramMap);
        public abstract Task<int> ExecuteNonQueryAsync<T>(string exec) where T : IDataMapper;
        public abstract int Insert<T>(T data);
        public abstract Task<int> InsertAsync<T>(T data);
        public abstract IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap);
        public abstract IEnumerable<T> Query<T>(string exec) where T : IDataMapper;
        public abstract Task<IList<T>> QueryAsync<T>(string exec) where T : IDataMapper;
        public abstract IEnumerable<T> SelectAll<T>();
        public abstract IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher);
        public abstract Task<IList<T>> SelectMatchingAsync<T>(Expression<Func<T, bool>> matcher);
        public abstract T SelectOne<T, K>(K key);
        public abstract Task<T> SelectOneAsync<T, K>(K key);
        public abstract IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length);
        public abstract Task<IList<T>> SelectRangeAsync<T>(Expression<Func<T, bool>> matcher, int from, int to);
        public abstract int Update<K, T>(K key, T newData);
        public abstract Task<int> UpdateAsync<K, T>(K key, T newData);
        public abstract int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher);
        public abstract int UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher);

        public Task<int> DeleteAsync<K>(K key, Type type)
        {
            int result = Delete(key, type);
            return Task.FromResult<int>(result);
        }
    }
}
