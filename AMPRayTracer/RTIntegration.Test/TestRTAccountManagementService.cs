using RTContainer;
using RTMeld.DataTransport;
using RTMeld.DependencyManagement;
using RTMeld.Enums;
using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Text;
using Xunit;

namespace RTIntegration.Test
{
    public class TestRTAccountManagementService
    {

        ICompositor container;
        IAccountManagementService service;
        IRTUser user;

        public TestRTAccountManagementService()
        {
            container = new Compositor();
            container.Compose();
            service = container.GetInstance<IAccountManagementService>();
            string mailApi = service.ConfigContext.GetAppSetting("SendGrid");
            IKeyStoreService keyStore = container.GetInstance<IKeyStoreService>();
            byte[] key = new byte[16];
            (new Random()).NextBytes(key); //16 bytes sym key
            keyStore.AddOrUpdateKey(service.ConfigContext.GetAppSetting("SymmetricKeyIndex"), key);
            ISecurityService securityService = container.GetInstance<ISecurityService>();
            string mailCredentialFile = service.ConfigContext.GetAppSetting("SendGridEncrypted");
            System.IO.File.WriteAllBytes(mailCredentialFile, securityService.Encrypt(System.Text.Encoding.UTF8.GetBytes(mailApi), key));


            user = container.GetInstance<IRTUser>();
            user.UserName = "rt_test_user";
            user.Email = service.ConfigContext.GetAppSetting("TestRecipient");
            user.Id = Guid.NewGuid();
            user.Password = Encoding.UTF8.GetBytes("P@$$w0rd");
        }
        
        [Fact]
        public async void TestCreateAndClose()
        {
            var status = await service.CreateUserAccountAsync(user);
            Assert.Equal(StatusCode.Successful, status);
            status = await service.CreateUserAccountAsync(user);
            Assert.Equal(StatusCode.Exists, status);
            var testUser = await service.SqlAzureDataContext.SelectOneAsync<IRTUser>(user.Id);
            Assert.Equal(AccountStatus.NeedsVerification, (AccountStatus)testUser.Status);
            status = await service.CloseUserAccountAsync(user);
            Assert.Equal(StatusCode.Successful, status);
            testUser = await service.SqlAzureDataContext.SelectOneAsync<IRTUser>(user.Id);
            Assert.Equal(AccountStatus.Closed, (AccountStatus)testUser.Status);
            await service.SqlAzureDataContext.DeleteAsync<IRTUser>(user.Id);
            IDictionary<string, object> parameters = new Dictionary<string, object>
            {
                { "@P0", user.Id.ToString() }
            };
            var userLogs = (await service.CosmosDataContext.QueryAsync<IRTUserLog>("SELECT * FROM rt_user_log U WHERE U.UserId = @P0", parameters));
            var log = userLogs[0];
            await service.CosmosDataContext.DeleteAsync<IRTUserLog>(log.Id);

        }

        ~TestRTAccountManagementService()
        {
            IKeyStoreService service = container.GetInstance<IKeyStoreService>();
            service.Clear();
        }
    }
}
