using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public abstract class RTUserSession : IRTUserSession
    {
        private Guid id;
        private Guid userId;
        private byte[] sessionId;
        private DateTime logInTime;
        
        public virtual Guid Id { get => id; set => id = value; }
        public virtual Guid UserId { get => userId; set => userId = value; }
        public virtual byte[] SessionId { get => sessionId; set => sessionId = value; }
        public virtual DateTime LogInTime { get => logInTime; set => logInTime = value; }

        public override bool Equals(object obj)
        {
            if (obj != null && obj is RTUserSession)
            {
                var temp = (RTUserSession)obj;
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
