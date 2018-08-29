using Microsoft.Extensions.Configuration;
using RTMeld.DataAccess;
using System;
using System.Collections.Generic;
using System.Composition;
using System.Text;

namespace RTDataAccess.DataConnections
{
    [Export("JsonConfig")]
    public class JsonConfiguration : IConnectionContext
    {

        IConfigurationRoot confRoot;

        public JsonConfiguration(string jsonFile)
        {
            IConfigurationBuilder config = new ConfigurationBuilder();
            config.AddJsonFile(jsonFile);
            confRoot = config.Build();

        }
        public string GetAppSetting(string key)
        {
            return confRoot[key];
        }

        public string GetConnectionString(string key)
        {
            return confRoot[key];
        }
    }
}
