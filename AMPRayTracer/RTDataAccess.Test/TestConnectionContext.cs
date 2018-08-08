using RTMeld.DataAccess;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.Test
{
    class TestConnectionContext : IConnectionContext
    {
        public string GetAppSetting(string key)
        {
            switch (key)
            {
                case "AzureCosmosSQLKey":
                    return "CfGm6dbdXiIEO6O3y4CiTUW3iTQMLJcpg7ukxcVbu0hn2PkMe3DEBGQ4k6RsrLRFzC40J1MdrNLDn3hxNI66sQ==";
                case "AzureCosmosSQLDB":
                    return "RT_Document";
            }
            return null;
        }

        public string GetConnectionString(string key)
        {
            switch (key)
            {
                case "DefaultSQLAzureConnection":
                    return "Server=tcp:rt-data-relational.database.windows.net,1433;Initial Catalog=RT_Relational;Persist Security Info=False;User ID=rt;Password=@1318Thomas;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False;Connection Timeout=30";
                case "DefaultCosmosDBConnection":
                    return "https://rt-data-document.documents.azure.com:443/";
            }
            return null;
        }
    }
}
