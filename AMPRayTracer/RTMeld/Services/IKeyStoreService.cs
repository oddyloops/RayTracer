using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all key stores must satisfy
    /// </summary>
    public interface IKeyStoreService : IService
    {
        /// <summary>
        /// Adds or replaces symmetric key to the store
        /// </summary>
        /// <param name="index">Index for identifying the key</param>
        /// <param name="key">key to be stored</param>
        void AddOrUpdateKey(string index, byte[] key);

        /// <summary>
        /// Retrieves a symmetric key from store by index
        /// </summary>
        /// <param name="index">Index for identifying the key</param>
        /// <returns>Key to be retrieved</returns>
        byte[] GetKey(string index);

        /// <summary>
        /// Removes key from store by the specified index
        /// </summary>
        /// <param name="index">Index for identifying the key</param>
        void DeleteKey(string index);

        /// <summary>
        /// Flushes the entire key store along with all resources it uses
        /// </summary>
        void Clear();

    }
}
