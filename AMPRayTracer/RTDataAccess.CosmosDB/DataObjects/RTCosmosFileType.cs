using RTDataAccess.DataObjects;
using RTMeld.Attributes;
using RTMeld.DataTransport;
using System.Composition;

namespace RTDataAccess.Cosmos.DataObjects
{
    [Export(typeof(IRTFileType))]
    [Meta(AzureCosmosDocCollection ="rt_file_type")]
    public class RTCosmosFileType : RTFileType
    {
        [Newtonsoft.Json.JsonProperty(PropertyName = "id")]
        public override string Id { get; set; }
    }

    
}
