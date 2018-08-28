using RTMeld.Enums;
using System;

namespace RTMeld
{
    public class RTException : Exception
    {
        public StatusCode ErrorCode { get; set; }
    }
}
