

namespace RTMeld.DataAccess
{
    /// <summary>
    /// Provides accessor methods for retrieving connection and other related settings parameters
    /// </summary>
    public interface IConnectionContext
    {
        /// <summary>
        /// Retrieves connection string mapped to specified key from source 
        /// </summary>
        /// <param name="key">key mapped to connection string</param>
        /// <returns>Mapped connection string</returns>
        string GetConnectionString(string key);

        /// <summary>
        /// Retrieves string mapped to specified key from source
        /// </summary>
        /// <param name="key">key mapped to string</param>
        /// <returns>Mapped string</returns>
        string GetAppSetting(string key);
    }
}
