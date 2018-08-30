using RTMeld.DataAccess;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Services
{
    /// <summary>
    /// Contract that all data accessing services should satisfy
    /// </summary>
    public interface IDataService : IService
    {
        /// <summary>
        /// Accessor for sql azure data context
        /// </summary>
        IDataContext SqlAzureDataContext { get; set; }

        /// <summary>
        /// Accessor for cosmos DB data context
        /// </summary>
        IDataContext CosmosDataContext { get; set; }
    }
}
