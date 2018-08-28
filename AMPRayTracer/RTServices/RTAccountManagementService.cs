using System;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using RTMeld.Services;
using System.Composition;

namespace RTServices
{
    [Export(typeof(IAccountManagementService))]
    public class RTAccountManagementService : IAccountManagementService
    {
        [Import]
        public IDataContext DataContext { get; set; }

        public StatusCode CloseUserAccount(IRTUser user)
        {
            user.Status = (int)AccountStatus.Closed;
            int result = DataContext.Update(user.Id, user, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);

        }

        public StatusCode CreateUserAccount(IRTUser user)
        {
            throw new NotImplementedException();
        }

        public StatusCode PasswordRecoveryByEmail(string email)
        {
            throw new NotImplementedException();
        }

        public StatusCode PasswordRecoveryByUsername(string username)
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
