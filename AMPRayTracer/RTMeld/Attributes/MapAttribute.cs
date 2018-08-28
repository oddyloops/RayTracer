using System;
using System.Collections.Generic;
using System.Linq;

namespace RTMeld.Attributes
{
    /// <summary>
    /// Used for mapping field names to other strings
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = true)]
    public class MapAttribute : Attribute
    {
        private IList<string> maps;

        /// <summary>
        /// Comma separated list of field names property can be mapped to
        /// </summary>
        /// <param name="maps"></param>
        public MapAttribute(string maps)
        {
            string[] mapArr = maps.Split(',');
            this.maps = mapArr.ToList();
        }

        /// <summary>
        /// Returns all the strings field is mapped to (apart from its actual field name)
        /// </summary>
        public IList<string> Maps { get { return maps;  } }

    }
}
