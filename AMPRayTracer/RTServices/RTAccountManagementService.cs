using System;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using RTMeld.Services;
using System.Composition;
using System.Threading.Tasks;
using RTMeld.Config;
using System.Linq;
using RTMeld;
using System.Collections;
using System.Collections.Generic;

namespace RTServices
{

    [Export(typeof(IAccountManagementService))]
    public class RTAccountManagementService : IAccountManagementService
    {
        [Import("RTSqlAzureContext")]
        public IDataContext SqlAzureDataContext { get; set; }

        [Import("RTCosmosDBContext")]
        public IDataContext CosmosDataContext { get; set; }


        [Import]
        public ISecurityService SecurityService { get; set; }

        [Import]
        public ICommunicationService CommunicationService { get; set; }

        [Import("JsonConfig")]
        public IConnectionContext ConfigContext { get; set; }

        #region Helpers
        private async Task<StatusCode> RecoveryHelperAsync(IRTUser user)
        {
           
            byte[] hash = SecurityService.Hash(user.Id.ToByteArray());
            user.RecoveryHash = hash;

            await SqlAzureDataContext.UpdateAsync(user.Id, user, true);

            string linkUrl = Config.RECOVERY_LINK_PREFIX + Convert.ToBase64String(hash);
            IDictionary<string, string> templatePair = new Dictionary<string, string>();
            templatePair.Add("#{link}", linkUrl);

            string emailBody = Util.BuildStringTemplateFromFile(SqlAzureDataContext.Context.GetAppSetting(Config.RECOVERY_MAIL_TEMPLATE_PATH), templatePair);
            string emailSubject = ConfigContext.GetAppSetting(Config.RECOVERY_MAIL_SUBJECT);
            string emailSender = ConfigContext.GetAppSetting(Config.RECOVERY_SENDER_ALIAS);

            return await CommunicationService.SendEmailAsync(emailSender, user.Email, emailSubject, emailBody);
        }
        #endregion


        public async Task<StatusCode> CloseUserAccountAsync(IRTUser user)
        {
            user.Status = (int)AccountStatus.Closed;
            int result = await SqlAzureDataContext.UpdateAsync(user.Id, user, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);

        }

        public async Task<StatusCode> CreateUserAccountAsync(IRTUser user)
        {
            var existing = await SqlAzureDataContext.SelectMatchingAsync<IRTUser>(u => u.UserName.ToLower() == user.UserName.ToLower() || u.Email.ToLower() == user.Email.ToLower());
            if (existing != null)
            {
                return StatusCode.Exists;
            }
            user.Status = (int)StatusCode.Unverified; //email needs to be verified


            byte[] salt = null;
            byte[] saltedPwd = SecurityService.Salt(user.Password, out salt);
            byte[] unsaltedHash = SecurityService.Hash(user.Password);
            byte[] hash = SecurityService.Hash(saltedPwd);
            user.Password = hash; //hash password before storage
            user.Salt = salt;
            await SqlAzureDataContext.InsertAsync(user);
            
            IRTUserLog newLog = Util.Container.GetInstance<IRTUserLog>();
            newLog.UserId = user.Id.ToString();
            newLog.Id = Guid.NewGuid().ToString();
            newLog.PastPwds = new List<string>() { Convert.ToBase64String(unsaltedHash) };
            await CosmosDataContext.InsertAsync(newLog);
            return StatusCode.Successful;
        }

        public async Task<StatusCode> PasswordRecoveryByEmailAsync(string email)
        {
            var users = await SqlAzureDataContext.SelectMatchingAsync<IRTUser>(u => u.Email.ToLower() == email.ToLower());
            if (users == null || users.Count != 1)
            {
                return StatusCode.NotFound;
            }
            IRTUser user = users[0];
            return await RecoveryHelperAsync(user);
        }



        public async Task<StatusCode> PasswordRecoveryByUsernameAsync(string username)
        {
            var users = await SqlAzureDataContext.SelectMatchingAsync<IRTUser>(u => u.UserName.ToLower() == username.ToLower());
            if (users == null || users.Count != 1)
            {
                return StatusCode.NotFound;
            }
            IRTUser user = users[0];
            return await RecoveryHelperAsync(user);
        }

        public async Task<StatusCode> ReopenUserAccountAsync(IRTUser user)
        {
            if(user.Status != (int)AccountStatus.Suspended)
            {
                return StatusCode.InvalidOp;
            }
            user.Status = (int)AccountStatus.Active;
            int result = await SqlAzureDataContext.UpdateAsync(user.Id, user, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);
        }

        public async Task<StatusCode> ResetPasswordAsync(IRTUser user)
        {
            byte[] salt = null;
            byte[] saltedPwd = SecurityService.Salt(user.Password,out salt);
            byte[] hash = SecurityService.Hash(saltedPwd);
            byte[] unsaltedHash = SecurityService.Hash(user.Password);
            string unsaltedHashStr = Convert.ToBase64String(unsaltedHash);
            IDictionary<string, object> parameters = new Dictionary<string, object>
            {
                { "@P0", user.Id.ToString() }
            };
            var userLogs = (await CosmosDataContext.QueryAsync<IRTUserLog>("SELECT * FROM rt_user_log U WHERE U.UserId = @P0", parameters));
            if (userLogs.Count > 0)
            {
                IRTUserLog storedLog = userLogs[0];
                if (storedLog.PastPwds.Contains(unsaltedHashStr))
                {
                    return StatusCode.Exists;
                }
                else
                {
                    storedLog.PastPwds.Add(unsaltedHashStr);
                    await CosmosDataContext.UpdateAsync(storedLog.Id, storedLog, true);
                }
            }
            else
            {
                IRTUserLog newUserLog = Util.Container.GetInstance<IRTUserLog>();
                newUserLog.Id = Guid.NewGuid().ToString();
                newUserLog.UserId = user.Id.ToString();
                newUserLog.PastPwds = new List<string>() { unsaltedHashStr };
                await CosmosDataContext.InsertAsync(newUserLog);
            }

            user.Password = hash;
            user.Salt = salt;
            int result = await SqlAzureDataContext.UpdateAsync(user.Id, user, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);

        }

        public async Task<StatusCode> SuspendUserAccountAsync(IRTUser user)
        {
            user.Status = (int)AccountStatus.Suspended;
            int result = await SqlAzureDataContext.UpdateAsync(user.Id, user, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);
        }
    }
}
