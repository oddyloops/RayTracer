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

        public static string RECOVERY_LINK_PREFIX = "RecoveryLinkPrefix";

        public static string RECOVERY_MAIL_TEMPLATE_PATH = "RecoveryMailTemplatePath";

        public static string RECOVERY_MAIL_SUBJECT = "RecoveryMailSubject";

        public static string RECOVERY_SENDER_ALIAS = "RecoverySenderAlias";

        public static string MAIL_HOST = "MailHost";

        public static string MAIL_PORT = "MailPort";

        public static string SYMMETRIC_KEY_INDEX = "SymmetricKeyIndex";

        public static string MAIL_CREDENTIAL_FILE = "MailCredentialFile";

        public const string ENCRYPTION_BLOCK_SIZE_BYTES = "EncryptionBlockSizeBytes";

    }
}
