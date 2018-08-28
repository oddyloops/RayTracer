﻿
using RTMeld.Attributes;

namespace RTMeld.DataTransport
{
    public interface ICosmos
    {
        [Key]
        [Map("id,Id")]
        [Newtonsoft.Json.JsonProperty(PropertyName = "id")]
        string Id { get; set; }
    }
}
