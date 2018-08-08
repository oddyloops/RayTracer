﻿using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public class RTUser : IRTUser
    {
        private Guid id;

        private string userName;

        private byte[] password;

        private string email;

        public virtual Guid Id { get => id; set => id = value; }
        public virtual string UserName { get => userName; set => userName = value; }
        public virtual byte[] Password { get => password; set => password = value; }
        public virtual string Email { get => email; set => email = value; }

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