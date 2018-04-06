

namespace RTMeld.DataAccess
{
    /// <summary>
    /// Defines the required specifications for a mapper class
    /// </summary>
    public interface IDataMapper
    {
        /// <summary>
        /// Returns value of field mapped to specified name
        /// </summary>
        /// <param name="fieldName">Mapped field name</param>
        /// <param name="obj">Object to be mapped</param>
        /// <returns>Field value</returns>
        object GetValue(string fieldName,object obj);

        /// <summary>
        /// Returns a reference field mapped to specified name
        /// </summary>
        /// <param name="fieldName">Mapped field name</param>
        /// <param name="obj">Object to be mapped</param>
        /// <returns>Field reference</returns>
        object GetField(string fieldName, object obj);

        /// <summary>
        /// Gets name of object's key field
        /// </summary>
        /// <param name="obj">Object to be mapped</param>
        /// <returns>key field name or null if no key field was found</returns>
        string GetKeyName(object obj);


    }
}
