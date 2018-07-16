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
        private IDataMapper mapper;
        private IConnectionContext context;

        protected string connStr;

        public virtual IDataMapper Mapper { get => mapper; set => mapper = value; }

        public virtual IConnectionContext Context { get => context; set => context = value; }

        public abstract void Connect();
        public virtual void Connect(string str)
        {
            this.connStr = str;
        }

        public DataContext(IConnectionContext _context, IDataMapper _mapper)
        {
            context = _context;
            mapper = _mapper;
        }

        public abstract void Commit();
        public abstract void RollBack();
        public abstract int Delete<K, T>(K key) where T : class;

        public abstract int DeleteMatching<T>(Expression<Func<T, bool>> matcher) where T : class;

        public abstract int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap);

        public abstract int Insert<T>(T data) where T : class;

        public virtual int Insert<T>(IList<T> data) where T : class
        {
            foreach(var item in data)
            {
                Insert(item);
            }
            return 0;
        }

        public virtual Task<int> InsertAsync<T>(IList<T> data) where T : class
        {
            int result = Insert(data);
            return Task.FromResult(result);
        }

        public abstract IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap);
        public abstract IEnumerable<T> Query<T>(string exec, IDictionary<string, object> paramMap) where T : class;

        public abstract IEnumerable<T> SelectAll<T>() where T : class;
        public abstract IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher) where T : class;

        public abstract T SelectOne<T, K>(K key) where T : class;

        public virtual IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length) where T : class
        {
            return SelectMatching<T>(matcher).Skip(from).Take(length).ToList();
        }

        public abstract int Update<K, T>(K key, T newData) where T : class;

        public abstract int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher) where T : class;


        public virtual Task CommitAsync()
        {
            Commit();
            return Task.FromResult<object>(null);
        }

        public virtual Task<int> DeleteAsync<K, T>(K key) where T : class
        {
            int result = Delete<K, T>(key);
            return Task.FromResult(result);
        }

        public virtual Task<int> DeleteMatchingAsync<T>(Expression<Func<T, bool>> matcher) where T : class
        {
            int result = DeleteMatching(matcher);
            return Task.FromResult(result);
        }


        public virtual Task<int> ExecuteNonQueryAsync(string exec, IDictionary<string, object> paramMap)
        {
            int result = ExecuteNonQuery(exec, paramMap);
            return Task.FromResult(result);
        }


        public virtual Task<int> InsertAsync<T>(T data) where T : class
        {
            int result = Insert(data);
            return Task.FromResult(result);
        }


        public virtual Task<IList<T>> QueryAsync<T>(string exec, IDictionary<string, object> paramMap) where T : class
        {
            IList<T> results = Query<T>(exec, paramMap).ToList();
            return Task.FromResult(results);
        }

        public virtual Task<IList<T>> SelectMatchingAsync<T>(Expression<Func<T, bool>> matcher) where T : class
        {
            IList<T> results = SelectMatching(matcher).ToList();
            return Task.FromResult(results);
        }

        public virtual Task<T> SelectOneAsync<T, K>(K key) where T : class
        {
            T result = SelectOne<T, K>(key);
            return Task.FromResult(result);
        }


        public virtual Task<IList<T>> SelectRangeAsync<T>(Expression<Func<T, bool>> matcher, int from, int to) where T : class
        {
            IList<T> results = SelectRange(matcher, from, to);
            return Task.FromResult(results);
        }


        public virtual Task<int> UpdateAsync<K, T>(K key, T newData) where T : class
        {
            int result = Update(key, newData);
            return Task.FromResult(result);
        }

        public virtual Task<int> UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher) where T : class
        {
            int result = UpdateMatching(newData, matcher);
            return Task.FromResult(result);
        }

        #region Helpers
        /// <summary>
        /// A helper method for checking if specified key type K actually exist in type T
        /// </summary>
        /// <typeparam name="T">Recrod Type</typeparam>
        /// <typeparam name="K">Key Type</typeparam>
        protected virtual void ValidateKeyType<T, K>() where T : class
        {
            Type tType = typeof(T);
            string keyName = Mapper.GetKeyName(tType);

            if (keyName == null)
            {
                throw new InvalidOperationException("Object of type " + tType.Name + " does not contain a key field.");
            }
            foreach (var prop in tType.GetProperties())
            {
                if (prop.Name == keyName)
                {
                    if (!prop.PropertyType.Equals(typeof(K)))
                    {
                        throw new InvalidOperationException("Object of type " + tType.Name + " does not contain a key of type " + typeof(K).Name);
                    }
                }
            }
        }

        /// <summary>
        /// Helper method to set key field of an object to a value
        /// </summary>
        /// <param name="obj">object</param>
        /// <param name="keyValue">value key is to be set</param>
        protected virtual void SetKeyField(object obj,object keyValue)
        {
            string keyName = Mapper.GetKeyName(obj.GetType());
            if (keyName == null)
            {
                throw new InvalidOperationException("Type " + obj.GetType() + " does not contain a key field");
            }
            Mapper.SetFieldValue(keyName, keyValue, obj);
        }
#endregion
        public virtual void Dispose()
        {
            //do nothing for classes that do not need to explicitly close their connection
        }


    }
}
