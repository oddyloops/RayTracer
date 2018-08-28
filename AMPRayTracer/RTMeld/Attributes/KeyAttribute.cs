using System;



namespace RTMeld.Attributes
{
    /// <summary>
    /// Specifies that a field is part of or the entire composition of the containing object's
    /// primary key
    /// </summary>
    [AttributeUsage(AttributeTargets.Property ,
                       AllowMultiple = false, Inherited = true)]
    public class KeyAttribute : Attribute
    {
        
    }
}
