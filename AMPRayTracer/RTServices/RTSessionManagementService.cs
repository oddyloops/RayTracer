using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using System.Threading.Tasks;
using System.Composition;
using RTMeld;

namespace RTServices
{
    [Export(typeof(ISessionManagementService))]
    public class RTSessionManagementService : ISessionManagementService
    {
        [Import("RTSqlAzureContext")]
        public IDataContext SqlAzureDataContext { get; set; }

        [Import("RTCosmosDBContext")]
        public IDataContext CosmosDataContext { get; set; }

        [Import]
        public ISecurityService SecurityService { get; set; }

        public async Task<IRTUserSession> LogInAsync(IRTUser user)
        {
            var results = await SqlAzureDataContext.SelectMatchingAsync<IRTUser>(u => u.UserName.ToLower() == user.UserName.ToLower()
           || u.Email.ToLower() == user.Email.ToLower());
            if (results != null && results.Count() > 0)
            {
                IRTUser returnedUser = results.First();
                byte[] saltedPwd = new byte[user.Password.Length + returnedUser.Salt.Length];
                byte[] saltedHash = SecurityService.Hash(saltedPwd);
                if (Util.ArrayEquals(saltedHash, returnedUser.Password))
                {
                    switch (returnedUser.Status)
                    {
                        case ((int)AccountStatus.Closed):
                            throw new RTException("This user account is closed.", StatusCode.InvalidOp);
                        case ((int)AccountStatus.Suspended):
                            throw new RTException("This user account is suspended.", StatusCode.InvalidOp);
                        case ((int)AccountStatus.NeedsVerification):
                            throw new RTException("User account has not gone through email verification.", StatusCode.Unverified);
                        default: //active account
                            //create new session
                            IRTUserSession session = Util.Container.GetInstance<IRTUserSession>();
                            session.Id = Guid.NewGuid();
                            session.LogInTime = DateTime.UtcNow;
                            session.SessionId = SecurityService.Hash(session.Id.ToByteArray());
                            session.UserId = returnedUser.Id;
                            return session;

                    }
                }
            }
            return null;
        }

        public Task<StatusCode> LogOutAsync(IRTUserSession userSession)
        {
            
        }
    }
}
