using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public class RTUserSession : IRTUserSession
    {
        protected Guid userId;
        protected byte[] sessionId;
        protected DateTime logInTime;
        protected IRTUser user;

        public virtual Guid UserId { get => userId; set => userId = value; }
        public virtual byte[] SessionId { get => sessionId; set => sessionId = value; }
        public virtual DateTime LogInTime { get => logInTime; set => logInTime = value; }
        public virtual IRTUser User { get => user; set => user = value; }

       
    }
}
