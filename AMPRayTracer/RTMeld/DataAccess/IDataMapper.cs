

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
        /// <returns>Field reference</returns>
        object GetField(string fieldName, object obj);

        /// <summary>
        /// Gets name of object's key field
        /// </summary>
        /// <param name="objType">Type of object</param>
        /// <returns>key field name or null if no key field was found</returns>
        string GetKeyName(System.Type objType);

        /// <summary>
        /// Get the value of object's key field
        /// </summary>
        /// <param name="obj">object in question</param>
        /// <returns>key field value or null if no key fields were found</returns>
        object GetKeyValue(object obj);

        /// <summary>
        /// Assigns a value to object's field mapped to specified field name
        /// </summary>
        /// <param name="fieldName">Field name mapped to field of interest</param>
        /// <param name="value">Value to be assigned to field of interest</param>
        /// <param name="obj">Object containing field</param>
        void SetFieldValue(string fieldName, object value, object obj);

        /// <summary>
        /// Gets the name of the azure document collection associated with object type.
        /// </summary>
        /// <param name="objRType">Type of object</param>
        /// <returns>Returns document collection name if meta attribute was found,
        ///  otherwise returns class name.
        /// </returns>
        string GetAzureDocumentCollection(System.Type objRType);

    }
}
