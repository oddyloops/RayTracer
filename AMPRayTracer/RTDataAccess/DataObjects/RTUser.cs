using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public class RTUser : IRTUser
    {
        protected Guid id;

        protected string userName;

        protected byte[] password;

        protected string email;

        protected ICollection<IRTUserSession> userSessions;
        public virtual Guid Id { get => id; set => id = value; }
        public virtual string UserName { get => userName; set => userName = value; }
        public virtual byte[] Password { get => password; set => password = value; }
        public virtual string Email { get => email; set => email = value; }
        public virtual ICollection<IRTUserSession> UserSessions { get => userSessions; set => userSessions =value; }

        public virtual object GetField(string fieldName)
        {
            throw new NotImplementedException();
        }

        public virtual object GetValue(string fieldName)
        {
            throw new NotImplementedException();
        }
    }
}
