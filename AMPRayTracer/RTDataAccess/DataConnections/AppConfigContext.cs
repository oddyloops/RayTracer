using RTMeld.DataAccess;
using System.Configuration;

namespace RTDataAccess.DataConnections
{
    public class AppConfigContext : IConnectionContext
    {
        public string GetAppSetting(string key)
        {
            return ConfigurationManager.AppSettings[key].ToString();
        }

        public string GetConnectionString(string key)
        {
            return ConfigurationManager.ConnectionStrings[key].ConnectionString;
        }
    }
}
