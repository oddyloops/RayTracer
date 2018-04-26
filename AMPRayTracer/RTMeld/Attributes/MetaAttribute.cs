using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Attributes
{
    /// <summary>
    /// An optional attribute used to store entity specific information
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
    public class MetaAttribute : Attribute
    {
        /// <summary>
        /// Specifies the name of the Document Collection entity is associated with
        /// </summary>
        public string AzureCosmosDocCollection { get; set; }
    }
}
