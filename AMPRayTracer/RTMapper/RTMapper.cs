using RTMeld.Attributes;
using RTMeld.DataAccess;
using System;
using System.Linq;
using System.Reflection;

namespace RTMapper
{
    /// <summary>
    /// A concrete implementation of the IDataMapper interface
    /// </summary>
    public class RTMapper : IDataMapper
    {

        #region HelperMethods
        private Attribute GetAttribute(Type objType, Type attrType, bool isClass = false)
        {
            Attribute result = null;
            if (isClass)
            {
                // a class attribute
                var attrs = objType.GetCustomAttributes(attrType, false);
                if (attrType != null)
                {
                    result = attrs.First() as Attribute;
                }
                else
                {
                    foreach (var _interface in objType.GetInterfaces())
                    { //recursively search its implemented interfaces (.net bug not performing this ancestral search)

                        result = GetAttribute(_interface, attrType, isClass);
                        if (result != null)
                        {
                            return result;
                        }
                    }
                }
            }
            else
            {
                //is a field attribute
                foreach (var field in objType.GetProperties())
                {
                    var attrs = field.GetCustomAttributes(attrType, false);

                    if (attrs != null)
                    {
                        result = attrs.First() as Attribute;
                    }
                }

                if (result == null)
                {
                    foreach (var _interface in objType.GetInterfaces())
                    {
                        result = GetAttribute(_interface, attrType, isClass);
                        if (result != null)
                        {
                            return result;
                        }
                    }
                }
            }
            return result;
        }

        private PropertyInfo GetPropertyWithAttr(Type objType, Type attrType)
        {

            foreach (var property in objType.GetProperties())
            {
                var attrs = property.GetCustomAttributes(attrType, false);
                if (attrs != null)
                {
                    return property;
                }
            }
            foreach (var _interface in objType.GetInterfaces())
            { //recursively search its implemented interfaces (.net bug not performing this ancestral search)

                var field = GetPropertyWithAttr(_interface, attrType);
                if (field != null)
                {
                    return field;
                }
            }
            return null;
        }


        private PropertyInfo GetFieldByName(string fieldName, Type objType)
        {
            foreach (var field in objType.GetProperties())
            {

                var mapAttr = field.GetCustomAttributes(typeof(MapAttribute), false);
                if (fieldName.Equals(field.Name) || (mapAttr != null && (mapAttr.First() as MapAttribute).Maps.Contains(fieldName)))
                {
                    return field;
                }
                else
                {
                    foreach (var _interface in objType.GetInterfaces())
                    {
                        var result = GetFieldByName(fieldName, _interface);
                        if (result != null)
                        {
                            return result;
                        }
                    }
                }
            }
            return null;
        }
        #endregion
        public string GetAzureDocumentCollection(Type objType)
        {
            MetaAttribute metaAttr = GetAttribute(objType, typeof(MetaAttribute), true)
                as MetaAttribute;
            if (metaAttr != null)
                return metaAttr.AzureCosmosDocCollection;
            return null;
        }

        public object GetField(string fieldName, object obj)
        {
            PropertyInfo field = GetFieldByName(fieldName, obj.GetType());

            if(field != null)
            {
                return field.GetValue(obj);
            }
            return null;
            
        }



        public string GetKeyName(Type objType)
        {

            PropertyInfo keyField = GetPropertyWithAttr(objType, typeof(KeyAttribute));
            if (keyField != null)
            {
                return keyField.Name;
            }
            return null;
        }

        public object GetKeyValue(object obj)
        {
            string keyName = GetKeyName(obj.GetType());

            if(keyName != null)
            {
                return GetField(keyName, obj);
            }

            return null;
        }

        public void SetFieldValue(string fieldName, object value, object obj)
        {
            PropertyInfo field = GetFieldByName(fieldName, obj.GetType());
            if (field != null)
            {
                field.SetValue(obj, value);
            }
            else
            {
                throw new InvalidOperationException("Field with name " + fieldName +
                    " cannot be found in object " + obj.GetType());
            }

        }
    }

}
