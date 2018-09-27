using RTContainer;
using RTMeld.DependencyManagement;
using RTMeld.Enums;
using RTMeld.Services;
using System;
using Xunit;

namespace RTIntegration.Test
{
    public class TestRTCommunicationService
    {
        ICompositor container;
        ICommunicationService service;
        
        public TestRTCommunicationService()
        {
            container = new Compositor();
            container.Compose();
            service = container.GetInstance<ICommunicationService>();
            string mailApi = service.ConfigContext.GetAppSetting("SendGrid");
            IKeyStoreService keyStore = container.GetInstance<IKeyStoreService>();
            byte[] key = new byte[16];
            (new Random()).NextBytes(key); //16 bytes sym key
            keyStore.AddOrUpdateKey(service.ConfigContext.GetAppSetting("SymmetricKeyIndex"), key);
            ISecurityService securityService = container.GetInstance<ISecurityService>();
            string mailCredentialFile = service.ConfigContext.GetAppSetting("SendGridEncrypted");
            System.IO.File.WriteAllBytes(mailCredentialFile, securityService.Encrypt(System.Text.Encoding.UTF8.GetBytes(mailApi), key));

        }


        [Fact]
        public async void TestSendEmailAsync()
        {
            string recipient = service.ConfigContext.GetAppSetting("TestRecipient");
            var statusCode = await service.SendEmailAsync("RT", recipient, "RT Integration Test", "Hello world");
            Assert.Equal(StatusCode.Successful, statusCode);
        }

        ~TestRTCommunicationService()
        {
            IKeyStoreService service = container.GetInstance<IKeyStoreService>();
            service.Clear();
        }
    }
}
