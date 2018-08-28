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

        public abstract int Delete<T>(object key) where T : class;


        public abstract int DeleteMatching<T>(Expression<Func<T, bool>> matcher) where T : class;

        public abstract int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap);

        public abstract int Insert<T>(T data) where T : class;

        public virtual int InsertAll<T>(IList<T> data) where T : class
        {
            foreach(var item in data)
            {
                Insert(item);
            }
            return 0;
        }

        public virtual Task<int> InsertAllAsync<T>(IList<T> data) where T : class
        {
            int result = InsertAll(data);
            return Task.FromResult(result);
        }

        public abstract IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap);
        public abstract IEnumerable<T> Query<T>(string exec, IDictionary<string, object> paramMap) where T : class;

        public abstract IEnumerable<T> SelectAll<T>() where T : class;
        public abstract IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher) where T : class;

        public abstract T SelectOne<T>(object key) where T : class;


        public virtual IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length) where T : class
        {
            return SelectMatching<T>(matcher).Skip(from).Take(length).ToList();
        }

        public abstract int Update<T>(object key, T newData, bool excludeNulls = false) where T : class;

        public abstract int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher, bool excludeNulls = false) where T : class;



        public virtual Task CommitAsync()
        {
            Commit();
            return Task.FromResult<object>(null);
        }

        public virtual Task<int> DeleteAsync<T>(object key) where T : class
        {
            int result = Delete<T>(key);
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


        public virtual Task<T> SelectOneAsync<T>(object key) where T : class
        {
            T result = SelectOne<T>(key);
            return Task.FromResult(result);
        }


        public virtual Task<IList<T>> SelectRangeAsync<T>(Expression<Func<T, bool>> matcher, int from, int to) where T : class
        {
            IList<T> results = SelectRange(matcher, from, to);
            return Task.FromResult(results);
        }

        public virtual Task<int> UpdateAsync<T>(object key, T newData, bool excludeNulls = false) where T : class
        {
            int result = Update(key, newData, excludeNulls);
            return Task.FromResult(result);
        }

        public virtual Task<int> UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher, bool excludeNulls = false) where T : class
        {
            int result = UpdateMatching(newData, matcher, excludeNulls);
            return Task.FromResult(result);
        }

        #region Helpers
        /// <summary>
        /// A helper method for checking if specified key type actually exist in the record type
        /// </summary>
        /// <param name="recordType">Record type in which key type is being examined</param>
        /// <param name="key">key type used for validation</param>
        protected virtual void ValidateKeyType(Type keyType, Type recordType)
        {
           
            string keyName = Mapper.GetKeyName(recordType);

            if (keyName == null)
            {
                throw new InvalidOperationException("Object of type " + recordType.Name + " does not contain a key field.");
            }
            foreach (var prop in recordType.GetProperties())
            {
                if (prop.Name == keyName)
                {
                    if (!prop.PropertyType.Equals(keyType))
                    {
                        throw new InvalidOperationException("Object of type " + recordType.Name + " does not contain a key of type " + keyType.Name);
                    }
                    else
                    {
                        return;
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


        public virtual int DeleteAll<T>(IList<T> records) where T : class
        {
            foreach(var record in records)
            {
                Delete<T>(Mapper.GetKeyValue(record));
            }

            return 0;
        }

        public virtual Task<int> DeleteAllAsync<T>(IList<T> records) where T : class
        {
            int result = DeleteAll<T>(records);
            return Task.FromResult(result);
        }

        public virtual int UpdateAll<T>(IList<T> oldData, T newData, bool excludeNulls = false) where T : class
        {
            foreach(var data in oldData)
            {
                Update(Mapper.GetKeyValue(data), newData, excludeNulls);
            }

            return 0;
        }

        public virtual Task<int> UpdateAllAsync<T>(IList<T> oldData, T newData, bool excludeNulls = false) where T : class
        {
            int result = UpdateAll(oldData, newData, excludeNulls);
            return Task.FromResult(result);
        }

    }
}
