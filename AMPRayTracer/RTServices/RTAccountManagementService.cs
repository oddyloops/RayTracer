using System;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using RTMeld.Services;

namespace RTServices
{
    public class RTAccountManagementService : IAccountManagementService
    {
        private IDataContext dataContext;
        public IDataContext DataContext { get => dataContext; set => dataContext = value; }

        public StatusCode CloseUserAccount(IRTUser user)
        {
            throw new NotImplementedException();
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
