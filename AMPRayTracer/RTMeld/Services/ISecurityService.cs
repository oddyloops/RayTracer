

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all security services must satisfy
    /// </summary>
    public interface ISecurityService : IService
    {

        /// <summary>
        /// Compute Message Hash
        /// </summary>
        /// <param name="message">Message byte buffer</param>
        /// <returns>Computed message hash</returns>
        byte[] Hash(byte[] message);

        /// <summary>
        /// Compute Message Hash
        /// </summary>
        /// <param name="message">Message String</param>
        /// <returns>Computed message hash</returns>
        byte[] Hash(string message);


        /// <summary>
        /// Adds a salt to message 
        /// </summary>
        /// <param name="message">Message byte buffer</param>
        /// <returns>Salted message</returns>
        byte[] Salt(byte[] message);

    }
}
