using RTDataAccess.DataObjects;
using RTMeld.Attributes;


namespace RTDataAccess.Cosmos.DataObjects
{
    [Meta(AzureCosmosDocCollection ="rt_file_type")]
    public class RTCosmosFileType : RTFileType
    {
        [Newtonsoft.Json.JsonProperty(PropertyName = "id")]
        public override string Id { get; set; }
    }

    
}
