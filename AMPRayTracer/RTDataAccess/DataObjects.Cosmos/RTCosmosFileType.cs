using RTMeld.Attributes;


namespace RTDataAccess.DataObjects.Cosmos
{
    [Meta(AzureCosmosDocCollection ="rt_file_type")]
    public class RTCosmosFileType : RTFileType
    {
        [Newtonsoft.Json.JsonProperty(PropertyName = "id")]
        public override string Id { get; set; }
    }

    
}
