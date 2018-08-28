
using RTMeld.Attributes;

namespace RTDataAccess.DataObjects.Cosmos;
{
    public interface ICosmos
    {
        [Key]
        [Map("id,Id")]
        [Newtonsoft.Json.JsonProperty(PropertyName = "id")]
        string Id { get; set; }
    }
}
