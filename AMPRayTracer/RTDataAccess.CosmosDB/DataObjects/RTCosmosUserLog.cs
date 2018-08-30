using RTDataAccess.DataObjects;
using RTMeld.Attributes;
using System;
using System.Collections.Generic;
using System.Composition;
using System.Text;

namespace RTDataAccess.CosmosDB.DataObjects
{
    [Export("RTCosmosUserLog")]
    [Meta(AzureCosmosDocCollection = "rt_user_log")]
    public class RTCosmosUserLog : RTUserLog
    {
        [Newtonsoft.Json.JsonProperty(PropertyName = "id")]
        public override string Id { get; set; }
    }
}
