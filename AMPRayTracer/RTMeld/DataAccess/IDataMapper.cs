

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
        /// <returns>Field value</returns>
        object GetValue(string fieldName);

        /// <summary>
        /// Returns a reference field mapped to specified name
        /// </summary>
        /// <param name="fieldName">Mapped field name</param>
        /// <returns>Field reference</returns>
        object GetField(string fieldName);
    }
}
