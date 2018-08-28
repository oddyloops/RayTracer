using Microsoft.Extensions.Configuration;
using RTMeld.DataAccess;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Text;

namespace RTDataAccess.Test
{
    class TestConnectionContext : IConnectionContext
    {

        IConfigurationRoot confRoot;

        public TestConnectionContext()
        {
            IConfigurationBuilder config = new ConfigurationBuilder();
            config.AddJsonFile("RTDataAccess.Test.xunit.runner.json");
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
