using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using System.Threading.Tasks;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts for simple crud operation services
    /// </summary>
    public interface ICrudService : IService
    {
        /// <summary>
        /// Adds a record to specified data source within a non-blocking context
        /// </summary>
        /// <param name="data">Recorded to be added</param>
        /// <param name="context">Handle to intended data source</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> CreateAsync(IDataContext context, IData data);

        /// <summary>
        /// Reads a single record from the specified data source within a non-blocking context
        /// </summary>
        /// <param name="context">Handle to intended data source</param>
        /// <param name="id">Id of the record to be read</param>
        /// <returns>A callback handle providing access to the record instance</returns>
        Task<IData> ReadAsync(IDataContext context, object id);

        /// <summary>
        /// Updates a record instance from specified data source within a non-blocking context
        /// </summary>
        /// <param name="context">Handle to intended data source</param>
        /// <param name="id">Id of the record to be updated</param>
        /// <param name="newData">Data object encapsulating update</param>
        /// <param name="excludeNulls">An ooption to prevent updating of fields with null values</param>
        /// <returns>A callback handle providing access to the status code indicating result of the operation</returns>
        Task<StatusCode> UpdateAsync(IDataContext context, object id, IData newData, bool excludeNulls = false);

        /// <summary>
        /// Deletes a record instance from specified data source within a non-blocking context
        /// </summary>
        /// <param name="context">Handle to the intended data source</param>
        /// <param name="id">Id of record to be deleted</param>
        /// <returns>A callback handle providing access to the status code indicating result of the operation</returns>
        Task<StatusCode> DeleteAsync(IDataContext context, object id);
    }
}