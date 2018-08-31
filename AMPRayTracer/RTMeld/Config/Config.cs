using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Config
{
    public static class Config
    {
        public const string DEFAULT_SQL_AZURE_CONNECTION = "DefaultSQLAzureConnection";

        public const string DEFAULT_COSMOS_CONNECTION = "DefaultCosmosDBConnection";

        public const string DEFAULT_COSMOS_KEY = "AzureCosmosSQLKey";

        public const string DEFAULT_COSMOS_DB = "AzureCosmosSQLDB";

        public const string RECOVERY_LINK_PREFIX = "RecoveryLinkPrefix";

        public const string RECOVERY_MAIL_TEMPLATE_PATH = "RecoveryMailTemplatePath";

        public const string RECOVERY_MAIL_SUBJECT = "RecoveryMailSubject";

        public const string RECOVERY_SENDER_ALIAS = "RecoverySenderAlias";

        public const string MAIL_HOST = "MailHost";

        public const string MAIL_PORT = "MailPort";

        public const string SYMMETRIC_KEY_INDEX = "SymmetricKeyIndex";

        public const string MAIL_CREDENTIAL_FILE = "MailCredentialFile";

        public const string ENCRYPTION_BLOCK_SIZE_BYTES = "EncryptionBlockSizeBytes";

        public const string KEY_STORE_PATH = "KeyStorePath";

    }
}
