using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public class RTUserLog : IRTUserLog
    {
        private string id;

        private string userId;

        private ICollection<string> pastPwds;

        public virtual string Id { get => id; set => id = value; }

        public virtual string UserId { get => userId; set => userId = value; }

        public virtual ICollection<string> PastPwds { get => pastPwds; set => pastPwds = value; }

        public override bool Equals(object obj)
        {
            if(obj != null && obj is RTUserLog)
            {
                return Id.Equals(((RTUserLog)obj).Id);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return Id.GetHashCode();
        }
    }
}
