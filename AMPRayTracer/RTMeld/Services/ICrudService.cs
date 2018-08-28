using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;


namespace RTMeld.Services
{
    /// <summary>
    /// Contracts for simple crud operation services
    /// </summary>
    public interface ICrudService : IService
    {
        /// <summary>
        /// Adds a record to specified data source
        /// </summary>
        /// <param name="data">Recorded to be added</param>
        /// <param name="context">Handle to intended data source</param>
        /// <returns>Status code indicating result of the operation</returns>
        StatusCode Create(IDataContext context, IData data);

        /// <summary>
        /// Reads a single record from the specified data source
        /// </summary>
        /// <param name="context">Handle to intended data source</param>
        /// <param name="id">Id of the record to be read</param>
        /// <returns>Record instance</returns>
        IData Read(IDataContext context, object id);

        /// <summary>
        /// Updates a record instance from specified data source
        /// </summary>
        /// <param name="context">Handle to intended data source</param>
        /// <param name="id">Id of the record to be updated</param>
        /// <param name="newData">Data object encapsulating update</param>
        /// <param name="excludeNulls">An ooption to prevent updating of fields with null values</param>
        /// <returns>Status code indicating result of the operation</returns>
        StatusCode Update(IDataContext context, object id, IData newData, bool excludeNulls = false);

        /// <summary>
        /// Deletes a record instance from specified data source
        /// </summary>
        /// <param name="context">Handle to the intended data source</param>
        /// <param name="id">Id of record to be deleted</param>
        /// <returns>Status code indicating result of the operation</returns>
        StatusCode Delete(IDataContext context, object id);
    }
}