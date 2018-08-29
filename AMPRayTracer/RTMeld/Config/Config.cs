using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Config
{
    public static class Config
    {
        public static string DEFAULT_SQL_AZURE_CONNECTION = "DefaultSQLAzureConnection";

        public static string DEFAULT_COSMOS_CONNECTION = "DefaultCosmosDBConnection";

        public static string DEFAULT_COSMOS_KEY = "AzureCosmosSQLKey";

        public static string DEFAULT_COSMOS_DB = "AzureCosmosSQLDB";

        public static string DEFAULT_RECOVERY_LINK_PREFIX = "";

        public static string RECOVERY_MAIL_TEMPLATE_PATH = "";

        public static string RECOVERY_MAIL_SUBJECT = "RecoveryMailSubject";

        public static string RECOVERY_SENDER_ALIAS = "RecoverySenderAlias";
    }
}
