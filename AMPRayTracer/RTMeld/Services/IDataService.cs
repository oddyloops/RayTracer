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
        /// Accessor for data context
        /// </summary>
        IDataContext DataContext { get; set; }
    }
}
