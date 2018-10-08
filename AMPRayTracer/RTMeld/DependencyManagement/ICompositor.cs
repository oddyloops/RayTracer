using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.DependencyManagement
{
    /// <summary>
    /// Contracts that all composition manager for dependency management must fulfill
    /// </summary>
    public interface ICompositor
    {
        /// <summary>
        /// Builds the composition catalogs for application based on assemblies from the specified directory
        /// <paramref name="assemblyDir">Directory containing assembly for composing catalogs (empty string means current working directory)</paramref>
        /// </summary>
        void Compose(string assemblyDir = "");

        /// <summary>
        /// Gets a single instance of a type T implementation from composed container
        /// </summary>
        /// <typeparam name="T">Type of instance to be retrieved</typeparam>
        /// <returns>Concrete instance of T from composed container</returns>
        T GetInstance<T>();

        /// <summary>
        /// Gets a single instance of a type T implementation from composed container exporting the specified contract
        /// </summary>
        /// <typeparam name="T">Type of instance to be retrieved</typeparam>
        /// <param name="contractName">Exported contract name</param>
        /// <returns>Concrete instance of T from composed container</returns>
        T GetInstance<T>(string contractName);

        /// <summary>
        /// Gets one instance per available type T implementations from composed container
        /// </summary>
        /// <typeparam name="T">Type of instance to be retrieved</typeparam>
        /// <returns>Concrete instances of T from composed container</returns>
        IEnumerable<T> GetInstances<T>();

        /// <summary>
        /// Gets one instance per available type T implementations from composed container exporting the specified contract
        /// </summary>
        /// <typeparam name="T">Type of instance to be retrieved</typeparam>
        /// <param name="contractName">Exported contract name</param>
        /// <returns>Concrete instances of T from composed container</returns>
        IEnumerable<T> GetInstances<T>(string contractName);

    }
}
