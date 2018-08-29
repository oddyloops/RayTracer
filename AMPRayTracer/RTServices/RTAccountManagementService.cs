using System;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using RTMeld.Services;
using System.Composition;
using System.Threading.Tasks;
using RTMeld.Config;
using System.Text;
using RTMeld;
using System.Collections;
using System.Collections.Generic;

namespace RTServices
{
   
    [Export(typeof(IAccountManagementService))]
    public class RTAccountManagementService : IAccountManagementService
    {
        [Import("RTSqlAzureContext")]
        public IDataContext DataContext { get; set; }

        [Import]
        public ISecurityService SecurityService { get; set; }

        [Import]
        public ICommunicationService CommunicationService { get; set; }

        public async Task<StatusCode> CloseUserAccountAsync(IRTUser user)
        {
            user.Status = (int)AccountStatus.Closed;
            int result =await DataContext.UpdateAsync(user.Id, user, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);

        }

        public async Task<StatusCode> CreateUserAccountAsync(IRTUser user)
        {
            var existing = await DataContext.SelectMatchingAsync<IRTUser>(u => u.UserName.ToLower() == user.UserName.ToLower() || u.Email.ToLower() == user.Email.ToLower());
            if(existing != null)
            {
                return StatusCode.Exists;
            }
            await DataContext.InsertAsync(user);
            return StatusCode.Successful;
        }

        public async Task<StatusCode> PasswordRecoveryByEmailAsync(string email)
        {
            var users = await DataContext.SelectMatchingAsync<IRTUser>(u => u.Email.ToLower() == email.ToLower());
            if (users == null || users.Count != 1)
            {
                return StatusCode.NotFound;
            }
            IRTUser user = users[0];
            byte[] saltedId = SecurityService.Salt(user.Id.ToByteArray());
            byte[] hash = SecurityService.Hash(saltedId);
            user.RecoveryHash = hash;

            await DataContext.UpdateAsync(user.Id, user, true);

            string linkUrl = Config.DEFAULT_RECOVERY_LINK_PREFIX + Convert.ToBase64String(hash);
            IDictionary<string, string> templatePair = new Dictionary<string, string>();
            templatePair.Add("#{link}",linkUrl);

            string emailBody = Util.BuildStringTemplateFromFile(DataContext.Context.GetAppSetting( Config.RECOVERY_MAIL_TEMPLATE_PATH),templatePair);
            string emailSubject = DataContext.Context.GetAppSetting(Config.RECOVERY_MAIL_SUBJECT);
            string emailSender = DataContext.Context.GetAppSetting(Config.RECOVERY_SENDER_ALIAS);

            return await CommunicationService.SendEmailAsync(emailSender, user.Email, emailSubject, emailBody);      
        }

        public async Task<StatusCode> PasswordRecoveryByUsername(string username)
        {
            throw new NotImplementedException();
        }

        public StatusCode ReopenUserAccount(IRTUser user)
        {
            throw new NotImplementedException();
        }

        public StatusCode ResetPassword(IRTUser user)
        {
            throw new NotImplementedException();
        }

        public StatusCode SuspendUserAccount(IRTUser user)
        {
            throw new NotImplementedException();
        }
    }
}
