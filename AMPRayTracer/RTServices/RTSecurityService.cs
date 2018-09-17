using RTMeld;
using RTMeld.Config;
using RTMeld.DataAccess;
using RTMeld.Enums;
using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Composition;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace RTServices
{
    [Export(typeof(ISecurityService))]
    public class RTSecurityService : ISecurityService
    {

        private const int DEFAULT_SALT_LEN = 20;

        [Import("JsonConfiguration")]

        public IConnectionContext ConfigContext { get; set; }


        #region HelperMethods
        private static Aes CreateAes(byte[] key, int blockSize)
        {
            Aes aes = new AesCryptoServiceProvider();

            aes.KeySize = key.Length * 8;
            aes.BlockSize = blockSize * 8;
            aes.Key = key;
            aes.Mode = CipherMode.CBC;
            aes.Padding = PaddingMode.PKCS7;
            aes.GenerateIV();


            return aes;
        }
        #endregion
        public byte[] Decrypt(byte[] encryptedMessage, byte[] key)
        {
            int blockSize = int.Parse(ConfigContext.GetAppSetting(Config.ENCRYPTION_BLOCK_SIZE_BYTES));
            Aes aes = CreateAes(key, blockSize);
            byte[] decrypted = null;
            //read iv out of cipher buffer
            byte[] iv = new byte[encryptedMessage[0]]; //1st byte for iv length
            for(int i = 1; i <= iv.Length; i++)
            {
                iv[i - 1] = encryptedMessage[i];
            }
            aes.IV = iv;
            using (ICryptoTransform crypto = aes.CreateDecryptor())
            {
                //decrypt message
                using (MemoryStream ms = new MemoryStream())
                {
                    using (var cs = new CryptoStream(ms, crypto, CryptoStreamMode.Write))
                    {
                        cs.Write(encryptedMessage, iv.Length + 1, encryptedMessage.Length - iv.Length - 1);
                    }
                    decrypted = ms.ToArray();
                }
                
            }
            aes.Clear();
            return decrypted;
          
        }

        public Tuple<string, string> DecryptCredentials(byte[] encryptedCredentials, byte[] key)
        {
            byte[] decryptedCredentials = Decrypt(encryptedCredentials, key);
            int userLength = decryptedCredentials[0];
            int pwdLength = decryptedCredentials[userLength + 1];

            byte[] userBuffer = new byte[userLength];
            byte[] pwdBuffer = new byte[pwdLength];
            Array.Copy(decryptedCredentials, 1, userBuffer, 0, userLength);
            Array.Copy(decryptedCredentials, userLength + 2, pwdBuffer, 0, pwdLength);

            return new Tuple<string, string>(Encoding.UTF8.GetString(userBuffer),
                Encoding.UTF8.GetString(pwdBuffer));
        }

        public byte[] Encrypt(byte[] message, byte[] key)
        {
            int blockSize = int.Parse(ConfigContext.GetAppSetting(Config.ENCRYPTION_BLOCK_SIZE_BYTES));
            Aes aes = CreateAes(key,  blockSize);
            byte padLength = (byte)(message.Length < blockSize ? blockSize - message.Length : message.Length % blockSize);
            byte[] encrypted = null;
            using (ICryptoTransform crypto = aes.CreateEncryptor())
            {
                using (MemoryStream ms = new MemoryStream())
                {

                    //encrypt
                    using (var cs = new CryptoStream(ms, crypto, CryptoStreamMode.Write))
                    {
                        //add iv length and iv to buffer unencrypted
                        ms.Write(new byte[] { (byte)aes.IV.Length }, 0, 1);
                        ms.Write(aes.IV, 0, aes.IV.Length);
                        cs.Write(message, 0, message.Length);
                    }
                    encrypted = ms.ToArray();
                }

            }
            aes.Clear();
            return encrypted;
        }



        public byte[] EncryptCredentials(string username, string password, byte[] key)
        {
            if (username.Length > byte.MaxValue)
            {
                throw new RTException("Username exceeds 255 characters", StatusCode.InvalidOp);
            }

            if (password.Length > byte.MaxValue)
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
            return Encrypt(credentialsBuffer, key);
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

        public byte[] Salt(byte[] message, out byte[] salt)
        {
            return Salt(message, out salt, DEFAULT_SALT_LEN);
        }



        public byte[] Salt(byte[] message, out byte[] salt, int saltLength)
        {
            salt = new byte[saltLength];

            using (var rng = new RNGCryptoServiceProvider())
            {
                rng.GetBytes(salt);
            }
            byte[] saltedMessage = new byte[message.Length + salt.Length];
            Array.Copy(message, saltedMessage, message.Length);
            Array.Copy(salt, 0, saltedMessage, message.Length, salt.Length);
            return saltedMessage;
        }


    }
}
