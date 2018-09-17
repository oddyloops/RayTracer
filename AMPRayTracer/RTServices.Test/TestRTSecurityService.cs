using RTMeld;
using RTMeld.Services;
using System;
using System.Collections.Generic;
using Xunit;

namespace RTServices.Test
{
    
    public class TestRTSecurityService
    {
        ISecurityService service;
        byte[] key;
        byte[] testData;
        string testUsername;
        string testPwd;

        public TestRTSecurityService()
        {
            service = new RTSecurityService();
            service.ConfigContext = new TestConnectionContext();
            key = new byte[16]; //AES-128
            (new Random()).NextBytes(key);
            testData = new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 };
            testUsername = "user1";
            testPwd = "P@$$w0rd";
        }

        [Fact]
        public void TestHash()
        {
            Assert.Equal(32, service.Hash(testData).Length); //32 bytes hash
        }

        [Fact]
        public void TestSalt()
        {
            byte[] salt;
            byte[] saltedMessage = service.Salt(testData, out salt);
            Assert.Equal(saltedMessage.Length, salt.Length + testData.Length);
        }


        [Fact]
        public void TestSaltWithLength()
        {
            int saltLength = 32;
            byte[] salt;
            byte[] saltedMessage = service.Salt(testData, out salt, saltLength);
            Assert.Equal(saltedMessage.Length, saltLength+ testData.Length);
        }


        [Fact]
        public void TestEncryptAndDecrypt()
        {
            byte[] encrypted = service.Encrypt(testData, key);
            byte[] decrypted = service.Decrypt(encrypted, key);
            Assert.True(Util.ArrayEquals(testData, decrypted));
        }


        [Fact]
        public void TestEncryptAndDecryptCredentialds()
        {
            byte[] encryptedCred = service.EncryptCredentials(testUsername, testPwd, key);
            var decryptedCred = service.DecryptCredentials(encryptedCred, key);
            Assert.Equal(decryptedCred.Item1, testUsername);
            Assert.Equal(decryptedCred.Item2, testPwd);
        }
    }
}
