using RTMeld.Enums;
using System;

namespace RTMeld
{
    public class RTException : Exception
    {
        public StatusCode ErrorCode { get; set; }


        public RTException(string message) : base(message)
        {
            
        }

        public RTException(string message, StatusCode errorCode) : base (message)
        {
            ErrorCode = errorCode;
        }
    }
}
