

using RTMeld.DataAccess;

namespace RTMeld.Services
{
    /// <summary>
    /// Marks an object as a service object
    /// </summary>
    public interface IService
    {
        /// <summary>
        /// Provides access to system configuration entries
        /// </summary>
        IConnectionContext ConfigContext { get; set; }
    }
}
