using System;


namespace RTMeld.Attributes
{
    [AttributeUsage( AttributeTargets.Property, AllowMultiple = false, Inherited = true)]
    public class UniqueAttribute : Attribute
    {
    }
}
