using System.Threading.Tasks;
using System.Collections.Generic;
using System;
using System.Linq.Expressions;

namespace RTMeld.DataAccess
{
    /// <summary>
    /// Contract that all data access objects must satisfy
    /// </summary>
    public interface IDataContext : IDisposable
    {
        /// <summary>
        /// An instance of a data mapper
        /// </summary>
        IDataMapper Mapper { get; set; }
        /// <summary>
        /// Connects data access object to its data source
        /// </summary>
        void Connect();

        /// <summary>
        /// Connects data access object to its data source
        /// </summary>
        /// <param name="str">A connection string or key to a config entry for the actual connection string</param>
        void Connect(string str);
        /// <summary>
        /// Commits changes to data source
        /// </summary>
        void Commit();

        /// <summary>
        /// Commits changes to data source in a non-blocking context
        /// </summary>
        /// <returns>Handle to status of execution</returns>
        Task CommitAsync();
        /// <summary>
        /// Reverts any accumulated changes that has not been committed
        /// </summary>
        void RollBack();


        #region StronglyTyped
        /// <summary>
        /// Adds a batch of data entries to data source
        /// </summary>
        /// <typeparam name="T">Type of data to be added</typeparam>
        /// <param name="data">Bulk data to be inserted</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int Insert<T>(IList<T> data) where T : class;


        /// <summary>
        /// Adds a batch of data entries to data source within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Type of data to be added</typeparam>
        /// <param name="data">Bulk data to be inserted</param>
        /// <returns>A callback handle that provides access to the status code indicating the result of the operation</returns>
        Task<int> InsertAsync<T>(IList<T> data) where T : class;

        /// <summary>
        /// Adds new data entry to data source
        /// </summary>
        /// <typeparam name="T"> Type of data to be added</typeparam>
        /// <param name="data">Data to be added</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int Insert<T>(T data) where T : class;

        /// <summary>
        /// Adds new data entry to data source within a non-blocking context
        /// </summary>
        /// <typeparam name="T"> Type of data to be added</typeparam>
        /// <param name="data">Data to be added</param>
        /// <returns>A callback handle that provides access to the status code indicating the result of the operation</returns>
        Task<int> InsertAsync<T>(T data) where T : class;


        /// <summary>
        /// Applies modification to record with the corresponding primary key
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <typeparam name="K">Key TYPE</typeparam>
        /// <param name="key">Matching primary key</param>
        /// <param name="newData">New data object encapsulating modifications to be made</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int Update<K,T>(K key,T newData) where T : class;


        /// <summary>
        /// Applies modification to record with the corresponding primary key within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <typeparam name="K">Key type</typeparam>
        /// <param name="key">Matching primary key</param>
        /// <param name="newData">New data object encapsulating modifications to be made</param>
        /// <returns>A callback handle that provides access to the status code indicating the result of the operation</returns>
        Task<int> UpdateAsync<K,T>(K key,T newData) where T : class;

        /// <summary>
        /// Applies modification to record(s) matching the predicate
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="newData">New data object encapsulating modifications to be made</param>
        /// <param name="matcher">Predicate expression used for matching records meant to be updated</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher) where T : class;

        /// <summary>
        /// Applies modification to record(s) matching the predicate within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="newData">New data object encapsulating modifications to be made</param>
        /// <param name="matcher">Predicate expression used for matching records meant to be updated</param>
        /// <returns>A callback handle that provides access to the status code indicating the result of the operation</returns>
        Task<int> UpdateMatchingAsync<T>(T newData, Expression<Func<T, bool>> matcher) where T : class;

        /// <summary>
        /// Removes the record from the data source
        /// </summary>
        /// <typeparam name="K">Primary key type</typeparam>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="key">Primary key of record to be deleted</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int Delete<K, T>(K key) where T : class;

        /// <summary>
        /// Removes the record from the data source within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Key type</typeparam>
        /// <typeparam name="K">Primary key type</typeparam>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="key">Primary key of record to be deleted</param>
        /// <returns>A callback handle that provides access to the status code indicating the result of the operation</returns>
        Task<int> DeleteAsync<K,T>(K key) where T : class;

        /// <summary>
        /// Removes the record(s) matching the predicate
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="matcher">Predicate expression used for matching records meant to be updated</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int DeleteMatching<T>(Expression<Func<T, bool>> matcher) where T : class;

        /// <summary>
        /// Removes the record(s) matching the predicate within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="matcher">Predicate expression used for matching records meant to be updated</param>
        /// <returns>A callback handle that provides access to the status code indicating the result of the operation</returns>
        Task<int> DeleteMatchingAsync<T>(Expression<Func<T, bool>> matcher) where T : class;

        /// <summary>
        /// Returns an iterator for traversing the instances of the record present in the data source
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <returns>The iterator for traversing the results</returns>
        IEnumerable<T> SelectAll<T>() where T : class;

        /// <summary>
        /// Returns a single record with the corresponding primary key
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <typeparam name="K">Key Type</typeparam>
        /// <param name="key">Matching Primary Key</param>
        /// <returns>Instance of matching record</returns>
        T SelectOne<T, K>(K key) where T : class;

        /// <summary>
        /// Returns a single record with the corresponding primary key within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <typeparam name="K">Key Type</typeparam>
        /// <param name="key">Matching Primary Key</param>
        /// <returns>A callback that provides access to the instance of matching record</returns>
        Task<T> SelectOneAsync<T, K>(K key) where T : class;

        /// <summary>
        /// Return record(s) matching the specified predicate
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="matcher">Predicate expression used for matching records meant to be retrieved</param>
        /// <returns>The iterator for traversing the results</returns>
        IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher) where T : class;

        /// <summary>
        /// Return record(s) matching the specified predicate within  a non-blocking context 
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="matcher">Predicate expression used for matching records meant to be retrieved</param>
        /// <returns>A callback that provides access to the list of matching records</returns>
        Task<IList<T>> SelectMatchingAsync<T>(Expression<Func<T, bool>> matcher) where T : class;


        /// <summary>
        /// Provides pagination functionality by selecting predicate matched records within a specified range
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="matcher">Predicate expression used for matching records meant to be retrieved</param>
        /// <param name="from">Starting index</param>
        /// <param name="length">Amount of records to be returned</param>
        /// <returns>A list of matching record within specified range</returns>
        IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length) where T : class;


        /// <summary>
        /// Provides pagination functionality by selecting predicate matched records within a specified range in a non-blocking context
        /// </summary>
        /// <typeparam name="T">Record type</typeparam>
        /// <param name="matcher">Predicate expression used for matching records meant to be retrieved</param>
        /// <param name="from">Starting index</param>
        /// <param name="length">Amount of records to be returned</param>
        /// <returns>A callback that provides access to the list of matching record within specified range</returns>
        Task<IList<T>> SelectRangeAsync<T>(Expression<Func<T, bool>> matcher, int from, int to) where T : class;

        #endregion


        #region DynamicMaps
        /// <summary>
        /// Executes a data altering statement against the underlying datasource with the most basic parameter specification technique
        /// </summary>
        /// <param name="exec">Statement to be executed</param>
        /// <param name="paramMap">A mapping of parameter placeholders to their actual values</param>
        /// <returns>A status code indicating the result of the operation</returns>
        int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap);

        /// <summary>
        /// Executes a data altering statement against the datasource with the most basic parameter specification technique
        /// within a non-blocking context
        /// </summary>
        /// <param name="exec">Statement to be executed</param>
        /// <param name="paramMap">A mapping of parameter placeholders to their actual values</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<int> ExecuteNonQueryAsync(string exec, IDictionary<string, object> paramMap);

        /// <summary>
        /// Executes a data retrieval statement against the underlying datasource with the most basic parameter specification technique
        /// </summary>
        /// <param name="query">Statement to be executed</param>
        /// <param name="paramMap">A mapping of parameter placeholders to their actual values</param>
        /// <returns>An iterator for traversing through the resultset with each entry abstracted as a mapping of fields to values</returns>
        IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap);


        /// <summary>
        /// Executes a data retrieval statement against the underlying datasource using a mapper interface
        /// </summary>
        /// <typeparam name="T">Record type to be retrieved</typeparam>
        /// <param name="exec">Statement to be executed</param>
        /// <param name="paramMap">A mapping of parameter placeholders to their actual values</param>
   
        /// <returns>The iterator for traversing the results</returns>
        IEnumerable<T> Query<T>(string exec,IDictionary<string,object> paramMap) where T : class;

        /// <summary>
        /// Executes a data retrieval statement against the underlying datasource using a mapper interface within a non-blocking context
        /// </summary>
        /// <typeparam name="T">Record type to be retrieved</typeparam>
        /// <param name="exec">Statement to be executed</param>
        /// ///<param name="paramMap">A mapping of parameter placeholders to their actual values</param>

        /// <returns>A callback handle providing access to the list of returned records</returns>
        Task<IList<T>> QueryAsync<T>(string exec,  IDictionary<string, object> paramMap) where T : class;

        #endregion



    }
}
