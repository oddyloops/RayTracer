using RTMeld.Config;
using RTMeld.DataAccess;
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
        
        public byte[] Decrypt(byte[] encryptedMessage, byte[] key, byte[] iv = null)
        {
            throw new NotImplementedException();
        }

        public Tuple<string, string> DecryptCredentials(byte[] encryptedCredentials, byte[] key, byte[] iv = null)
        {
            throw new NotImplementedException();
        }

        public byte[] Encrypt(byte[] message, byte[] key, byte[] iv = null)
        {
            int blockSize = int.Parse(ConfigContext.GetAppSetting(Config.ENCRYPTION_BLOCK_SIZE_BYTES));
            Aes aes = new AesCryptoServiceProvider
            {
                KeySize = key.Length,
                Key = key,
                Mode = CipherMode.CTS,
                BlockSize =  blockSize
            };
            if (iv != null)
            {
                aes.IV = iv;
            }
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
            throw new NotImplementedException();
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
