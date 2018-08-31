using RTMeld;
using RTMeld.Config;
using RTMeld.DataAccess;
using RTMeld.Enums;
using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Composition;
using System.Security.Cryptography;
using System.Text;

namespace RTServices
{
    [Export(typeof(ISecurityService))]
    public class RTSecurityService : ISecurityService
    {
        [Import("JsonConfiguration")]

        IConnectionContext ConfigContext { get; set; }


        #region HelperMethods
        private static Aes CreateAes(byte[] key, byte[] iv, int blockSize)
        {
            Aes aes = new AesCryptoServiceProvider
            {
                KeySize = key.Length,
                Key = key,
                Mode = CipherMode.CTS,
                BlockSize = blockSize
            };
            if (iv != null)
            {
                aes.IV = iv;
            }

            return aes;
        }
        #endregion
        public byte[] Decrypt(byte[] encryptedMessage, byte[] key, byte[] iv = null)
        {
            int blockSize = int.Parse(ConfigContext.GetAppSetting(Config.ENCRYPTION_BLOCK_SIZE_BYTES));
            Aes aes = CreateAes(key, iv, blockSize);
            byte[] decrypted = new byte[encryptedMessage.Length];
            using (ICryptoTransform crypto = aes.CreateDecryptor())
            {
                int iterations = encryptedMessage.Length / blockSize;
                for (int i = 0; i < iterations; i++)
                {
                    crypto.TransformBlock(encryptedMessage, i * blockSize, blockSize, decrypted, i * blockSize);
                }
                crypto.TransformFinalBlock(encryptedMessage, iterations * blockSize, encryptedMessage.Length % (iterations * blockSize));
            }
            return decrypted;
        }

        public Tuple<string, string> DecryptCredentials(byte[] encryptedCredentials, byte[] key, byte[] iv = null)
        {
            byte[] decryptedCredentials = Decrypt(encryptedCredentials, key, iv);
            int userLength = decryptedCredentials[0];
            int pwdLength = decryptedCredentials[userLength + 1];

            byte[] userBuffer = new byte[userLength];
            byte[] pwdBuffer = new byte[pwdLength];
            Array.Copy(decryptedCredentials, 1, userBuffer, 0, userLength);
            Array.Copy(decryptedCredentials, userLength + 2, pwdBuffer, 0, pwdLength);

            return new Tuple<string, string>(Encoding.UTF8.GetString(userBuffer),
                Encoding.UTF8.GetString(pwdBuffer));
        }

        public byte[] Encrypt(byte[] message, byte[] key, byte[] iv = null)
        {
            int blockSize = int.Parse(ConfigContext.GetAppSetting(Config.ENCRYPTION_BLOCK_SIZE_BYTES));
            Aes aes = CreateAes(key, iv, blockSize);
            byte[] encrypted = new byte[message.Length];
            using (ICryptoTransform crypto = aes.CreateEncryptor())
            {
                int iterations = message.Length / blockSize;
                for (int i = 0; i < iterations; i++)
                {
                    crypto.TransformBlock(message, i * blockSize, blockSize, encrypted, i * blockSize);
                }
                crypto.TransformFinalBlock(message, iterations * blockSize, message.Length % (iterations * blockSize));
            }
            return encrypted;
        }

      

        public byte[] EncryptCredentials(string username, string password, byte[] key, byte[] iv = null)
        {
            if(username.Length > byte.MaxValue)
            {
                throw new RTException("Username exceeds 255 characters",StatusCode.InvalidOp);
            }

            if(password.Length > byte.MaxValue)
            {
                throw new RTException("Password exceeds 255 characters", StatusCode.InvalidOp);
            }
            byte userLength = (byte)username.Length;
            byte pwdLength = (byte)password.Length;
            byte[] credentialsBuffer = new byte[2 + userLength + pwdLength]; //+2 to store their lengths
            credentialsBuffer[0] = userLength;
            Array.Copy(Encoding.UTF8.GetBytes(username), 0, credentialsBuffer, 1, userLength);
            credentialsBuffer[userLength + 1] = pwdLength;
            Array.Copy(Encoding.UTF8.GetBytes(password), 0, credentialsBuffer, userLength + 2, pwdLength);
            //forms structure ULength : username : PLength : Password
            return Encrypt(credentialsBuffer, key, iv);
        }

        public byte[] GetKeyFromStore(string index)
        {
            throw new NotImplementedException();
        }

        public byte[] Hash(byte[] message)
        {
            SHA256 sha = new SHA256Managed();
            return sha.ComputeHash(message);
        }

        public byte[] Hash(string message)
        {
            return Hash(Encoding.UTF8.GetBytes(message));
        }

        public byte[] Salt(byte[] message)
        {
            throw new NotImplementedException();
        }

       
    }
}
