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
            throw new NotImplementedException();
        }

        public string GetConnectionString(string key)
        {
            switch (key)
            {
                case "DefaultSQLAzureConnection":
                    return "Server=tcp:rt-data-relational.database.windows.net,1433;Initial Catalog=RT_Relational;Persist Security Info=False;User ID=rt;Password=@1318Thomas;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False;Connection Timeout=30";
            }
            return null;
        }
    }
}
