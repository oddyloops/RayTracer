using RTMeld.DataTransport;
using RTMeld.Enums;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public abstract class RTUser : IRTUser
    {
        private Guid id;

        private string userName;

        private byte[] password;

        private string email;

        private int status;

        private byte[] recoveryHash;

        public virtual Guid Id { get => id; set => id = value; }
        public virtual string UserName { get => userName; set => userName = value; }
        public virtual byte[] Password { get => password; set => password = value; }
        public virtual string Email { get => email; set => email = value; }
        public virtual int Status { get => status; set => status = value; }

        public virtual byte[] RecoveryHash { get => recoveryHash; set => recoveryHash = value; }



        public override bool Equals(object obj)
        {
            if (obj != null && obj is RTUser)
            {
                var temp = (RTUser)obj;
                return Id.Equals(temp.Id);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return id.GetHashCode();
        }

    }
}
