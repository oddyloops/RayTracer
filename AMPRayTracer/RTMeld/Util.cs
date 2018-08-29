using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld
{
    public static class Util
    {
        /// <summary>
        /// Copies the property values of the first parameter into the second provided they are of the same type
        /// </summary>
        /// <param name="obj1"> Source object </param>
        /// <param name="obj2"> Destination object </param>
        public static void DeepCopy(object obj1, object obj2)
        {
            DeepCopy(obj1, obj2, new List<string>());

        }


        /// <summary>
        /// Copies the property values of the first parameter into the second provided they are of the same type
        /// </summary>
        /// <param name="obj1"> Source object </param>
        /// <param name="obj2"> Destination object </param>
        /// <param name="exclusions"> List of fields to exclude in the copy process</param>

        public static void DeepCopy(object obj1, object obj2, IList<string> exclusions)
        {
            if (!obj1.GetType().Equals(obj2.GetType()))
            {
                throw new InvalidOperationException("Cannot deep copy across objects of different types");
            }

            foreach (var prop in obj1.GetType().GetProperties())
            {
                if (!exclusions.Contains(prop.Name))
                {
                    obj2.GetType().GetProperty(prop.Name).SetValue(obj2, prop.GetValue(obj1));
                }
            }
        }

        /// <summary>
        /// Copies the property values of the first parameter into the second excluding nulls, and provided they
        /// are of the same type.
        /// </summary>
        /// <param name="obj1">Source object</param>
        /// <param name="obj2">Destination object</param>
        public static void DeepCopyNoNulls(object obj1, object obj2)
        {
            DeepCopyNoNulls(obj1, obj2, new List<string>());
        }


        /// <summary>
        /// Copies the property values of the first parameter into the second excluding nulls, and provided they
        /// are of the same type.
        /// </summary>
        /// <param name="obj1">Source object</param>
        /// <param name="obj2">Destination object</param>
        /// <param name="exclusions">List of fields to exclude in the copy process</param>
        public static void DeepCopyNoNulls(object obj1, object obj2, IList<string> exclusions)
        {
            if (!obj1.GetType().Equals(obj2.GetType()))
            {
                throw new InvalidOperationException("Cannot deep copy across objects of different types");
            }


            foreach (var prop in obj1.GetType().GetProperties())
            {
                if (!exclusions.Contains(prop.Name))
                {
                    var propValue = prop.GetValue(obj1);
                    if (propValue != null)
                    {
                        obj2.GetType().GetProperty(prop.Name).SetValue(obj2, propValue);
                    }
                }
            }
        }


        /// <summary>
        /// Replaces placeholders in a file with desired value
        /// </summary>
        /// <param name="filePath">Path to file</param>
        /// <param name="templatePairs">Map containing placeholders to be replaced</param>
        /// <returns>New templated string with placeholders replaced</returns>

        public static string BuildStringTemplateFromFile(string filePath,IDictionary<string,string> templatePairs)
        {
            string str = System.IO.File.ReadAllText(filePath);
            return BuildStringTemplate(str, templatePairs);
        }

        /// <summary>
        /// Replaces placeholders in a string with desired value
        /// </summary>
        /// <param name="str">Original string</param>
        /// <param name="templatePairs">Map containing placeholders to be replaced</param>
        /// <returns>New templated string with placeholders replaced</returns>
        public static string BuildStringTemplate(string str, IDictionary<string, string> templatePairs)
        {
            foreach(var pair in templatePairs)
            {
                str = str.Replace(pair.Key, pair.Value);
            }
            return str;
        }

    }
}
