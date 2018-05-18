﻿using RTMeld.Attributes;
using RTMeld.DataAccess;
using System;

namespace RTMapper
{
    /// <summary>
    /// A concrete implementation of the IDataMapper interface
    /// </summary>
    public class RTMapper : IDataMapper
    {
        public string GetAzureDocumentCollection(Type objRType)
        {
            MetaAttribute metaAttr = (MetaAttribute)Attribute.GetCustomAttribute(objRType, typeof(MetaAttribute));
            return metaAttr.AzureCosmosDocCollection;
        }

        public object GetField(string fieldName, object obj)
        {
            Type tObj = obj.GetType();
            foreach(var field in tObj.GetProperties())
            {
                var mapAttr = Attribute.GetCustomAttribute(field.PropertyType, typeof(MapAttribute))
                    as MapAttribute;
                if(mapAttr != null && mapAttr.Maps.Contains(fieldName))
                {
                    return field.GetValue(obj);
                }
            }
            return null;
        }

        public string GetKeyName(Type objType)
        {
 
            foreach (var field in objType.GetProperties())
            {
                var keyAttr = Attribute.GetCustomAttribute(field.PropertyType, typeof(KeyAttribute));
                if(keyAttr!= null)
                {
                    //found key field 
                    return field.Name;
                }
            }
            return null;
        }

        public object GetKeyValue(object obj)
        {
            foreach (var field in obj.GetType().GetProperties())
            {
                var keyAttr = Attribute.GetCustomAttribute(field.PropertyType, typeof(KeyAttribute));
                if (keyAttr != null)
                {
                    //found key field 
                    return field.GetValue(obj);
                }
            }
            return null;
        }

        public object GetValue(string fieldName, object obj)
        {
            Type tObj = obj.GetType();
            foreach (var field in tObj.GetProperties())
            {
                if (field.Name.Equals(fieldName))
                {
                    return field.GetValue(obj);
                }
            }
            return null;
        }

        public void SetFieldValue(string fieldName, object value, object obj)
        {
            Type tObj = obj.GetType();
            foreach (var field in tObj.GetProperties())
            {
                var mapAttr = Attribute.GetCustomAttribute(field.PropertyType, typeof(MapAttribute))
                    as MapAttribute;
                if (mapAttr != null && mapAttr.Maps.Contains(fieldName))
                {
                    field.SetValue(obj, value);
                    return;
                }
            }
            
        }
    }
    
}
